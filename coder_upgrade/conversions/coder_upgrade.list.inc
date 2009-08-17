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
      'name' => 'simple_changes',
      'type' => 'parser',
    ),
  );

  $list['end'] = array('end');

  return $list;
}
