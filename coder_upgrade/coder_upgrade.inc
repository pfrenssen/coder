<?php
// $Id$

/**
 * @file
 * Constants and utility functions for the coder_upgrade module.
 *
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * The default directory to store modules to be converted.
 * Relative to file_directory_path().
 */
define('DEADWOOD_DIR', 'coder_upgrade');

/**
 * The default directory to store modules to be converted.
 * Relative to file_directory_path().
 */
define('DEADWOOD_OLD', 'coder_upgrade/old');

/**
 * The default directory to store converted modules.
 * Relative to file_directory_path().
 */
define('DEADWOOD_NEW', 'coder_upgrade/new');

/**
 * Pass a string through t() and wrap the result in html entity <p>.
 */
function tp($string, $args = array()) {
  return '<p>' . t($string, $args) . '</p>';
}

/**
 * Scan a specified directory and find all first-level directories beneath it.
 *
 * TODO Replace this with a call to file_scan_directory in include/files.inc.
 *
 * @param string $path Directory path.
 * @return Array of directory names.
 */
function coder_upgrade_scan_directory($path) {
  static $ignore = array('.', '..', '.svn');
  $dirs = array();

  $path = $path . '/';
  if (!is_dir($path)) {
    return $dirs;
  }
  $files = scandir($path);
  foreach ($files as $file) {
    $file_path = $path . $file;
    if (is_dir($file_path) && !in_array(basename($file_path), $ignore)) {
      $dirs[] = $file;
    }
  }
  return $dirs;
}

/**
 * Remove all files from the specified directory and optionally remove the
 * directory.
 *
 * @param string $path Directory path.
 */
function coder_upgrade_clean_directory($path, $remove_me = FALSE) {
  $path = $path . '/';
  if (!is_dir($path)) {
    return;
  }
  $files = scandir($path);
  foreach ($files as $file) {
    if ($file != '.' && $file != '..') {
      $file_path = $path . $file;
      if (is_dir($file_path)) {
        coder_upgrade_clean_directory($file_path, TRUE);
      }
      else {
        file_unmanaged_delete($file_path);
      }
    }
  }
  if ($remove_me) {
    rmdir($path);
  }
}
