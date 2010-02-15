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
 * Input Sanitization and Input Formats
 * http://drupal.org/node/224333#filter_urls
 *
 * @param string $file
 *   The text of the file to convert.
 */

/**
 * Upgrades text using simple regular expressions.
 */
function coder_upgrade_convert_regex_simple_changes(&$file) {
  cdp("inside " . __FUNCTION__);
  $hook = 'regex_simple_changes';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // Moved admin/reports/settings to admin/config/system/statistics (replacing admin/settings/statistics).
  $from[] = "@'admin/reports/settings'@";
  $to[] = "'admin/config/system/statistics'";

  // Replace "use PHP for block visibility" with "use PHP for settings."
  $from[] = '/(\'|")use PHP for block visibility(\'|")/';
  $to[] = "$1use PHP for settings$1";

  // Moved admin/settings/filters/* to admin/settings/filter/*
  $from[] = '@admin\/settings\/filters(\/.*?)@';
  $to[] = "admin\/settings\/filter$1";

  coder_upgrade_do_conversions($from, $to, $new);
  coder_upgrade_save_changes($cur, $new, $file, $hook);
}
