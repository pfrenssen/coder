<?php
// $Id$ 

/** @file
 * This include file implements coder functionality for comments
 */

function coder_comment_reviews() {
  $rules = array(
    array(
      '#type' => 'grep_invert',
      '#source' => 'comment',
      '#value' => '$Id',
      '#case-sensitive' => true,
      '#warning' => 'Include the CVS keyword $Id'.'$ in each file',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^\*',
      '#warning' => 'indent secondary line of comment one space ',
      '#severity' => 'minor',
    )
  );
  $review = array(
    '#title' => t('Drupal Commenting Standards'),
    '#link' => 'http://drupal.org/node/318',
    '#rules' => $rules,
    '#description' => t('every developer should use'),
  );
  return array('comment' => $review);
}
