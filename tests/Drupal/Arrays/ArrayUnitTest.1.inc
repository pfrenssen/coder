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
  'inline3_not_ok' => array('thisisaverylongstring', 'thisisaverylongstring', 'verylongstring', 'the array ends at 120'),
  'inline4' => array('thisisaverylongstringwithallotoftext', 'thisisaverylongstringwithallotoftext'),
  'inline_long_ok' => array('one', 'two', 'three', 'four', 'five', 'six', 'seven'),
  'inline_long_not_ok' => array('one', 'two', 'three', 'four', 'five', 'six', 'seven', 'array ends at 107'),
  'inline_two_elements_ok' => array('one-two-three', 'the-2nd-element-is-within-the-limit'),
  'inline_two_elements_ok2' => array('one-two-three-four', 'the-2nd-element-is-right-on-the-limit'),
  'inline_two_elements_not_ok' => array('one-two-three-four-five', '2nd-element-goes-beyond-the-limit-to-110'),
  'inline_two_elements_ok3' => func(['one-two-three-four', 'five'], 'other text which goes past the limit'),
  'inline_two_elements_ok4' => func(['one-two-three-four', 'this-2nd-element-is-right-on-the-limit'], 'other text'),
  'inline_two_elements_ok5' => func(['one-two-three-four'], ['second_array' => 'ends at 92'], 'func ends at 113'),
  'inline_two_elements_not_ok' => func(['one-two'], ['second_array' => 'stops', 'at' => 'column 101'], 'other text'),
  'inline_with_nested_functions_but_array_has_one_element_ok' => t('Tags: @tags', ['@tags' => implode(', ', $tags)]),
  'inline_one_element_ok' => 'Extends beyond the limit but ok as there is only one element. The array ends at 115'),
);
