<?php
/**
 * @file
 * This include file implements coder functionality for 7.x -> 8.x upgrades.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_8x_reviews() {
  $rules[] = array(
  );

  $review = array(
    '#title' => t('Converting 7.x modules to 8.x'),
    '#link' => 'http://drupal.org/list-changes/drupal?to_branch=8.x',
    '#rules' => $rules,
    '#severity' => 'critical',
  );

  return array('upgrade8x' => $review);
}
