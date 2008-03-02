<?php
/**
 * Implementation of hook_drush_command().
 */
function coder_drush_command() {
  $items['coder'] = array(
    'callback' => 'coder_drush_review',
    'description' => 'Run coder review.'
  );
  return $items;
}

function coder_drush_review() {
  // process command line arguments
  $args = func_get_args();
  $reviews = array();
  $modules = array();
  $settings = _coder_get_default_settings();
  if (count($args)) {
    $avail_reviews = _coder_reviews();
    foreach ($args as $arg) {
      switch ($arg) {
        case 'active':
        case 'core':
        case 'all':
          $settings = _coder_get_default_settings($arg);
          break;
        default:
          if (isset($avail_reviews[$arg])) {
            $reviews[$arg] = $arg;
          }
          else {
            unset($settings['coder_active_modules']);
            unset($settings['coder_core']);
            unset($settings['coder_all']);
            $settings['coder_modules-'. $arg] = 1;
          }
          break;
      }
    }
    if (count($reviews)) {
      $settings['coder_reviews'] = $reviews;

      $output = array();
      foreach ($reviews as $review) {
        $output[] = $avail_reviews[$review]['#title'];
      }
      print implode(', ', $output) ."\n\n";
    }
  }
  $settings['op'] = 'drush';

  global $_coder_is_drush;
  $_coder_is_drush = TRUE;
  coder_page_form($settings);
}

function coder_is_drush() {
  global $_coder_is_drush;
  return isset($_coder_is_drush);
}

function theme_drush_coder($name, $filename, $results) {
  if (!empty($results) && $results[0] != t('No Problems Found')) {
    print $filename .":\n ". implode("\n ", $results) ."\n\n";
  }
}

function theme_drush_coder_warning($warning, $severity_name, $lineno = 0, $line = '') {
  $output  = $lineno ? '+'. $lineno .': ' : '';
  $output .= is_array($warning) ? $warning['#warning'] : $warning;
  return strip_tags($output);
}
