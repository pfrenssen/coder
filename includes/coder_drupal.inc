<?php
// $Id$

/** @file
 * This include file implements coder functionality for Drupal Standards
 *
 * Todo: The rules for this review are not yet complete.
 */

function coder_drupal_reviews() {
  $rules = array(
/*
    array(
      '#type' => 'regex',
      '#value' => '\t',
      '#warning' => t('Use an indent of 2 spaces, with no tabs'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s(if|elseif|while|foreach|switch|return|for)\(',
      '#warning' => t('Control statements should have one space between the control keyword and opening parenthesis'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s\w(?!(if|elseif|while|foreach|switch|return|for))\s\(',
      '#warning' => t('Functions should be called with no spaces between the function name'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\){',
      '#warning' => t('use a space between the closing parenthesis and the open bracket'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '(\S=>|=>\S)',
      '#warning' => t('Arrays should be formatted with a space separating each element and assignment operator'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '<\?(?!php)',
      '#warning' => t('Always use &lt;?php ?&gt; to delimit PHP code, not the &lt;? ?&gt; shorthand'),
    ),
    array(
      '#type' => 'grep',
      '#original' => 1,
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
    array(
      '#type' => 'regex',
      '#value' => '}\s*else',
      '#warning' => t('else statements should begin on a new line'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '[,][^ \n\r]',
      '#warning' => t('missing space after comma'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '^\s*{',
      '#warning' => t('curly braces { should end a line, not start one'),
    ),
*/
    array(
      '#type' => 'regex',
      '#value' => '[a-z][A-Z]',
      '#case' => 1,
      '#warning' => t('do not use mixed case (camelCase), use lower case and _'),
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

function _coder_drupal_callback($orig_lines, $lines, &$results, &$tmp) {
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
