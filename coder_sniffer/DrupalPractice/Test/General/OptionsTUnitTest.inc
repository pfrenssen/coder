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
