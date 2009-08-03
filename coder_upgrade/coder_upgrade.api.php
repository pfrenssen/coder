<?php
// $Id$

/**
 * @file
 * The api documentation for the coder_upgrade module.
 *
 * Instructions:
 * - Add the contents of this file to a file in your module.
 * - Replace "your_module_name" below with your actual module name.
 * - Replace other "your_" occurrences with actual values.
 * - Complete the conversions_list() routine.
 *
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Implement hook_upgrades().
 *
 * NOTE: Change this function name to your_module_name_upgrades.
 * 
 * @todo Add Doxygen style comments with API documentation.
 *
 * @param boolean $include_routines
 *   Whether to include the list of routines (only needed on submit).
 */
function hook_upgrades($include_routines)  {
  global $_coder_upgrades;

  // If your module has multiple files defining upgrade routines, then refer
  // to coder_upgrade.main.inc for an example with a loop.
  if (!isset($_coder_upgrades)) {
    $_coder_upgrades = array();
    $path = drupal_get_path('module', 'your_module_name') . '/your_subdirectory/your_filename';
    require_once DRUPAL_ROOT . '/' . $path;
    $function = str_replace('.', '_', $file->name) . '_upgrades';
    if (drupal_function_exists($function)) {
      if ($upgrade = call_user_func($function, $include_routines)) {
        $_coder_upgrades = array_merge($_coder_upgrades, $upgrade);
      }
    }
  }
  return $_coder_upgrades;
}

/**
 * Callback from hook_upgrades().
 *
 * @param boolean $include_routines
 *   Whether to include the list of routines (only needed on submit).
 */
function your_module_name_list_upgrades($include_routines) {
  $routines = $include_routines ? your_module_name_conversions_list() : array();
  $upgrade = array(
    'title' => t('Your module API changes from 6.x to 7.x'),
    'link' => 'http://...',
    'routines' => $routines,
    'severity' => 'critical',
  );
  return array('coder_upgrade' => $upgrade);
}

/**
 * Return a list of conversion routines.
 *
 * The complete function name that Coder Upgrade searches for is:
 *   <your_module_name> . '_convert_' . <your_routine_name>
 *
 * The entries in the array returned from this routine should only contain
 * the <your_routine_name> portion.
 *
 * @return array
 *   Array of conversion routine suffixes.
 */
function your_module_name_conversions_list() {
  return array(
    'your_routine_name',
  );
}

/**
 * Describe the upgrade applied by this routine.
 *
 * @param string $file
 *   The file to convert.
 */
function your_module_name_convert_your_routine_name(&$file) {
  // Do something to $file.
}
