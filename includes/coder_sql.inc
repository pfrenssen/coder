<?php
// $Id$

/** @file
 * This include file implements coder functionality for SQL strings
 */

function coder_sql_reviews() {
  $table = '\{[A-Za-z_]+\}'; // table-regex
  $bad = '[A-Za-z_]+';
  $rules = array(
    // NOTE: this doesn't catch all non-upper case keywords, but is a good start
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+'. $table .'|insert\s+into\s+'. $table .'|update\s+'. $table .'\s+set|delete\s+from\s+'. $table .')',
      '#source' => 'quote',
      '#warning' => 'SQL keywords should be upper case',
      '#case-sensitive' => TRUE,
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+'. $bad .'|insert\s+into\s+'. $bad .'|update\s+'. $bad .'\s+set|delete\s+from\s'. $bad .')\s+',
      '#source' => 'quote',
      '#warning' => 'table names should be enclosed in {curly_brackets}',
      '#severity' => 'critical',
    ),
  );
  $review = array(
    '#title' => t('Drupal SQL Standards'),
    '#rules' => $rules,
    '#description' => t('new review, so use with caution'),
  );
  return array('sql' => $review);
}

