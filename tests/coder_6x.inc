<?php
// $Id$

/**
 * @file
 * This file implements tests for the Drupal 6.x upgrade.
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder
 */

function _coder_6x_more_tests() {


  $node = node_load(arg(1)); // Not ok.

}
