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
      '#value' => '[ \t]+(if|elseif|while|foreach)\(',
      '#warning' => t('use spaces after control statements, before parenthesis'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\t]+',
      '#warning' => t('replace tabs with two spaces for formatting (or \\t for quoted characters)'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\){',
      '#warning' => t('use a space between the closing parenthesis and the open bracket'),
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
?>
