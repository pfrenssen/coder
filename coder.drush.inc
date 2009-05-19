<?php
/**
 * Implementation of hook_drush_command().
 */
function coder_drush_command() {
  $items['coder'] = array(
    'callback' => 'coder_drush_review',
    'description' => dt('Run code reviews'),
  );
  return $items;
}

function _coder_set_form_modules(&$settings) {
  if (isset($settings['coder_modules'])) {
    foreach ($settings['coder_modules'] as $key => $value) {
      if ($value == 1) {
        $settings['coder_modules-'. $key] = 1;
      }
    }
    unset($settings['coder_modules']);
  }
}

function coder_drush_review() {
  _coder_drush_set_option('drush');

  // process command line arguments
  $args = func_get_args();
  $reviews = array();
  $modules = array();
  $settings = _coder_get_default_settings();
  if (count($args)) {
    $avail_reviews = _coder_reviews();
    foreach ($args as $arg) {
      switch ($arg) {
        case 'summary':
        case 'no-empty':
          _coder_drush_set_option($arg);
          break;
        case 'active':
        case 'core':
        case 'all':
        case 'default':
          $settings = _coder_get_default_settings($arg);
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
            _coder_set_form_modules($settings);
            unset($settings['coder_modules-'. substr($arg, 3)]);
          }
          else {
            unset($settings['coder_active_modules']);
            unset($settings['coder_core']);
            unset($settings['coder_all']);
            unset($settings['coder_modules']);
            $settings['coder_modules-'. $arg] = 1;
            $settings['coder_includes'] = TRUE;
          }
          break;
      }
    }
    $output = array();
    if (isset($severity_name)) {
      $output[] = dt('Severity @severity_name', array('@severity_name' => $severity_name));
      $settings['coder_severity'] = _coder_severity($severity_name);
    }
    if (count($reviews)) {
      foreach ($reviews as $review) {
        $output[] = $avail_reviews[$review]['#title'];
      }
      $settings['coder_reviews'] = $reviews;
    }
    if (count($output) && !_coder_drush_is_option('summary')) {
      drush_print(implode(', ', $output) ."\n");
    }
  }
  _coder_set_form_modules($settings);
  $settings['op'] = 'drush';

  coder_page_form($settings);
}

function theme_drush_coder($name, $filename, $results) {
  if (!_coder_drush_is_option('summary') && !empty($results) && (count($results) > 1 || !_coder_drush_is_option('no-empty'))) {
    drush_print($filename .":\n ". implode("\n ", $results) ."\n");
  }
}

function theme_drush_coder_warning($warning, $severity_name, $lineno = 0, $line = '') {
  $output  = $lineno ? '+'. $lineno .': ' : '';
  $output .= is_array($warning) ? $warning['#warning'] : $warning;
  return _coder_drush_output($output);
}

function coder_print_drush_messages() {
  foreach (drupal_get_messages() as $type => $messages) {
    drush_print(dt(ucfirst($type) .' Messages') .":\n ". _coder_drush_output(implode("\n ", $messages)) ."\n");
  }
}

function _coder_drush_output($output) {
  return html_entity_decode(strip_tags($output));
}

function _coder_drush_set_option($option) {
  global $_coder_drush_options;
  if (!isset($_coder_drush_options)) {
    $_coder_drush_options = array();
  }
  $_coder_drush_options[$option] = TRUE;
}

function _coder_drush_is_option($option) {
  global $_coder_drush_options;
  return isset($_coder_drush_options[$option]);
}
