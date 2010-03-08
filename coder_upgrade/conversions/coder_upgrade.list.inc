<?php
// $Id$

/**
 * @file
 * List of conversion routines for the coder_upgrade module.
 *
 * The functions in these conversion routine files correspond to the topics in
 * the category roadmap at http://drupal.org/node/394070 that are marked with
 * a green check mark in the Upgrade column.
 *
 * Copyright 2009-10 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Returns the list of upgrade sets from all modules.
 */
function _coder_upgrade_upgrades() {
  $upgrades = &drupal_static(__FUNCTION__);

  if (is_null($upgrades)) {
    $upgrades = module_invoke_all('upgrade_info');
  }
  return $upgrades;
}

/**
 * Implements hook_upgrade_info().
 */
function coder_upgrade_upgrade_info() {
  $upgrade = array(
    'title' => t('Core API changes from 6.x to 7.x (parser routines)'),
    'link' => 'http://drupal.org/node/394070',
    'files' => array(
      'conversions/coder_upgrade.begin.inc',
      'conversions/coder_upgrade.call.inc',
      'conversions/coder_upgrade.db.inc',
      'conversions/coder_upgrade.end.inc',
      'conversions/coder_upgrade.function.inc',
      'conversions/coder_upgrade.install.inc',
      'conversions/coder_upgrade.regex.inc',
    ),
  );
  return array('coder_upgrade' => $upgrade);
}
