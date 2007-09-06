<?php
// $Id$

function this_is_not_a_menu_hook() {
  $items = array();
  $items[] = array(); // this should not generate an error
  return $items;
}

function _coder_6x_l_tests() {
  $link = l(t($display_vars['back_to_top']), 'faq', array('fragment' => '')); // this is ok
  $link = l(t($display_vars['back_to_top']), 'faq', NULL, NULL); // this is not ok
  $output = l($image, $item['filepath'], $link_attributes, NULL, NULL, FALSE, TRUE); // not ok
}

function _coder_6x_more_tests() {
  if (taxonomy_node_get_terms($nid)) { // should generate an error
  }
  if (taxonomy_node_get_terms($node->nid)) { // should generate an error
  }
  if (taxonomy_node_get_terms($node)) { // not an error
  }
  $output = strtr(format_plural($num, 'There is currently 1 %type post on your site.', 'There are currently @count %type posts on your site.'), array('%type' => theme('placeholder', $type))); // a valid error
  $output = notstrtr(format_plural($num, 'There is currently 1 %type post on your site.', 'There are currently @count %type posts on your site.'), array('%type' => theme('placeholder', $type))); // not an error
  watchdog('user', t('Removed %username user.', array('%username' => $user->name))); // an error
  watchdog('user', 'Removed %username user.', array('%username' => $user->name)); // not an error
  $complex = unserialize(cache_get('complex_cid')); // an error
  cache_set('simple_cid', 'cache', $simple); // an error
  $ip = $_SERVER['REMOTE_ADDR']; // an error

  if ($file = file_check_upload('picture_upload')) { // an error
    $file = file_save_upload('picture_upload', $destination, FILE_EXISTS_REPLACE); // an error
  }

  if ($file = file_save_upload('picture_upload', $validators)) { // ok
  }

  $sql = 'UPDATE {file_revisions} SET vid=1'; // an error
  if (db_query('SELECT * FROM {file_revisions}')) { // an error
  } 

  // add FAPI tests
  $form = array(
    '#base' => 'my_shared_form_id', // error
    '#pre_render' => '<div>', // error
  );
  $form['#base'] = 'http://example.com';
  $form['#submit']['my_submit_function'] = array($param1, $param2); // error
  $form['#submit']['my_validate_function'] = array(); // error
  $form['#multistep'] = TRUE;

  drupal_retrieve_form('testform'); // missing second arg
  drupal_retrieve_form($form_id, $form_state); // ok

  form_set_value($element, 'value', $form_status); // This one is okay
  form_set_value($element, 'value'); // error

  confirm_form($form, t('Do you really want to delete this?'), "node/$nid", t("Don't do it unless you're really sure!"), t('Delete it'), t('Go back'), 'delete'); // should fail

  confirm_form($form, t('Do you really want to delete this?'), "node/$nid", $options); // is ok
  confirm_form($form, t('Do you really want to delete this?'), "node/$nid", array()); // also ok

  custom_url_rewrite($url); // not ok
  custom_url_rewrite_inbound($url); // ok
}

function _coder_6x_test_link_alter($node, &$links) { // an error
}

function _coder_6x_test2_link_alter(&$links, $node) { // not an error
}

function _coder_6x_test_mail_alter(&$mailkey, &$to, &$subject, &$body, &$from, &$headers) { // this is an error
}

function _coder_6x_test2_mail_alter(&$message) { // not an error
}

function coder_node_get_name() {
  node_get_name('name');
}

function myform_validate($form_id, $form) { // this will fail.
}

function myform_submit($form_id, $form) { //this willfail
  $form['#submit']['newsubmit'] = array();
  $form['#submit'][] = "newsubmit";
}

function mymodule_info($field = 0) {
}

function mymodule_auth($username, $password, $server) {
}

function mymodule_help($section) {
}

function mymodule_help($section, $arg) { // ok
}
