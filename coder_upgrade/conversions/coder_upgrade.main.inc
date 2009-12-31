<?php
// $Id$

/**
 * @file
 * Main conversion routine file for the coder_upgrade module.
 *
 * The functions in these conversion routine files correspond to the topics in
 * the category roadmap at http://drupal.org/node/394070 that are marked with
 * a "Yes" in the Upgrade column.
 *
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

require_once drupal_get_path('module', 'coder_upgrade') . '/conversions/coder_upgrade.end.inc';
require_once drupal_get_path('module', 'coder_upgrade') . '/conversions/coder_upgrade.other.inc';

/**
 * Prepares conversion environment and starts conversion loop.
 *
 * @param array $upgrades
 *   Array of upgrade sets to apply.
 * @param array $extensions
 *   Array of file types to convert based on extension.
 * @param array $items
 *   Array of directories containing the files to convert.
 * @param boolean $recursive
 *   Indicates whether to recurse the subdirectories of each $item.
 * @return boolean
 *   Indicates whether the conversion code was successfully applied.
 */
function coder_upgrade_start($upgrades, $extensions, $items, $recursive = TRUE) {
  // Declare global variables.
  global $log, $debug, $_coder_upgrade_module_name;
//  global $_coder_upgrade_dirname; // Not used.

  // Check lists in case this function is called apart from form submit.
  if (!is_array($upgrades) || empty($upgrades)) {
    return FALSE;
  }
  if (!is_array($extensions) || empty($extensions)) {
    return FALSE;
  }
  if (!is_array($items) || empty($items)) {
    return FALSE;
  }

  $log = TRUE;
  // Clear the log file.
  if ($log) {
    file_put_contents(coder_upgrade_log_path(), '');
  }
  $debug = TRUE; // FALSE; //
  // Clear the debug file.
  if ($debug) {
    file_put_contents(coder_upgrade_debug_path(), '');
  }

  // Loop on items.
  foreach ($items as $item) {
    $_coder_upgrade_module_name = '';
//    $_coder_upgrade_dirname = $item['old_dir'];
    // TODO Set a global variable indicating the current module name (since this is often needed) - it may change as we enter a subdir.

    // Call main conversion loop.
    coder_upgrade_convert_dir($upgrades, $extensions, $item, $recursive);

    // Apply finishing touches to the directory.
    // TODO Turn this into a loop on routines.
//    $_coder_upgrade_dirname = $item['new_dir'];
    coder_upgrade_convert_end($item['new_dir']);

    // Make a patch file.
    coder_upgrade_make_patch_file($item);
  }

  return TRUE;
}

/**
 * Converts files in a directory.
 *
 * @param array $upgrades
 *   Array of upgrade sets to apply.
 * @param array $extensions
 *   Array of file types to convert based on extension.
 * @param array $item
 *   Array of a directory containing the files to convert.
 * @param boolean $recursive
 *   Indicates whether to recurse the subdirectories of $item.
 */
function coder_upgrade_convert_dir($upgrades, $extensions, $item, $recursive = TRUE) {
//  global $_coder_upgrade_filename; // TODO Only referenced in this function -- remove global
  static $ignore = array('.', '..', 'CVS', '.svn');
  global $_coder_upgrade_module_name;

  $dirname = $item['old_dir'];
  $new_dirname = $item['new_dir'];

  // Create an output directory we can write to.
  if (!is_dir($new_dirname)) {
    mkdir($new_dirname);
    chmod($new_dirname, 0757);
  }
  else {
    coder_upgrade_clean_directory($new_dirname);
  }

  if (!in_array($dirname, $ignore)) {
    coder_upgrade_log_print("\n*************************");
    coder_upgrade_log_print('Converting the directory => ' . $dirname);
    coder_upgrade_log_print("*************************");
  }

  // Determine module name.
  coder_upgrade_module_name($dirname, $item);
  $_coder_upgrade_module_name = $item['module'] ? $item['module'] : $_coder_upgrade_module_name;

  // Loop on files.
  $filenames = scandir($dirname . '/');
  foreach ($filenames as $filename) {
//    $_coder_upgrade_filename = $filename;
    if (!in_array($filename, $ignore)) {
      if (is_dir($dirname . '/' . $filename)) {
        $new_filename = $filename;
        // Handle D6 conversion item #79.
        if ($filename == 'po') {
          $new_filename = 'translations';
        }
        if ($recursive) {
          // TODO Fix this!!!
          $new_item = array(
            'name' => $item['name'],
            'old_dir' => $dirname . '/' . $filename,
            'new_dir' => $new_dirname . '/' . $filename,
          );
          coder_upgrade_convert_dir($upgrades, $extensions, $new_item, $recursive);
          // Reset the module name.
          $_coder_upgrade_module_name = $item['module'];
        }
      }
      elseif (in_array(pathinfo($filename, PATHINFO_EXTENSION), array_keys($extensions))) {
        copy($dirname . '/' . $filename, $new_dirname . '/' . $filename);
        coder_upgrade_log_print("\n*************************");
        coder_upgrade_log_print('Converting the file => ' . $filename);
        coder_upgrade_log_print("*************************");
        coder_upgrade_convert_file($new_dirname . '/' . $filename, $upgrades);
      }
      else {
        copy($dirname . '/' . $filename, $new_dirname . '/' . $filename);
      }
    }
  }
}

