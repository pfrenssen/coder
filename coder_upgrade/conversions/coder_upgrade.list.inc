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
 * Callback from hook_upgrades().
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
 * Return a list of conversion routine suffixes.
 *
 * @return array
 *   Array of conversion routine suffixes.
 */
function coder_upgrade_conversions_list() {
  return array();
}
