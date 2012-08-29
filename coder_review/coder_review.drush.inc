<?php
/**
 * @file
 * Command line utility for coder_review.
 */

/**
 * Implements hook_drush_command().
 */
function coder_review_drush_command() {
  $items['coder-review'] = array(
    'callback' => 'coder_review_drush_review',
    'description' => dt('Run code reviews'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('coder'),
    'options' => array(
      'severity' => 'Show warnings at the specified level, one of major, minor, or critical.',
      'major' => 'Show major and critical warnings.',
      'minor' => 'Show all warnings.',
      'critical' => 'Show only critical warnings.',
      'reviews' => 'Run the specified review(s), "all" runs all reviews, also accepts a comma separated list (no spaces).',
      'comment' => 'Run the comment review.',
      'style' => 'Run the style review (default).',
      'i18n' => 'Run the i18n review.',
      'security' => 'Run the security review.',
      'sql' => 'Run the SQL review.',
      'upgrade7x' => 'Run the 6.x -> 7.x upgrade review.',
      'upgrade8x' => 'Run the 7.x -> 8.x upgrade review.',
      'druplart' => 'Run the druplart review.',
      'core' => 'Review all of core.',
      'contrib' => 'Review all of contrib.',
      'active' => 'Review all active modules and themes.',
      'all' => 'Review all modules and themes (default).',
      'default' => 'Review "default" modules and themes (as defined in coder settings).',
      'summary' => 'Display summary information only, no warning output.',
      'no-empty' => 'Hide results that are empty',
      'ignore' => 'Module(s) to ignore, accepts comma separated list (no spaces).',
      'xml' => 'Output results as xml.',
      'checkstyle' => 'Output results in Checkstyle xml format.',
    ),
    'arguments' => array(
      'name' => 'Review specified module or theme name, i.e. taxonomy.',
      'patch-url' => 'URL to patch file.',
      'file-path' => 'Path to file or multiple files, relative to Drupal installation.',
    ),
  );
  return $items;
}

/**
 * @todo: add function header comment.
 */
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

/**
 * Do the actual review.
 */
function coder_review_drush_review() {
  // Bootstrap enough to run.
  require_once realpath(dirname(__FILE__)) . '/coder_review.common.inc';

  // Get the command line options severity options.
  $severity = drush_get_option('severity');
  foreach (array('major', 'minor', 'critical') as $option) {
    if ($severity == $option || drush_get_option($option)) {
      $severity_name = $option;
    }
  }

  // Get the command line options module grouping options.
  foreach (array('active', 'core', 'contrib', 'all', 'default') as $option) {
    if (drush_get_option($option)) {
      $settings = _coder_review_get_default_settings($option);
      $settings['coder_includes'] = 1;
      break;
    }
  }
  if (!isset($settings)) {
    $settings = _coder_review_get_default_settings();
  }

  // Get the command line options ignore options.
  $ignore_option = drush_get_option('ignore');
  if ($ignore_option) {
    foreach (explode(',', $ignore_option) as $option) {
      _coder_review_set_form_modules($settings);
      unset($settings['coder_modules-' . drupal_substr($option, 3)]);
    }
  }

  // Get the command line review options.
  $avail_reviews = _coder_review_reviews();
  $reviews = array();
  $reviews_option = drush_get_option('reviews');
  if ($reviews_option) {
    if ($reviews_option == 'all') {
      $reviews += drupal_map_assoc(array_keys($avail_reviews));
    }
    else {
      foreach (explode(',', $reviews_option) as $option) {
        if (isset($avail_reviews[$option])) {
          $reviews[$option] = $option;
        }
      }
    }
  }
  foreach (array_keys($avail_reviews) as $option) {
    if (drush_get_option($option)) {
      $reviews[$option] = $option;
    }
  }
  if (empty($reviews)) {
    $reviews['style'] = 'style';
  }

  // Process command line arguments.
  $args = func_get_args();
  $modules = array();
  $output = array();
  if (!empty($args)) {
    foreach ($args as $arg) {
      switch ($arg) {
        case 'summary':
        case 'no-empty':
        case 'xml':
        case 'checkstyle':
        case 'active':
        case 'core':
        case 'contrib':
        case 'all':
        case 'default':
          return drush_set_error(dt('use --@option.', array('@option' => $arg)));

        case 'major':
        case 'minor':
        case 'critical':
          return drush_set_error(dt('use --severity or --@option.', array('@option' => $arg)));

        default:
          if (isset($avail_reviews[$arg])) {
            return drush_set_error(dt('use --reviews or --@option.', array('@option' => $arg)));
          }
          elseif (function_exists('db_query') && (strpos($arg, '*') !== FALSE || strpos($arg, '%') !== FALSE)) {
            $result = db_query('SELECT name FROM {system} WHERE name LIKE :name', array(':name' => str_replace('*', '%', $arg)));
            foreach ($result as $system) {
              $settings['coder_modules-' . $system->name] = 1;
              $match = TRUE;
            }
            if (!isset($match)) {
              _coder_review_drush_print(dt('no matches found for @name', array('@name' => $arg)));
              return;
            }
            unset($settings['coder_active_modules']);
            unset($settings['coder_core']);
            unset($settings['coder_all']);
            unset($settings['coder_modules']);
            $settings['coder_includes'] = 1;
          }
          else {
            $root = dirname(__FILE__);
            if (_coder_review_drush_is_patch_arg($arg, $root)) {
              $settings['coder_patches'] = 1;
              $settings['coder_patch_link'] = $arg;
            }
            else {
              $path = preg_replace(",^$root/,", '', (function_exists('drush_getcwd') ? drush_getcwd() : getcwd()) . '/' . $arg);
              if (!file_exists($path)) {
                $path = $root . '/' . $arg;
                if (!file_exists($path)) {
                  $path = '';
                }
              }
              if ($path) {
                $settings['coder_files'] = 1;
                if (empty($settings['coder_file_list'])) {
                  $settings['coder_file_list'] = '';
                }
                if (is_dir($path)) {
                  $ext = _variable_get('coder_review_php_ext', array('inc', 'php', 'install', 'test'));
                  // @todo: convert this to not use drupal_system_listing().
                  $settings['coder_file_list'] .= implode("\n", array_keys(drupal_system_listing('/\.(' . implode('|', $ext) . ')$/', $path, 'filepath', 0)));
                  $settings['coder_includes'] = 1;
                }
                else {
                  if (empty($settings['coder_file_lsit'])) {
                    $settings['coder_includes'] = 0;
                  }
                  $settings['coder_file_list'] .= $path . "\n";
                }
              }
              else {
                $settings['coder_modules-' . $arg] = 1;
                $settings['coder_includes'] = 1;
              }
            }
            unset($settings['coder_active_modules']);
            unset($settings['coder_core']);
            unset($settings['coder_all']);
            unset($settings['coder_modules']);
          }
          break;
      }
    }
  }
  if (drush_get_option('checkstyle')) {
    _coder_review_drush_xml_output_header('checkstyle');
  }
  elseif (drush_get_option('xml')) {
    _coder_review_drush_xml_output_header('xml');
  }
  if (!empty($severity_name)) {
    if (drush_get_option('xml')) {
      _coder_review_drush_xml_output_severity($severity_name);
    }
    $output[] = dt('Severity @severity_name', array('@severity_name' => $severity_name));
    $settings['coder_severity'] = _coder_review_severity($severity_name);
  }
  if (!empty($reviews)) {
    if (drush_get_option('xml') && !drush_get_option('checkstyle')) {
      _coder_review_drush_xml_output_reviews($reviews, $avail_reviews);
    }
    else {
      foreach ($reviews as $review) {
        $output[] = $avail_reviews[$review]['#title'];
      }
    }
    $settings['coder_reviews'] = $reviews;
  }
  if (!empty($output) && !drush_get_option('summary') && !drush_get_option('xml') && !drush_get_option('checkstyle')) {
    _coder_review_drush_print(implode(', ', $output) . "\n");
  }
  _coder_review_set_form_modules($settings);
  $settings['op'] = 'drush';

  $form_state['storage'] = $settings;
  coder_review_page_form(array(), $form_state);

  if (drush_get_option('checkstyle')) {
    _coder_review_drush_xml_output_footer('checkstyle');
  }
  elseif (drush_get_option('xml')) {
    _coder_review_drush_xml_output_footer('xml');
  }
}

/**
 * Handle patch reviews.
 */
function _coder_review_drush_is_patch_arg(&$arg, $root) {
  if (substr($arg, 0, 7) == 'http://') {
    return TRUE;
  }
  if (_drush() && preg_match('/\.(patch|diff|tmp)$/', $arg)) {
    $path = preg_replace(",^$root/,", '', drush_cwd() . '/' . $arg);
    if (is_file($path)) {
      $arg = $path;
      return TRUE;
    }
  }
}

/**
 * Theme printing of results and filenames.
 */
function theme_drush_coder_review($variables) {
  $name = $variables['name'];
  $filename = $variables['filename'];
  $results = $variables['results'];
  if (!drush_get_option('summary') && !empty($results) && ((!empty($results) && !isset($results[0])) || !drush_get_option('no-empty'))) {
    if (drush_get_option('checkstyle') || drush_get_option('xml')) {
      _coder_review_drush_xml_output_results($filename, $results);
    }
    else {
      _coder_review_drush_print($filename . ":\n " . implode("\n ", $results) . "\n");
    }
  }
}

/**
 * Theme warning message, including source snippet.
 */
function theme_drush_coder_review_warning($variables) {
  // Supply default theme variables.
  $variables += array(
    'lineno' => 0,
    'line' => '',
    'warning' => dt('Unknown warning (drush)'),
  );

  // Return warning as XML.
  if (drush_get_option('xml') || drush_get_option('checkstyle')) {
    $attr = array(
      'line' => $variables['lineno'],
      'column' => 0,
      'severity' => $variables['severity_name'],
      'message' => $variables['warning'],
      'source' => $variables['line'],
    );
    if (drush_get_option('checkstyle')) {
      $checkstyle_levels = array(
        'minor' => 'info',
        'normal' => 'warning',
        'critical' => 'error',
      );
      $attr['severity'] = $checkstyle_levels[$variables['severity_name']];
    }
    return '<error ' . drupal_attributes($attr) . ' />';
  }

  // Return warning as text output, formatted for the drush screen.
  $output  = $variables['lineno'] ? ($variables['lineno'] == -1 ? dt('File') : ('+' . $variables['lineno'])) . ': ' : '';
  $output .= '[' . $variables['severity_name'] . '] ';
  if (is_string($variables['warning'])) {
    $output .= $variables['warning'];
  }
  elseif (isset($variables['warning']['#warning'])) {
    $output .= dt($variables['warning']['#warning']);
  }
  elseif (isset($variables['warning']['#text'])) {
    $variables['warning'] += array('#args' => array());
    $output .= call_user_func('dt', $variables['warning']['#text'], $variables['warning']['#args']);
  }
  return _coder_review_drush_output($output);
}

/**
 * Handle printing of drupal_set_message() messages.
 */
function coder_review_print_drush_messages() {
  foreach (drupal_get_messages() as $type => $messages) {
    $output = _coder_review_drush_output(implode("\n ", $messages));
    if (!drush_get_option('checkstyle')) {
      if (drush_get_option('xml')) {
        _coder_review_drush_print('<status type="' . $type . '">' . $output . '</status>');
      }
      else {
        _coder_review_drush_print(dt(ucfirst($type) . ' Messages') . ":\n " . $output . "\n");
      }
    }
  }
}

/**
 * Strip HTML tags from drush output.
 */
function _coder_review_drush_output($output) {
  return html_entity_decode(strip_tags($output));
}

/**
 * Switch between print and pipe print output.
 */
function _coder_review_drush_print($message) {
  if (drush_get_context('DRUSH_PIPE')) {
    drush_print_pipe($message);
  }
  else {
    drush_print($message);
  }
}

/**
 * XML output functions.
 */

/**
 * XML: output appropriate header tag.
 */
function _coder_review_drush_xml_output_header($type = 'xml') {
  // Put in an extra concatenation so syntax highlighting in vim doesn't break.
  _coder_review_drush_print('<?xml version="1.0" encoding="UTF-8"?' . '>');
  _coder_review_drush_print($type == 'checkstyle' ? '<checkstyle version="1.3.0RC1">' : '<coderreview version="1.1">');
}

/**
 * XML: output appropriate footer tag.
 */
function _coder_review_drush_xml_output_footer($type = 'xml') {
  _coder_review_drush_print($type == 'checkstyle' ? '</checkstyle>' : '</coderreview>');
}

/**
 * XML: output severity level if provided.
 */
function _coder_review_drush_xml_output_severity($severity_name) {
  _coder_review_drush_print('<severity>' . $severity_name . '</severity>');
}

/**
 * XML: output list of chosen reviews.
 */
function _coder_review_drush_xml_output_reviews($reviews, $avail_reviews) {
  _coder_review_drush_print('<reviews>');
  foreach ($reviews as $review) {
    _coder_review_drush_print('<review>' . $avail_reviews[$review]['#title'] . '</review>');
  }
  _coder_review_drush_print('</reviews>');
}

/**
 * XML: output filename and its results (if any).
 */
function _coder_review_drush_xml_output_results($filename, $results) {
  if (empty($results) || (count($results) == 1 && isset($results[0]))) {
    _coder_review_drush_print('<file name = "' . $filename . '" />');
  }
  else {
    _coder_review_drush_print('<file name = "' . $filename . '">' . "\n" . implode("\n ", $results) . "\n" . '</file>');
  }
}
