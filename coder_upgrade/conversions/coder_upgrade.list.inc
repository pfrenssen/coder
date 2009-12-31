<?php
// $Id$

/**
 * @file
 * List of conversion routines for the coder_upgrade module.
 *
 * The functions in these conversion routine files correspond to the topics in
 * the category roadmap at http://drupal.org/node/394070 that are marked with
 * a "Yes" in the Upgrade column.
 *
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Returns the list of upgrade sets from all modules.
 *
 * @param boolean $include_routines
 *   Indicates whether to include the list of conversion routines. This list
 *   is only needed when the conversions are to be applied (typically on form
 *   submission). This parameter equals FALSE when this routine is invoked by
 *   Coder Upgrade while building the form, and TRUE when applying the
 *   conversions.
 */
function _coder_upgrade_upgrades($include_routines = FALSE) {
  return module_invoke_all('upgrades', $include_routines);
}

/**
 * Implements hook_upgrades().
 *
 * @param boolean $include_routines
 *   Indicates whether to include the list of conversion routines. This list
 *   is only needed when the conversions are to be applied (typically on form
 *   submission). This parameter equals FALSE when this hook is invoked by
 *   Coder Upgrade while building the form, and TRUE when applying the
 *   conversions.
 */
function coder_upgrade_upgrades($include_routines) {
  $_coder_upgrades = drupal_static(__FUNCTION__);

  if (!isset($_coder_upgrades)) {
    $_coder_upgrades = array();
    $path = drupal_get_path('module', 'coder_upgrade') . '/conversions';
    $files = drupal_system_listing('/coder_upgrade\..*\.inc$/', $path, 'filepath', 0);
    foreach ($files as $file) {
//      require_once DRUPAL_ROOT . '/' . $file->filepath;
      $function = str_replace('.', '_', $file->name) . '_upgrades';
      if (function_exists($function)) {
        if ($upgrade = call_user_func($function, $include_routines)) {
          $_coder_upgrades = array_merge($_coder_upgrades, $upgrade);
        }
      }
    }
  }
  return $_coder_upgrades;
}

/**
 * Returns a list of upgrade sets provided by this module.
 *
 * If requested, each set includes its associated list of upgrade routines.
 * This is a callback from hook_upgrades().
 *
 * @param boolean $include_routines
 *   Indicates whether to include the list of conversion routines.
 */
function coder_upgrade_list_upgrades($include_routines) {
  $routines = $include_routines ? coder_upgrade_conversions_list() : array();
  $upgrade = array(
    'title' => t('Core API changes from 6.x to 7.x'),
    'link' => 'http://drupal.org/node/394066',
    'routines' => $routines,
  );
  return array('coder_upgrade' => $upgrade);
}

/**
 * Returns a list of conversion routine suffixes.
 *
 * Group changes by processing order (beginning, middle, or end). The beginning
 * and end conversion routines are applied only at the module or directory
 * level and the routine will need to determine what to do and how (regex or
 * parser) to do it.
 *
 * The middle changes are applied at the file level and the list entry must
 * specify the handler (regex or parser). The regex changes are applied before
 * the parser changes.
 *
 * @return array
 *   Array of conversion routine suffixes.
 */
function coder_upgrade_conversions_list() {
  $list = array(
    'beginning' => array(),
    'middle' => array(),
    'end' => array(),
  );

  $list['middle'] = array(
    array(
      'name' => 'function_calls',
      'type' => 'parser',
    ),
    array(
      'name' => 'functions',
      'type' => 'parser',
    ),
    array(
      'name' => 'regex_simple_changes',
      'type' => 'regex',
    ),
  );

  $list['end'] = array('end');

  return $list;
}
