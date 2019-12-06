<?php
/**
 * @file
 * Tests array declarations.
 * phpcs:set Drupal.Arrays.Array lineLimit 100
 */

$array = array(
  'data' => 'my-data',
  'animal' => 'squirrel',
  'inline' => array(),
  'inline1' => array('thisisaverylongstring', 'thisisaverylongstring'),
  'inline2' => array('thisisaverylongstring', 'thisisaverylongstring', 'thisisaverylongstring'),
  'inline3' => array('thisisaverylongstring', 'thisisaverylongstring', 'thisisaverylongstring', 'thisisaverylongstring'),
  'inline4' => array('thisisaverylongstringwithallotoftext', 'thisisaverylongstringwithallotoftext'),
  'inline_long_ok' => array('one', 'two', 'three', 'four', 'five', 'six', 'seven'),
  'inline_long_nok' => array('one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten'),
);
