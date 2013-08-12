<?php

/**
 * @file
 * This file should throw no warnings/errors.
 */

$form['vocab_fieldset']['entity_view_modes'] = array(
  '#type'     => 'select',
  '#prefix'   => '<div id="entity_view_modes_div">',
  '#suffix'   => '</div>',
  '#title'    => t('View Mode'),
  '#required' => 1,
  '#options'  => array(),
  '#ajax' => array(
    'callback' => 'custom_listing_pages_entity_vocabulary_listing',
    'wrapper'  => 'vocab_fieldset_div',
    'method'   => 'replace',
    'effect'   => 'fade',
  ),
);
print_r($form);

/**
 * Ignoring the array value in a foreach loop is OK.
 */
function test1() {
  $array = array(1, 2);
  foreach ($array as $key => $value) {
    print $key;
  }

  try {
    print 'foo';
  }
  catch (Exception $e) {
    // $e is unused here, which is fine.
    print 'error';
  }

  // Initializing an array on the fly is allowed.
  $items['foo'] = 'bar';
  return $items;
}
