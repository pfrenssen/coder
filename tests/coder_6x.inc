<?php
// $Id$

function this_is_not_a_menu_hook() {
  $items = array();
  $items[] = array(); // this should not generate an error
  return $items;
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
}

function _coder_6x_test_link_alter($node, &$links) { // an error
}

function _coder_6x_test2_link_alter(&$links, $node) { // not an error
}

function _coder_6x_test_mail_alter(&$mailkey, &$to, &$subject, &$body, &$from, &$headers) { // this is an error
}

function _coder_6x_test2_mail_alter(&$message) { // not an error
}
