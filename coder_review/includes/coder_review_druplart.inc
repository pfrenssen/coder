<?php
/**
 * @file
 * This include file implements coder functionality for style I prefer, Drupl'Art.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_druplart_reviews() {
  $rules[] = array(
    '#type' => 'regex',
    '#value' => 'if \(.*\s+=\s+.*\)',
    '#warning' => 'Consider putting the assignment on a line above the conditional.',
    '#severity' => 'minor',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => 'global\s+\$user;',
    '#warning' => 'Use $GLOBALS[\'user\'] instead of global $user, because the latter is prone to accidentally overwriting the global user object.',
    '#severity' => 'minor',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#function' => '_(submit|validate)$',
    '#value' => '\s+return\s+(TRUE|FALSE)',
    '#warning' => 'Submit and validate handlers return values are ignored.',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#function' => '_menu$',
    '#value' => '\s+\$items\s+=\s+array\(\)',
    '#warning' => 'This array initialization is unnecessary if the array is assigned in all code paths.',
    '#severity' => 'minor',
  );

  $review = array(
    '#title' => "Drupl'Art Coding Style",
    '#link' => 'http://drupalwatchdog.com/2/1/druplart',
    '#rules' => $rules,
    '#description' => 'Checks for coding style suggested by the author.',
    '#version' => 2,
  );
print_r($review);
  return array('druplart' => $review);
}