/**
 * Converts a file.
 *
 * @param string $filename
 *   The name of the file to convert.
 * @param array $upgrades
 *   Array of upgrade sets to apply.
 */
function coder_upgrade_convert_file($filename, $upgrades = array()) {
  if (!file_exists($filename)) {
    return FALSE;
  }

  // Read the file and copy the contents.
  $cur = file_get_contents($filename);
  $new = $cur;

  if (empty($upgrades)) {
    // TODO Temporary solution for development.
    $upgrades = coder_upgrade_list_upgrades(TRUE);
  }

  // Loop on upgrades.
  foreach ($upgrades as $upgrade) {
    if (!isset($upgrade['prefix']) || !isset($upgrade['routines'])) {
      continue;
    }

    $prefix = $upgrade['prefix'];
    $routines = $upgrade['routines']['middle'];

    // Apply regular expression routines.
    coder_upgrade_apply_regex($filename, $new, $prefix, $routines);
    if (coder_upgrade_parser_routines_exist($routines)) {
      // Apply parser routines.
      coder_upgrade_apply_parser($filename, $new, $prefix, $routines);
    }
  }

  // Write the new file.
  if ($new != $cur) {
    if (file_put_contents($filename, $new) === FALSE) {
      coder_upgrade_log_print('File could not be written');
    }
    coder_upgrade_log_print('Replaced the file');
  }
}

/**
 * Applies regular expression conversion routines to a file.
 *
 * @param string $filename
 *   The name of the file to convert.
 * @param string $new
 *   The contents of the file to convert.
 * @param string $prefix
 *   The function name prefix.
 * @param array $routines
 *   Array of upgrade routines to apply.
 */
function coder_upgrade_apply_regex($filename, &$new, $prefix, $routines = array()) {
  // Categorize certain files.
  $is_info_file = pathinfo($filename, PATHINFO_EXTENSION) == 'info';
  $is_schema_file = pathinfo($filename, PATHINFO_EXTENSION) == 'install';

  // Loop on conversion routines using regular expressions.
  foreach ($routines as $routine) {
    if ($routine['type'] != 'regex') {
      continue;
    }
    if (function_exists($function = $prefix . '_convert_' . $routine['name'])) {
      coder_upgrade_log_print("Applying $function");
      $is_format_conversion = strpos($routine['name'], 'format') === 0;
      $is_info_conversion = strpos($routine['name'], 'info') === 0;
      $is_schema_conversion = in_array($routine['name'], array('hook_schema', 'hook_install', 'hook_uninstall', 'hook_update'));
      // Only run info changes on info files
      // Only run install changes on install files
      if ((!$is_info_file && !$is_info_conversion && !$is_schema_file && !$is_schema_conversion) ||
          ($is_info_file && $is_info_conversion) ||
          ($is_schema_file && $is_schema_conversion) ||
          ($is_format_conversion)) {
        // Apply conversion routine to the file.
        call_user_func_array($function, array(&$new)); // TODO Remove &???
      }
      coder_upgrade_log_print('Completed ' . $function);
    }
    else {
      coder_upgrade_log_print("ERROR: Conversion routine not found: $function");
    }
  }
}

/**
 * Applies grammar parser conversion routines to a file.
 *
 * @param string $filename
 *   The name of the file to convert.
 * @param string $new
 *   The contents of the file to convert.
 * @param string $prefix
 *   The function name prefix.
 * @param array $routines
 *   Array of upgrade routines to apply.
 */
