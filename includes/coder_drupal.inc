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
      '#warning' => 'Use an indent of 2 spaces, with no tabs',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s(if|elseif|while|foreach|switch|return|for)\(',
      '#warning' => 'Control statements should have one space between the control keyword and opening parenthesis',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s\w(?!(if|elseif|while|foreach|switch|return|for))\s\(',
      '#warning' => 'Functions should be called with no spaces between the function name',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\){',
      '#warning' => 'use a space between the closing parenthesis and the open bracket',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(\S=>|=>\S)',
      '#warning' => 'Arrays should be formatted with a space separating each element and assignment operator',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(\.\s\'\'|\'\'\s\.|\.\s""|\.\s"")',
      '#warning' => 'string concatenation should be formatted with a space separating the operators (dot .) and terms (\'string\')',
    ),
    array(
      '#type' => 'regex',
      '#value' => '<\?(?!php)',
      '#warning' => 'Always use &lt;?php ?&gt; to delimit PHP code, not the &lt;? ?&gt; shorthand',
    ),
    array(
      '#type' => 'grep',
      '#original' => 1,
      '#value' => '\$Id',
      '#warning' => 'Include the Id CVS keyword in each file',
    ),
    array( // TODO: don't catch $user and other 'core' globals
      '#type' => 'regex',
      '#value' => 'global\s+\$[^_]',
      '#warning' => 'global variables should start with a single underscore followed by the module and another underscore',
    ),
    array(
      '#type' => 'callback',
      '#value' => _coder_drupal_callback,
    ),
    array(
      '#type' => 'regex',
      '#value' => '}\s*else',
      '#warning' => 'else statements should begin on a new line',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[,][^ \n\r]',
      '#warning' => 'missing space after comma',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^\s*{',
      '#warning' => 'curly braces { should end a line, not start one',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[a-z][A-Z]',
      '#value' => '(?-i)[a-z][A-Z]',
      '#warning' => 'do not use mixed case (camelCase), use lower case and _',
    ),
  );
  $review = array(
    '#title' => t('Drupal Coding Standards'),
    '#link' => 'http://drupal.org/node/318',
    '#rules' => $rules,
  );
  return array('drupal' => $review);
}

/**
 * Define the rule callbacks
 */

function _coder_drupal_callback(&$coder_args, $rule, $lines, &$results) {
  for ($lineno = -1; $last = array_slice($lines, $lineno); $lineno --) {
    $lastline = $last[0];
    if (preg_match('/\S/', $lastline)) {
      break;
    }
  }
  if ($last && $lastline && preg_match('/\?>/i', $lastline)) {
    _coder_error($results, 'the final ?> should be omitted from all code files', count($lines));
  }
}
