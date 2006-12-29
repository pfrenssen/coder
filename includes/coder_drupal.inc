<?php
// $Id$

/** @file
 * This include file implements coder functionality for Drupal Standards
 *
 * Todo: The rules for this review are not yet complete.
 */

function coder_drupal_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => '\t',
      '#warning' => t('Use an indent of 2 spaces, with no tabs'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s(if|elseif|while|foreach|for)\(',
      '#warning' => t('Control statements should have one space between the control keyword and opening parenthesis'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s\w(?!(if|elseif|while|foreach|for))\s\(',
      '#warning' => t('Functions should be called with no spaces between the function name'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\){', // #coder_ignore
      '#warning' => t('use a space between the closing parenthesis and the open bracket'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '(\S=>|=>\S)', // #coder_ignore
      '#warning' => t('Arrays should be formatted with a space separating each element and assignment operator'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '<\?(?!php)',
      '#warning' => t('Always use &lt;?php ?&gt; to delimit PHP code, not the &lt;? ?&gt; shorthand'), // #coder_ignore
    ),
    array(
      '#type' => 'grep',
      '#value' => '\$Id',
      '#warning' => t('Include the Id CVS keyword in each file'),
    ),
    array( // TODO: don't catch $user and other 'core' globals
      '#type' => 'regex',
      '#value' => 'global\s+\$[^_]',
      '#warning' => t('global variables should start with a single underscore followed by the module and another underscore'),
    ),
    array(
      '#type' => 'callback',
      '#value' => _coder_drupal_callback,
    ),
  );

  $reviews = array();
  $reviews['drupal'] = array(
    '#title' => t('Drupal Coding Standards'),
    '#link' => 'http://drupal.org/node/318',
    '#rules' => $rules,
  );
  return $reviews;
}

function _coder_drupal_callback($lines, &$results, &$tmp) {
  for ($lineno = -1; $last = array_slice($lines, $lineno); $lineno --) {
    $lastline = $last[0];
    if (preg_match('/\S/', $lastline)) {
      break;
    }
  }
  if ($last && $lastline && preg_match('/\?>/i', $lastline)) {
    $lineno = count($lines);
    $results[$lineno] = theme('coder_warning', $lineno, t('the final ?> should be omitted from all code files'));
  }
}
