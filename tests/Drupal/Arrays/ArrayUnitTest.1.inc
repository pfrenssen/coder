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
  'inline_two_elements_ok' => array('one-two-three', 'the-2nd-element-is-within-the-limit'),
  'inline_two_elements_ok2' => array('one-two-three-four', 'the-2nd-element-is-right-on-the-limit'),
  'inline_two_elements_not_ok' => array('one-two-three-four-five', 'the-2nd-element-extends-beyond-the-limit'),
  'inline_two_elements_ok3' => func(['one-two-three-four', 'five'], 'other text which goes past the limit'),
  'inline_two_elements_ok4' => func(['one-two-three-four', 'this-2nd-element-is-right-on-the-limit'], 'other text'),
  'inline_two_elements_ok5' => func(['one-two-three-four'], ['second_array' => 'this-is-ok'], 'other text'),
  'inline_two_elements_not_ok' => func(['one-two'], ['second_array' => 'three', 'four-five' => 'six'], 'other text'),
);
