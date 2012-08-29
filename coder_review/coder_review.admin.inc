<?php
/**
 * @file
 * Coder module administrative forms and functions.
 */

/**
 * Configuration page.
 */
function coder_review_admin_settings($form, &$form_state) {
  $settings = _coder_review_get_default_settings();
  $form = _coder_review_settings_form($settings, $system, $files);
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => t('After setting these defaults, use <a href="@url">coder review</a> to perform code reviews.', array('@url' => url('admin/config/development/coder/review'))),
    '#weight' => -1,
  );
  $form['#submit'][] = 'coder_review_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Callback function for settings page.
 */
function coder_review_settings_form_submit($form, &$form_state) {
  $form_state['storage'] = $form_state['values'];
  variable_set('coder_modules', _coder_review_settings_array($form_state, 'module'));
  variable_set('coder_themes', _coder_review_settings_array($form_state, 'theme'));
}