function coder_upgrade_apply_parser($filename, &$new, $prefix, $routines = array()) {
  // Categorize certain files.
  $is_info_file = pathinfo($filename, PATHINFO_EXTENSION) == 'info';
  $is_schema_file = pathinfo($filename, PATHINFO_EXTENSION) == 'install';
  if ($is_info_file) {
    return;
  }

  // Include parser files.
  require_once drupal_get_path('module', 'pgp') . '/engine/pgp.parser.inc';
  require_once drupal_get_path('module', 'pgp') . '/engine/pgp.reader.inc';
  require_once drupal_get_path('module', 'pgp') . '/engine/pgp.writer.inc';
  require_once drupal_get_path('module', 'pgp') . '/engine/pgp.editor.inc';
  require_once drupal_get_path('module', 'pgp') . '/engine/pgp.list.inc';
  require_once drupal_get_path('module', 'pgp') . '/engine/pgp.object.inc';

  // Create reader object; pass it to the routines.
  $reader = new PGPReader($new);
//  $reader->setDebug(TRUE);
  $reader->addTokenNames();
  $reader->buildGrammar();

  // Loop on conversion routines using parser.
  foreach ($routines as $routine) {
    if ($routine['type'] != 'parser') {
      continue;
    }
    if (function_exists($function = $prefix . '_convert_' . $routine['name'])) {
      coder_upgrade_log_print("Applying $function");
      $is_format_conversion = strpos($routine['name'], 'format') === 0;
      $is_info_conversion = strpos($routine['name'], 'info') === 0;
      $is_schema_conversion = in_array($routine['name'], array('hook_schema', 'hook_install', 'hook_uninstall', 'hook_update'));
      // Only run info changes on info files
      // Only run install changes on install files
      if ((!$is_info_file && !$is_info_conversion && !$is_schema_file && !$is_schema_conversion) ||
          ($is_info_file && $is_info_conversion) ||
          ($is_schema_file && $is_schema_conversion) ||
          ($is_format_conversion)) {
        // Apply conversion routine to the file.
        call_user_func_array($function, array(&$reader)); // TODO Remove &???
      }
      coder_upgrade_log_print('Completed ' . $function);
    }
    else {
      coder_upgrade_log_print("ERROR: Conversion routine not found: $function");
    }
  }

  // Use writer to redo file.
  $writer = new PGPWriter();
  $new = $writer->toString($reader->getStatements());
}

/**
 * Returns whether grammar parser routines are present in the list.
 *
 * @param array $routines
 *   Array of upgrade routines.
 * @return boolean
 *   Whether parser routines are present in the list.
 */
