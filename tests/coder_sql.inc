<?php
// $Id$

/** @file
 * This include file implements tests for the Drupal SQL
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder
 */

function coder_test_sql() {
  $sql = 'select * from {node}'; // lower case
  $sql = 'insert into {node} (changed) VALUES (%d)'; // lower case
  $sql = 'delete from {node}'; // lower case
  $sql = 'update {node} set changed = now()'; // lower case
  $var = t('select something from this');
  $var = t('update something');
  $var = t('insert something');
  $var = t('delete something');
  $sql = 'INSERT INTO node (changed) VALUES (1)';
  $sql = 'SELECT * FROM {node} LIMIT 10';
  $sql = "SELECT * FROM {node} WHERE nid=$nid"; // unsecure  
}
