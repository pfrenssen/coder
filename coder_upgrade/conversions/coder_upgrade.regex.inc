<?php
// $Id$

/**
 * @file
 * Miscellaneous conversion routine file for the coder_upgrade module.
 * These routines upgrade text using simple regular expressions.
 *
 * The functions in this conversion routine file correspond to topics in the
 * category roadmap at http://drupal.org/node/394070 that are marked with a
 * green check mark in the Upgrade column.
 *
 * Copyright 2008-10 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * The upgrades to these items are documented at the following urls.
 *
 * System
 * http://drupal.org/node/224333#moved_statistics_settings
 *
 *
 * Permissions and Access
 * http://drupal.org/node/224333#moved_statistics_settings (DUP System)
 * http://drupal.org/node/224333#php_permissions
 *
 *
 * Menu
 * http://drupal.org/node/224333#admin_path_changes
 *
 *
 * Blocks
 * http://drupal.org/node/224333#custom_block
 *
 *
 * Input Sanitization and Input Formats
 * http://drupal.org/node/224333#filter_urls
 *
 *
 * File API
 * http://drupal.org/node/224333#remove_FILE_STATUS_TEMPORARY
 *
 * @param string $file
 *   The text of the file to convert.
 */

/**
 * Implements hook_upgrade_regex_alter().
 *
 * Upgrades text using simple regular expressions.
 */
function coder_upgrade_upgrade_regex_alter(&$file) {
  cdp("inside " . __FUNCTION__);
  $hook = 'regex_simple_changes';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // Moved admin/reports/settings to admin/config/system/statistics (replacing admin/settings/statistics).
//  $from[] = "@'admin/reports/settings'@";
//  $to[] = "'admin/config/system/statistics'";

  // Moved admin/settings/filters/* to admin/settings/filter/*
//  $from[] = '@admin\/settings\/filters(\/.*?)@';
//  $to[] = "admin\/settings\/filter$1";

  coder_upgrade_upgrade_admin_path_changes($from, $to);
  // TODO Intend to do all of these when converting db_query calls.
  coder_upgrade_upgrade_database_tables($from, $to);

  // Replace "use PHP for block visibility" with "use PHP for settings."
  $from[] = '/(\'|")use PHP for block visibility(\'|")/';
  $to[] = "$1use PHP for settings$1";

  // FILE_STATUS_TEMPORARY has been removed.
  $from[] = "/FILE_STATUS_TEMPORARY/";
  $to[] = "~FILE_STATUS_PERMANENT";

  coder_upgrade_do_conversions($from, $to, $new);
  coder_upgrade_save_changes($cur, $new, $file, $hook);
}

/**
 * Upgrades admin paths.
 *
 * http://drupal.org/node/224333#admin_path_changes
 */
function coder_upgrade_upgrade_admin_path_changes(&$from, &$to) {
  $path_conversions = array(
    'admin/reports/settings' => 'admin/config/system/statistics',
    'admin/settings/filters' => 'admin/config/content/filter',
    'admin/build/modules' => 'admin/modules',
    'admin/build/themes' => 'admin/appearance',
    'admin/build/path' => 'admin/config/search/path',
    'admin/content/types' => 'admin/structure/types',
    'admin/content/taxonomy' => 'admin/structure/taxonomy',
    'admin/content/forum' => 'admin/structure/forum',
    'admin/build/testing' => 'admin/config/development/testing',
    'admin/settings/site-maintenance' => 'admin/config/development/maintenance',
    'admin/settings/performance' => 'admin/config/development/performance',
    'admin/settings/search' => 'admin/config/search/settings',
    'admin/settings/clean-urls' => 'admin/config/search/clean-urls',
    'admin/settings' => 'admin/config',
    'admin/build' => 'admin/structure',
  );

  foreach ($path_conversions as $old_path => $new_path) {
    $from[] = "@(['\"])$old_path(/?.*?)(['\"])@";
    $to[] = "$1$new_path$2$1";
  }
}

/**
 * Upgrades database tables.
 */
function coder_upgrade_upgrade_database_tables(&$from, &$to) {
  $table_conversion = array(
    'box' => 'custom_block',
  );
  foreach ($table_conversion as $old_table => $new_table) {
    $from[] = '/\{' . $old_table . '\}/i';
    $to[] = '{' . $new_table . '}';
  }
}
