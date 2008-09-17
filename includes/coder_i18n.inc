<?php
// $Id$

/**
 * @file
 * This include file implements coder functionality to check for Internationalization issues.
 */

/**
 * Implementation of hook_reviews().
 */
function coder_i18n_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]l\s*\(\s*["\']',
      '#filename-not' => '\.install$',
      '#warning_callback' => '_coder_i18n_l_without_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]l\s*\(\s*["\']',
      '#function' => '_install$',
      '#warning_callback' => '_coder_i18n_in_install_l_without_st',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]t\s*\(\s*["\']',
      '#function' => '_install$',
      '#warning_callback' => '_coder_i18n_in_install_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]alert\s*\(\s*["\']',
      '#filename' => '\.js$',
      '#warning' => 'Javascript strings should be passed through Drupal.t().',
    ),
  );
  $review = array(
    '#title' => t('Internationalization'),
    '#rules' => $rules,
    '#severity' => 'major',
  );
  return array('i18n' => $review);
}

/**
 * Define the warning callbacks.
 */

function _coder_i18n_l_without_t() {
  return array(
    '#warning' => t('The $text argument to !l() should be enclosed within !t() so that it is translatable.',
      array(
        '!l' => theme('drupalapi', 'l'),
        '!t' => theme('drupalapi', 't'),
      )
    ),
  );
}

function _coder_i18n_in_install_l_without_st() {
  return array(
    '#warning' => t('The $text argument to !l() should be enclosed within !st() so that it is translatable from within the install.',
      array(
        '!l' => theme('drupalapi', 'l'),
        '!st' => theme('drupalapi', 'st'),
      )
    ),
  );
}

function _coder_i18n_in_install_t() {
  return array(
    '#warning' => t('Use !get_t() or !st() instead of !t() in !hook_install(), !hook_uninstall() and !hook_update_N()',
      array(
        '!get_t' => theme('drupalapi', 'get_t'),
        '!st' => theme('drupalapi', 'st'),
        '!t' => theme('drupalapi', 't'),
        '!hook_install' => theme('drupalapi', 'hook_install'),
        '!hook_uninstall' => theme('drupalapi', 'hook_uninstall'),
        '!hook_update_N' => theme('drupalapi', 'hook_update_N'),
      )
    ),
  );
}
