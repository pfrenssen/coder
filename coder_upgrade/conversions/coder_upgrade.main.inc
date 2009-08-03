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

/**
 * Return the list of automated conversion code from all modules.
 *
 * @param boolean $include_routines
 *   Whether to include the list of routines (only needed on submit).
 */
function _coder_upgrade_upgrades($include_routines = FALSE) {
  return module_invoke_all('upgrades', $include_routines);
}

/**
 * Implement hook_upgrades().
 *
 * @param boolean $include_routines
 *   Whether to include the list of routines (only needed on submit).
 */
function coder_upgrade_upgrades($include_routines) {
  global $_coder_upgrades;

  // TODO We only have one file with routines - could elminate loop?
  if (!isset($_coder_upgrades)) {
    $_coder_upgrades = array();
    $path = drupal_get_path('module', 'coder_upgrade') . '/conversions';
    $files = drupal_system_listing('/coder_upgrade\..*\.inc$/', $path, 'filepath', 0);
    foreach ($files as $file) {
      require_once DRUPAL_ROOT . '/' . $file->filepath;
      $function = str_replace('.', '_', $file->name) . '_upgrades';
      if (drupal_function_exists($function)) {
        if ($upgrade = call_user_func($function, $include_routines)) {
          $_coder_upgrades = array_merge($_coder_upgrades, $upgrade);
        }
      }
    }
  }
  return $_coder_upgrades;
}

/**
 * Prepare conversion environment and start conversion loop.
 *
 * @param array $upgrades
 *   Array of conversion sets to apply.
 * @param array $extensions
 *   Array of file types to convert based on extension.
 * @param array $items
 *   Array of directories containing the files to convert.
 * @param boolean $recursive
 *   Indicates whether to recurse the subdirectories of each $item.
 * @return boolean
 *   Indicates whether conversion code was successfully applied.
 */
function coder_upgrade_start($upgrades, $extensions, $items, $recursive = TRUE) {
  return TRUE;
}
