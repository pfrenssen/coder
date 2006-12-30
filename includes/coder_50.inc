<?php
// $Id$

/** @file
 * This include file implements coder functionality for 4.7 -> 5.0 upgrades
 * 
 * Todo: The rules for this review have not yet been defined.
 */

function coder_50_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => 'user_mail\s*\(',
      '#warning' => t('user_mail() is replaced by drupal_mail()'),
    ),
    array(
      '#type' => 'regex',
      '#value' => 'user_mail_wrapper\s*\(',
      '#warning' => t('user_mail_wrapper() changed to drupal_mail_wrapper()'),
    ),
    array(
      '#type' => 'regex',
      '#value' => 'message_na\s*\(',
      '#warning' => t('The function message_na() was removed, remove it from your modules as well and replace it with t(\'n/a\')'),
    ),
    array(
      '#type' => 'regex',
      '#value' => 'module_exist\s*\(',
      '#warning' => t('module_exist() is now module_exists()'),
    ),
    array(
      '#type' => 'regex',
      '#value' => 'drupal_call_js\s*\(',
      '#warning' => t('Remove drupal_call_js(), use "drupal_add_js(\'myCustomFunction(your, parameters, here)\', \'inline\');" instead'),
    ),
    array(
      '#type' => 'regex',
      '#original' => 1,
      '#value' => '\$_POST\s*\[\s*[\'"]?op[\'"]?\s*\]',
      '#warning' => t('$_POST[op] deprecated in favor of $form_values[op]'),
    ),
  );
  $review = array(
    '#title' => t('Converting 4.7.x modules to 5.x'),
    '#link' => 'http://drupal.org/node/64279',
    '#rules' => $rules,
  );
  return array('upgrade50' => $review);
}
