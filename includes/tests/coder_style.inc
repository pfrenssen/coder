<?php
// $Id$

/** @file
 * This include file implements tests for the Drupal Standards
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder
 */

function coder_test_tab() {
  // tab in	comment - is this ok?
  $var = 'tab in	quote'; // is this ok?
	$var = 'tab error';
}

function coder_test_stdclass() {
  $var = new stdClass(); // this is ok
  $var = new StdClass(); // this is not
  $var = new stdclassToo(); // should be camelCase rule
}

function coderCamelCase() {
  $camelCaseVar = 'whatever';
}
