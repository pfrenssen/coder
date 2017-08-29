<?php

/**
 * @file
 * Drupal.Semantics.RemoteAddress sniff tests.
 */

// Pass - Drupal.Semantics.RemoteAddress.RemoteAddress.
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';

// Error - Drupal.Semantics.RemoteAddress.RemoteAddress.
$remote_address = $_SERVER['REMOTE_ADDR'];

function dummy() {
  // Pass - Drupal.Semantics.RemoteAddress.RemoteAddress.
  $_SERVER['REMOTE_ADDR'] = '127.0.0.1';

  // Error - Drupal.Semantics.RemoteAddress.RemoteAddress.
  $remote_address = $_SERVER['REMOTE_ADDR'];
}
