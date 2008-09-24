<?php
// $Id$

/**
 * @file
 * This file implements tests for the Drupal 6.x upgrade.
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder
 */

function this_is_not_a_menu_hook() {
  $items = array();
  $items[] = array(); // This should not generate an error.
  return $items;
}

function _coder_6x_l_tests() {
  $link = l(t($display_vars['back_to_top']), 'faq', array('fragment' => '')); // this is ok
  $link = l(t($display_vars['back_to_top']), 'faq', NULL, NULL); // this is not ok
  $output = l($image, $item['filepath'], $link_attributes, NULL, NULL, FALSE, TRUE); // Not ok.
}

function _coder_6x_more_tests() {


  $node = node_load(arg(1)); // Not ok.

}

function _coder_6x_test_link_alter($node, &$links) { // An error.
}

function _coder_6x_test2_link_alter(&$links, $node) { // Not an error.
}

function _coder_6x_test_mail_alter(&$mailkey, &$to, &$subject, &$body, &$from, &$headers) { // An error.
}

function _coder_6x_test2_mail_alter(&$message) { // An error.
}

function coder_node_get_name() {
  node_get_name('name');
}

function myform_validate($form_id, $form) { // This will fail.
}

function myform_prerender($form_id, $form) { // This will fail.
}

function myform_submit($form_id, $form) { // This will fail.
  $form['#submit']['newsubmit'] = array();
  $form['#submit'][] = "newsubmit";
  return $foo; // Not ok.
  return 'my/path'; // Not ok.
}

function mymodule_info($field = 0) {
}

function mymodule_auth($username, $password, $server) {
}

function mymodule_help($section) { // Not ok.
}

function mymodule_help($section, $arg) { // Ok.
}

function mymodule_enable() {
  node_access_rebuild(); // Not ok in node access modules.
}

function mymodule_disable() {
  node_access_rebuild(); // Not ok in node access modules.
}

function mymodule_access($op, $node) { // Not ok.
}

function coder_6x_tests_menu() {
  $items = array();

  $items['test_good'] = array(
    'title' => 'My title', // Ok.
    'description' => 'My description', // Ok.
  );

  $items['test_bad'] = array(
    'title' => t('My title'), // Not Ok.
    'description' => t('My description'), // Not Ok.
  );

}
