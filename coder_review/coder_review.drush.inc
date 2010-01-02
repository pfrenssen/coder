<?php
// $Id$

/**
 * @file
 * Command line utility for coder_review.
 */

/**
 * Implements hook_drush_help().
 */
function coder_review_drush_help($section) {
  switch ($section) {
    case 'drush:coder review':
      return dt('Run code reviews');
  }
}

/**
 * Implements hook_drush_command().
 */
function coder_review_drush_command() {
  $items['coder review'] = array(
    'callback' => 'coder_review_drush_review',
    'description' => dt('Run code reviews'),
    'drupal dependencies' => array('coder_review'),
  );
  return $items;
}

function _coder_review_set_form_modules(&$settings) {
  if (isset($settings['coder_modules'])) {
    foreach ($settings['coder_modules'] as $key => $value) {
      if ($value == 1) {
        $settings['coder_modules-' . $key] = 1;
      }
    }
    unset($settings['coder_modules']);
  }
}

function coder_review_drush_review() {
  _coder_review_drush_set_option('drush');

  // process command line arguments
  $args = func_get_args();
  $reviews = array();
  $modules = array();
  $settings = _coder_review_get_default_settings();
  if (count($args)) {
    $avail_reviews = _coder_review_reviews();
    foreach ($args as $arg) {
      switch ($arg) {
        case 'summary':
        case 'no-empty':
          _coder_review_drush_set_option($arg);
          break;
        case 'active':
        case 'core':
        case 'contrib':
        case 'all':
        case 'default':
          $settings = _coder_review_get_default_settings($arg);
          $settings['coder_includes'] = TRUE;
          break;
        case 'major':
        case 'minor':
        case 'critical':
          $severity_name = $arg;
          break;
        default:
          if (isset($avail_reviews[$arg])) {
            $reviews[$arg] = $arg;
          }
          elseif (substr($arg, 0, 3) == 'no-') {
            _coder_review_set_form_modules($settings);
            unset($settings['coder_modules-' . substr($arg, 3)]);
          }
          else {
            if (_coder_review_drush_is_patch_arg($arg)) {
              $settings['coder_patches'] = 1;
              $settings['coder_patch_link'] = $arg;
            }
            else {
              $settings['coder_modules-' . $arg] = 1;
              $settings['coder_includes'] = TRUE;
            }
            unset($settings['coder_active_modules']);
            unset($settings['coder_core']);
            unset($settings['coder_all']);
            unset($settings['coder_modules']);
          }
          break;
      }
    }
    $output = array();
    if (isset($severity_name)) {
      $output[] = dt('Severity @severity_name', array('@severity_name' => $severity_name));
      $settings['coder_severity'] = _coder_review_severity($severity_name);
    }
    if (count($reviews)) {
      foreach ($reviews as $review) {
        $output[] = $avail_reviews[$review]['#title'];
      }
      $settings['coder_reviews'] = $reviews;
    }
    if (count($output) && !_coder_review_drush_is_option('summary')) {
      drush_print(implode(', ', $output) . "\n");
    }
  }
  _coder_review_set_form_modules($settings);
  $settings['op'] = 'drush';

  $form_state['storage'] = $settings;
  coder_review_page_form($form_state);
}

function _coder_review_drush_is_patch_arg(&$arg) {
  if (substr($arg, 0, 7) == 'http://' || is_file(realpath($arg))) {
    return TRUE;
  }
  // @NOTE: relies on http://drupal.org/node/297611
  if (isset($_SERVER['OLDPWD'])) {
    $path = $_SERVER['OLDPWD'] . '/' . $arg;
    if (is_file($path)) {
      $arg = $path;
      return TRUE;
    }
  }
}

function theme_drush_coder_review($name, $filename, $results) {
  if (!_coder_review_drush_is_option('summary') && !empty($results) && ((count($results) && !isset($results[0])) || !_coder_review_drush_is_option('no-empty'))) {
    drush_print($filename . ":\n " . implode("\n ", $results) . "\n");
  }
}

function theme_drush_coder_review_warning($warning, $severity_name, $lineno = 0, $line = '') {
  $output  = $lineno ? '+' . $lineno . ': ' : '';
  $output .= is_array($warning) ? $warning['#warning'] : $warning;
  return _coder_review_drush_output($output);
}

function coder_review_print_drush_messages() {
  foreach (drupal_get_messages() as $type => $messages) {
    drush_print(dt(ucfirst($type) . ' Messages') . ":\n " . _coder_review_drush_output(implode("\n ", $messages)) . "\n");
  }
}

function _coder_review_drush_output($output) {
  return html_entity_decode(strip_tags($output));
}

function _coder_review_drush_set_option($option) {
  global $_coder_drush_options;
  if (!isset($_coder_drush_options)) {
    $_coder_drush_options = array();
  }
  $_coder_drush_options[$option] = TRUE;
}

function _coder_review_drush_is_option($option) {
  global $_coder_drush_options;
  return isset($_coder_drush_options[$option]);
}
