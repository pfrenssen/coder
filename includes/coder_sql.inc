<?php

/**
 * @file
 * This include file implements coder functionality for SQL strings
 */

/**
 * Implementation of hook_reviews().
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
      '#severity' => 'minor'
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+'. $bad .'|insert\s+into\s+'. $bad .'|update\s+'. $bad .'\s+set|delete\s+from\s'. $bad .')',
      '#source' => 'quote',
      '#warning' => 'table names should be enclosed in {curly_brackets}',
      '#severity' => 'critical',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+'. $table .'|insert\s+into\s+'. $table .'|update\s+'. $table .'\s+set|delete\s+from\s'. $table .')\s+.*[Ll][Ii][Mm][Ii][Tt]\s[0-9]+',
      '#source' => 'quote',
      '#warning_callback' => '_coder_sql_db_query_range_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+'. $table .'|update\s+'. $table .'\s+set|delete\s+from\s'. $table .')\s+.*!=',
      '#source' => 'quote',
      '#warning' => 'Use ANSI standard <> instead of !=',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+'. $table .'\s+.+?=\s*`|insert\s+into\s+'. $table .'\s+.*?VALUES\s*(\(\s*`|\(.*?,\s*`)|update\s+'. $table .'\s+set\s+.*?=\s*`|delete\s+from\s'. $table .'\s+.*?=\s*`)',
      '#source' => 'quote',
      '#warning' => "Don't use back ticks to quote values as it is not compliant with ANSI SQL"
    ),
    array(
      '#type' => 'regex',
      '#filename' => array('install'),
      '#function' => '_update_[0-9]+$',
      '#source' => 'allphp',
      '#value' => 'db_query\s*\(\s*[\'"](UPDATE|INSERT|DELETE)\s+',
      '#warning_callback' => '_coder_sql_db_query_in_install',
    ),
  );
  $review = array(
    '#title' => t('Drupal SQL Standards'),
    '#rules' => $rules,
    '#description' => t('new review, so use with caution'),
  );
  return array('sql' => $review);
}

/**
 * Define the warning callbacks
 */

function _coder_sql_db_query_range_warning() {
  return array(
    '#warning' => t('Use !db_query_range() instead of the SQL LIMIT clause',
      array(
        '!db_query_range' => theme('drupalapi', 'db_query_range'),
      )
    ),
    '#link' => 'http://drupal.org/node/1395',
  );
}

function _coder_sql_db_query_in_install() {
  return array(
    '#warning' => t('Use !update_sql() instead of !db_query() in !hook_update_N()',
      array(
        '!update_sql' => theme('drupalapi', 'update_sql'),
        '!db_query' => theme('drupalapi', 'db_query'),
        '!hook_update_N' => theme('drupalapi', 'hook_update_N'),
      )
    ),
  );
}
