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
      '#warning_callback' => '_coder_security_l_check_plain_warning',
    ),
/*  array(
      '#type' => 'callback',
      '#value' => _coder_security_callback,
    ), */
    array(
      '#type' => 'regex',
      '#value' => '(?-i)\$REQUEST_URI',
      '#warning_callback' => '_coder_security_request_uri_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '(?-i)\"REQUEST_URI\"|\'REQUEST_URI\'',
      '#warning_callback' => '_coder_security_request_uri_warning',
    ),
  );
  $review = array(
    '#title' => 'Handle text in a secure fashion',
    '#link' => 'http://drupal.org/node/28984',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('security' => $review);
}


/**
 * Define the warning callbacks
 */

/* function _coder_security_callback(&$coder_args, $review, $rule, $lines, &$results) {
  if (!isset($coder_args['#tokens'])) {
    $source = implode('', $lines);
    $coder_args['#tokens'] = token_get_all($source);
  }
} */

/**
 * Define the warning callbacks
 */

function _coder_security_l_check_plain_warning() {
  return t('!l() already contains a !check_plain() call by default',
    array(
      '!l' => theme('drupalapi', 'l'),
      '!check_plain' => theme('drupalapi', 'check_plain'),
    )
  );
}

function _coder_security_request_uri_warning() {
  return t('the use of REQUEST_URI is prone to XSS exploits and does not work on IIS; use !request_uri() instead',
    array(
      '!request_uri' => theme('drupalapi', 'request_uri'),
    )
  );
}
