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
  $var = $obj->camelCase; // but camel case objects elements are
  new camcelCase(); // is ok
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

function coder_multiline_quote() { // from Drupal5 block.module
  return t('<p>Blocks are boxes of content that may be rendered into certain regions of your web pages, for example, into sidebars. They are usually generated automatically by modules, but administrators can create blocks manually.</p>
<p>Only enabled blocks are shown. You can position blocks by specifying which area of the page they should appear in (e.g., a sidebar).  Highlighted labels on this page show the regions into which blocks can be rendered. You can specify where within a region a block will appear by adjusting its weight.</p>
<p>If you want certain blocks to disable themselves temporarily during high server loads, check the "Throttle" box. You can configure the auto-throttle on the <a href="@throttle">throttle configuration page</a> after having enabled the throttle module.</p>
<p>You can configure the behaviour of each block (for example, specifying on which pages and for what users it will appear) by clicking the "configure" link for each block.</p>', array('@throttle' => url('admin/settings/throttle')));
}

function coder_break() {
  print '<br>'; // should generate an error, and will throw performance warning
  ?><br><?php // should also generate an error
}

function coder_heredoc() {
  $var = <<< __EOD__
<br><!-- a php error and not a camelCase error -->
<B><!-- an uppercase XHTML error -->
__EOD__;
  $var = <<<__EOD__
<br><!-- and again -->
__EOD__;
}

function coder_dot() {
  if ($file = file_check_upload($fieldname . '_upload')) { // not ok
  }
  $v .= 'bugger'; // ok
  $a = $v .'bugger'; // ok
  $a = $v ."bugger"; // ok, but will throw performance warning
  $a = $v.'bugger'; // not ok
  $a = $some_func().'bugger'; // not ok
}

function coder_array_indexes() {
  $a[hello] = 'this is bad';
  $a['hello'] = 'this is no';
}

// should generate an error about the trailing php close
?>
