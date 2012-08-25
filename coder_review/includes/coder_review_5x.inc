<?php
/**
 * @file
 * This include file implements coder_review functionality for 4.7 -> 5.x upgrades.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_5x_reviews() {
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]user_mail\s*\(',
    '#warning' => '_coder_review_5x_user_mail_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]user_mail_wrapper\s*\(',
    '#warning' => '_coder_review_5x_user_mail_wrapper_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]message_na\s*\(',
    '#warning' => '_coder_review_5x_message_na_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]module_exist\s*\(',
    '#warning' => '_coder_review_5x_module_exist_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]drupal_call_js\s*\(',
    '#warning' => '_coder_review_5x_drupal_call_js_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]system_listing\s*\(',
    '#warning' => '_coder_review_5x_system_listing_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]node_get_name\s*\(',
    '#warning' => '_coder_review_5x_node_get_name_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#source' => 'allphp',
    '#value' => '\$_POST\s*\[\s*[\'"]?op[\'"]?\s*\]',
    '#warning' => '$_POST[\'op\'] deprecated in favor of $form_values[\'op\']',
    '#severity' => 'normal',
  );
  $rules[] = array(
    '#type' => 'callback',
    '#value' => '_coder_review_5x_callback',
    '#warning' => '_coder_review_5x_callback_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#source' => 'all',
    '#value' => '[\s\(]theme_add_style\s*\(',
    '#warning' => '_coder_review_5x_theme_add_style_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#source' => 'all',
    '#value' => '[\s\(]form_render\s*\(',
    '#warning' => '_coder_review_5x_form_render_warning',
  );

  $review = array(
    '#title' => 'Converting 4.7.x modules to 5.x',
    '#link' => 'http://drupal.org/node/64279',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade5x' => $review);
}

/**
 * Define the rule callbacks for 5.x, see do_coder_review_review_callback().
 */
function _coder_review_5x_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // Only perform this check on module's (not includes).
  $filename = $coder_args['#filename'];
  if (drupal_substr($filename, -7) == '.module') {
    // Make sure that a .info file exists.
    $filename = drupal_substr(realpath($filename), 0, -7) . '.info';
    if (!file_exists($filename)) {
      $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
      _coder_review_error($results, $rule, $severity_name);
    }
  }
}

/**
 * Define the warning callbacks.
 */

function _coder_review_5x_callback_warning() {
  return _t('All modules now need to have a <a href="@info">modulename.info file</a>', array('@info' => 'http://drupal.org/node/101009'));
}

function _coder_review_5x_user_mail_warning() {
  return _t('!user_mail() is replaced by !drupal_mail()',
    array(
      '!user_mail' => _drupalapi('user_mail', 4.7),
      '!drupal_mail' => _drupalapi('drupal_mail', 5),
    )
  );
}

function _coder_review_5x_user_mail_wrapper_warning() {
  return _t('!user_mail_wrapper() changed to !drupal_mail_wrapper()',
    array(
      '!user_mail_wrapper' => _drupalapi('user_mail_wrapper', 4.7),
      '!drupal_mail_wrapper' => _drupalapi('drupal_mail_wrapper', 5),
    )
  );
}

function _coder_review_5x_message_na_warning() {
  return _t('The function !message_na() was removed, remove it from your modules as well and replace it with !t(\'n/a\')',
    array(
      '!message_na' => _drupalapi('message_na', 4.7),
      '!t' => _drupalapi('t', 5),
    )
  );
}

function _coder_review_5x_module_exist_warning() {
  return _t('!module_exist() is now !module_exists()',
    array(
      '!module_exist' => _drupalapi('module_exist', 4.7),
      '!module_exists' => _drupalapi('module_exists', 5),
    )
  );
}

function _coder_review_5x_drupal_call_js_warning() {
  return _t('Remove !drupal_call_js(), use "!drupal_add_js(\'myCustomFunction(your, parameters, here)\', \'inline\');" instead',
    array(
      '!drupal_call_js' => _drupalapi('drupal_call_js', 4.7),
      '!drupal_add_js' => _drupalapi('drupal_add_js', 5),
    )
  );
}

function _coder_review_5x_system_listing_warning() {
  return _t('!system_listing() is now !drupal_system_listing()',
    array(
      '!system_listing' => _drupalapi('system_listing', 4.7),
      '!drupal_system_listing' => _drupalapi('drupal_system_listing', 5),
    )
  );
}

function _coder_review_5x_theme_add_style_warning() {
  return _t('Replace !theme_add_style() with !drupal_add_css()',
    array(
      '!theme_add_style' => _drupalapi('theme_add_style', 4.7),
      '!drupal_add_css' => _drupalapi('drupal_add_css', 5),
    )
  );
}

function _coder_review_5x_form_render_warning() {
  return _t('Replace !form_render() with !drupal_render()',
    array(
      '!form_render' => _drupalapi('form_render', 4.7),
      '!drupal_render' => _drupalapi('drupal_render', 5),
    )
  );
}

function _coder_review_5x_node_get_name_warning() {
  return _t('Replace !node_get_name($node) with !node_get_types(\'name\', $node)',
    array(
      '!node_get_name' => _drupalapi('node_get_name', 4.7),
      '!node_get_types' => _drupalapi('node_get_types', 5),
    )
  );
}
