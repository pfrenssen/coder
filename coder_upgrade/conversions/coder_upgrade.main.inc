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
 */
function _coder_upgrade_upgrades() {
  return module_invoke_all('upgrades');
}

/**
 * Implement hook_upgrades().
 */
function coder_upgrade_upgrades() {
  global $_coder_upgrades; // TODO Only referenced in this function -- remove global
  if (!isset($_coder_upgrades)) {
    $_coder_upgrades = array();
    $path = drupal_get_path('module', 'coder_upgrade') . '/conversions';
    $files = drupal_system_listing('/coder_upgrade\..*\.inc$/', $path, 'filepath', 0);
    foreach ($files as $file) {
      require_once DRUPAL_ROOT . '/' . $file->filepath;
      $function = str_replace('.', '_', $file->name) . '_upgrades';
      if (drupal_function_exists($function)) {
        if ($upgrade = call_user_func($function)) {
          $_coder_upgrades = array_merge($_coder_upgrades, $upgrade);
        }
      }
    }
  }
  return $_coder_upgrades;
}

/**
 * Prepare conversion environment before starting conversion loop.
 *
 * @param array $upgrades
 *   Array of conversions to apply.
 * @param array $extensions
 *   Array of file types to convert based on extension.
 * @param array $items
 *   Array of directories containing the files to convert.
 * @param boolean $recursive
 *   Indicates whether to recurse the subdirectories of each $item.
 */
function coder_upgrade_start($upgrades, $extensions, $items, $recursive = TRUE) {
  return TRUE;
}