function coder_upgrade_parser_routines_exist($routines = array()) {
  // Loop on conversion routines.
  foreach ($routines as $routine) {
    if ($routine['type'] == 'parser') {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Adds the module name to the item array.
 *
 * @param string $dirname
 *   A string of the directory name.
 * @param array $item
 *   Array of a directory containing the files to convert.
 */
function coder_upgrade_module_name($dirname, &$item) {
  // Extensions that indicate a module is present.
  $extensions = array('info', 'module');

  /*
   * Set the module name in case there is no module in the directory (e.g. po
   * or translations).
   *
   * This code assumes at most one module per directory. Absent this condition
   * we have no way of determining the file list for the .info file. This
   * condition does not hold for the devel project which has 4 modules in its
   * top-level directory.
   */
  $item['module'] = '';

  // Loop on files.
  $path = $dirname . '/';
  $files = scandir($path);
  foreach ($files as $file) {
    $file_path = $path . $file;
    if (!is_dir($file_path)) {
      if (in_array(pathinfo($file_path, PATHINFO_EXTENSION), $extensions)) {
        $item['module'] = pathinfo($file_path, PATHINFO_FILENAME);
        break;
      }
    }
  }
}

/**
 * Makes a patch file of the conversion routine changes.
 *
 * @param array $item
 *   Array of the directory containing the files to convert.
 */
function coder_upgrade_make_patch_file($item) {
  // Patch directory.
  $patch_dir = file_directory_path() . '/' . variable_get('coder_upgrade_dir_patch', DEADWOOD_PATCH) . '/';

  // Make a patch file.
  coder_upgrade_log_print("\n*************************");
  coder_upgrade_log_print('Creating a patch file for the directory => ' . $item['old_dir']);
  coder_upgrade_log_print("*************************");
  $patch_filename = $patch_dir . $item['name'] . '.patch';
  coder_upgrade_log_print("Making patch file: diff -u -r {$item['old_dir']} {$item['new_dir']} > {$patch_filename}");
  shell_exec("diff -u -r {$item['old_dir']} {$item['new_dir']} > {$patch_filename}");
}

/**
 * Finds the text of a particular function.
 *
 * This is used with regular expressions, but is obsoleted by the parser.
 *
 * @param string $hook
 *   By default, the suffix of the function name to find.
 *   Example: passing $hook = 'menu' will find a function whose name ends in '_menu'.
 *   When $hook_is_suffix = FALSE, then $hook is the entire function name to find.
 * @param string $file
 *   The file to search.
 * @param boolean $match_all
 *   When TRUE, find all functions with $hook in the name.
 * @param boolean $hook_is_suffix
 *   The $hook is only the suffix of the function name.
 * @return string
 *   The function text.
 */
function coder_upgrade_find_hook($hook, $file, $match_all = FALSE, $hook_is_suffix = TRUE) {
  // Construct pattern based on function parameters.
  $prefix = $hook_is_suffix ? '\w+_' : '';
  $pattern  = '/^function\s*';
//  $pattern .= $hook_is_suffix ? '\w+_' : '';
  $pattern .= $prefix . $hook . '\s*\(.*?(?=(\/\*\*|^function|\z))/ms';

  if ($match_all) {
    preg_match_all($pattern, $file, $matches, PREG_PATTERN_ORDER);
    // This block should be unnecessary with the changes to pattern above.
    if (!isset($matches[0][0])) {
      // Check to see if the function name exists.
      $pattern = '/^function\s*' . $prefix . $hook . '\s*\(/m';
      preg_match($pattern, $file, $matches);
      if (!isset($matches[0])) {
        return array();
      }
      // Find last function in file.
      $pattern = '/^function\s*' . $prefix . $hook . '.*\z/ms';
      preg_match_all($pattern, $file, $matches, PREG_PATTERN_ORDER);
      coder_upgrade_log_print('Primary search failed to find function text for _' . $hook . '. Resorting to secondary pattern to find function.');
    }
    return isset($matches[0]) ? $matches[0] : array();
  }
  else {
    preg_match($pattern, $file, $matches);
    // This block should be unnecessary with the changes to pattern above.
    if (!isset($matches[0])) {
      // Check to see if the function name exists.
      $pattern = '/^function\s*' . $prefix . $hook . '\s*\(/m';
      preg_match($pattern, $file, $matches);
      if (!isset($matches[0])) {
        return '';
      }
      // Find last function in file.
      $pattern = '/^function\s*' . $prefix . $hook . '.*\z/ms';
      preg_match($pattern, $file, $matches);
      coder_upgrade_log_print('Primary search failed to find function text for _' . $hook . '. Resorting to secondary pattern to find function.');
    }
    return isset($matches[0]) ? $matches[0] : '';
  }
}

/**
 * Loops on from and to arrays, converting the text of the subject string.
 *
 * @param string $from
 *   The pattern to search for.
 * @param string $to
 *   The string to replace the pattern with.
 * @param string $subject
 *   The string to search and replace.
 */
function coder_upgrade_do_conversions($from, $to, &$subject) {
  for ($i = 0; $i < count($from); $i++) {
    $subject = preg_replace($from[$i], $to[$i], $subject);
  }
}

/**
 * Saves the regular expression changes back to the file.
 *
 * @param string $cur
 *   The string to search for in $file.
 * @param string $new
 *   The replacement string.
 * @param string $file
 *   The text being replaced (all of or part of a file).
 * @param string $hook
 *   The hook being modified.
 */
function coder_upgrade_save_changes($cur, $new, &$file, $hook) {
  if ($new != $cur) {
    $file = str_replace($cur, $new, $file);
//    coder_upgrade_log_print('Completed conversions for ' . $hook);
  }
}

/**
 * Prints log information if log flag is on.
 *
 * @param mixed $text
 *   A string, array, or object to print.
 */
function coder_upgrade_log_print($text) {
  global $log;

  if (!$log) {
    return;
  }
  if (is_array($text) || is_object($text)) {
    file_put_contents(coder_upgrade_log_path(), print_r($text, 1), FILE_APPEND);
  }
  else {
    file_put_contents(coder_upgrade_log_path(), $text . "\n", FILE_APPEND);
  }
}

/**
 * Prints debug information if debug flag is on.
 *
 * @param mixed $text
 *   A string, array, or object to print.
 */
function coder_upgrade_debug_print($text) {
  global $debug;

  if (!$debug) {
    return;
  }
  if (is_array($text) || is_object($text)) {
    // Caller needs to set the $text using PGPBase::print_r as appropriate.
    file_put_contents(coder_upgrade_debug_path(), $text, FILE_APPEND);
  }
  else {
    file_put_contents(coder_upgrade_debug_path(), $text . "\n", FILE_APPEND);
  }
}

function clp($text) {
	coder_upgrade_log_print($text);
}

function cdp($text) {
  coder_upgrade_debug_print($text);
}

/**
 * Returns path to log file.
 *
 * @return string
 *   Path to file.
 */
function coder_upgrade_log_path() {
  static $path = '';

  if (!$path) {
    $path = file_directory_path() . '/' . DEADWOOD_DIR . '/log.txt';
  }
  return $path;
}

/**
 * Returns path to log file.
 *
 * @return string
 *   Path to file.
 */
function coder_upgrade_debug_path() {
  static $path = '';

  if (!$path) {
    $path = file_directory_path() . '/' . DEADWOOD_DIR . '/debug.txt';
  }
  return $path;
}
