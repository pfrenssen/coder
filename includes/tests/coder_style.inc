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
  $var = new stdclassToo(); // should be camelcase rule
}

function coderCamelCase() {
  $camelCaseVar = 'whatever'; // camel case functions and vars not allowed
}

function coder_test_two_errors_on_same_line() {
  if('test=' . $test == 'test='){ // there are 3 errors on this line
  }
}

function coder_test_embedded_php() {
  ?>This is embedded php and should Not trigger a camelCase error.<?php
  ?>This second embedded php and should Not trigger
a camelCase error.<?php
}

function coder_hex_number() {
  $var = 0xFF; // should NOT be camelcase
}

// should generate an error about the trailing php close
?>
