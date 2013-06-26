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
