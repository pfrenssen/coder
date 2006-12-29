<?php
// $Id$

/** @file
 * This include file implements coder functionality for Drupal Standards
 *
 * Todo: The rules for this review are not yet complete.
 */

function coder_security_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => 'l\(check_plain\(',
      '#warning' => t('l() already contains a check_plain() call by default'),
    ),
    array(
      '#type' => 'callback',
      '#value' => _coder_security_callback,
    ),
  );

  $reviews = array();
  $reviews['security'] = array(
    '#title' => t('Handle text in a secure fashion'),
    '#link' => 'http://drupal.org/node/28984',
    '#rules' => $rules,
  );
  return $reviews;
}

function _coder_security_callback($orig_lines, $lines, &$results, &$tmp) {
  if (!isset($tmp['tokens'])) {
    $source = implode('', $lines);
    $tmp['tokens'] = token_get_all($source);
  }

  // TODO: parse the file looking for missing stuff
}
