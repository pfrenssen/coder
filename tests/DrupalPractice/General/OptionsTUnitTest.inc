<?php

$form['number_display'] = array(
  '#type' => 'select',
  '#title' => t('Display'),
  '#options' => array(
    '1' => '1',
    '2' => '2',
    '3' => '3',
    '4' => '4',
    '500' => '500',
    'a' => 'a',
    'abc' => 'abc',
    'four' => 'four',
  ),
);

$form['display']['show_thumbnail'] = array(
  '#title' => t('Show Thumbnail', array(), array('context' => 'test')),
  '#type' => 'radios',
  '#options' => array(
    '1' => t('Yes', array(), array('context' => 'test')),
    '0' => t('No', array(), array('context' => 'test')),
  ),
);

$form['display']['status'] = [
  '#title' => t('Status', array(), array('context' => 'test')),
  '#type' => 'select',
  '#options' => array(
    '1' => 'Enabled',
    '0' => t('Disabled', array(), array('context' => 'test')),
  ),
];

$title_display = 'before';
$form['amount'] = [
  '#type' => 'select',
  '#title' => t('Amount'),
  '#title_display' => $title_display,
  '#options' => [
    '1' => '1',
    '2' => '2',
    '3' => '3',
    '4' => '4',
    '500' => '500',
    'a' => 'a',
    'abc' => 'abc',
    'four' => 'four',
  ],
];

// Make sure this one does not trigger any warnings, as its type cannot be
// easily determined.
// This test array deliberately put in the middle of two arrays with '#type'
// specified to ensure that lookup is bounded by the array itself and does not
// leak to the beginning/end of the document.
$form['ipsum']['#options'] = [
  'attributes' => [
    'target' => '_blank',
  ],
];

$form['display']['hide_thumbnail'] = [
  '#title' => t('Hide Thumbnail', [], ['context' => 'test']),
  '#type' => 'radios',
  '#required' => $required,
  '#options' => [
    '1' => t('Yes', [], ['context' => 'test']),
    '0' => t('No', [], ['context' => 'test']),
  ],
];

$form['link'] = [
  '#type' => 'link',
  '#title' => t('Download'),
  '#url' => Url::fromUri('internal:/'),
  '#options' => [
    'attributes' => [
      'target' => '_blank',
      'class' => [
        'home',
      ],
    ],
  ],
];
