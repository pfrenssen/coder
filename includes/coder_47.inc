<?php
// $Id$

/** @file
 * This include file implements coder functionality for 4.6 -> 4.7 upgrades
 *
 * Todo: The rules for this review have not yet been defined.
 */ 

function coder_47_reviews() {
  $reviews = array();
  $reviews['upgrade47'] = array(
    '#title' => t('Converting 4.6.x modules to 4.7.x'),
    '#link' => 'http://drupal.org/node/22218',
  );
  return $reviews;
}
