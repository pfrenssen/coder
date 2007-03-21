<?php
// $Id$

/** @file
 * This include file implements coder functionality for Performance
 *
 * Todo: The rules for this review are not yet complete.
 */

function coder_performance_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => 'TRUE',
      '#warning_callback' => '_coder_performance_true_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^\s*\$\w+\+\+\s*;',
      '#warning_callback' => '_coder_performance_increment_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\(strlen\(\$\w+\) < \d+\)',
      '#warning_callback' => '_coder_performance_strlen_less_than_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\(strlen\(\$\w+\) > \d+\)',
      '#warning_callback' => '_coder_performance_strlen_greater_than_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^\s*print ',
      '#warning_callback' => '_coder_performance_print_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'in_array\(\'\', \$\w+\)',
      '#warning_callback' => '_coder_performance_in_array_warning',
    ),
  );
  $review = array(
    '#title' => 'Performance Optimization',
    '#link' => 'http://drupal.org/node/110128',
    '#rules' => $rules,
    '#severity' => 'minor',
  );
  return array('performance' => $review);
}

function _coder_performance_true_warning() {
  return t('true is faster than TRUE');
}

function _coder_performance_increment_warning() {
  return t('++ $i is faster than $ i++');
}

function _coder_performance_strlen_less_than_warning() {
  return t('if (!isset($foo{5})) is faster than if (strlen($foo) < 5)');
}

function _coder_performance_strlen_greater_than_warning() {
  return t('if (isset($foo{6})) is faster than if (strlen($foo) > 5)');
}

function _coder_performance_print_warning() {
  return t('echo is faster than print (when the return value from print is not used)');
}

function _coder_performance_in_array_warning() {
  return t('if (isset($keys[\'foo\'])) is faster than if (in_array(\'foo\', $keys))');
}
