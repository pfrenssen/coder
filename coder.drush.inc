<?php
// $Id$

/**
 * @file
 * Command line utility for coder.
 */

/**
 * Implements hook_drush_help().
 */
function coder_drush_help($section) {
  switch ($section) {
    case 'drush:coder':
      return dt("Review source files for code that doesn't follow Drupal's coding standards or that needs to change due to Drupal API changes. If no arguments specified, the default settings configured at admin/settings/coder will be used.");
  }
}

/**
 * Implements hook_drush_command().
 */
function coder_drush_command() {
  $items['coder'] = array(
    'callback' => 'coder_drush_review',
    'description' => dt('Run code reviews'),
    'drupal dependencies' => array('coder'),
    'examples' => array(
      'drush coder active' =>  'Review all active modules and themes.',
      'drush coder contrib critical' => 'Review installed contrib modules and themes, but only return critical warnings found.',
      'drush coder major cck' => 'Review cck module for major or higher level warnings.',
      'drush coder style zen' => 'Review zen theme for coding style standards.',
      'drush coder style sql zen' => 'Review zen theme for coding style and sql standards.',
      'drush coder http://example.com/foo.patch' => 'Review a patch file.',
      'drush coder /src/myfile.patch' => 'Review a local patch file.',
    ),
    'options' => array(
      'summary' => 'Display summary statistics instead of default detailed messages.',
      'no-empty' => 'Only display files with warning or error messages.',
      'active' => 'Review all active projects.',
      'core' => 'Review all core modules and themes.',
      'contrib' => 'Review all contributed projects.',
      'all' => 'Review all projects, both in core and contrib.',
      'default' => 'Review all projects configured in the default set via the UI.',
      'critical' => 'Only return results with the critical severity.',
      'major' => 'Return results with the major and critical severities.',
      'minor' => 'Return results marked as minor severity.',
      'upgrade47' => 'Converting 4.6.x modules to 4.7.x',
      'upgrade50' => 'Converting 4.7.x modules to 5.x',
      'upgrade6x' => 'Converting 5.x modules to 6.x',
      'comment' => 'Drupal Commenting Standards',
      'i18n' => 'Internationalization',
      'security' => 'Drupal Security Checks',
      'sql' => 'Drupal SQL Standards',
      'style' => 'Drupal Coding Standards',
    ),
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
        case 'contrib':
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
          elseif (drupal_substr($arg, 0, 3) == 'no-') {
            _coder_set_form_modules($settings);
            unset($settings['coder_modules-'. drupal_substr($arg, 3)]);
          }
          else {
            if (_coder_drush_is_patch_arg($arg)) {
              $settings['coder_patches'] = 1;
              $settings['coder_patch_link'] = $arg;
            }
            else {
              $settings['coder_modules-'. $arg] = 1;
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

  $form_state['storage'] = $settings;
  coder_page_form($form_state);
}

function _coder_drush_is_patch_arg(&$arg) {
  if (drupal_substr($arg, 0, 7) == 'http://' || is_file(realpath($arg))) {
    return TRUE;
  }
  // @NOTE: relies on http://drupal.org/node/297611
  if (isset($_SERVER['OLDPWD'])) {
    $path = $_SERVER['OLDPWD'] .'/'. $arg;
    if (is_file($path)) {
      $arg = $path;
      return TRUE;
    }
  }
}

function theme_drush_coder($name, $filename, $results) {
  if (!_coder_drush_is_option('summary') && !empty($results) && ((count($results) && !isset($results[0])) || !_coder_drush_is_option('no-empty'))) {
    drush_print($filename .":\n ". implode("\n ", $results) ."\n");
  }
}

function theme_drush_coder_warning($warning, $severity_name, $lineno = 0, $line = '') {
  $output  = $lineno ? '+'. $lineno .': ' : '';
  $output .= '[' . $severity_name . '] ';
  $output .= is_array($warning) ? $warning['#warning'] : $warning;
  return _coder_drush_output($output);
}

function coder_print_drush_messages() {
  foreach (drupal_get_messages() as $type => $messages) {
    drush_print(dt(drupal_ucfirst($type) .' Messages') .":\n ". _coder_drush_output(implode("\n ", $messages)) ."\n");
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
