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
    '#value' => 'if \(.* = .*\)',
    '#warning' => 'Consider putting the assignment on a line above the conditional.',
    '#severity' => 'minor',
  );

  $review = array(
    '#title' => "Drupl'Art Coding Style",
    '#link' => 'http://drupalwatchdog.com/2/1/druplart',
    '#rules' => $rules,
    '#description' => 'Checks for coding style suggested by the author.',
  );
  return array('druplart' => $review);
}
