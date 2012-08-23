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
    'drupal dependencies' => array('coder_review'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('coder'),
    'options' => array(
      'severity' => 'Show warnings at the specified level, one of major, minor, or critical.',
      'major' => 'Show major and critical warnings.',
      'minor' => 'Show all warnings.',
      'critical' => 'Show only critical warnings.',
      'reviews' => 'Run the specified review(s), accepts a comma separated list (no spaces) of style,i18n,security,sql,comment,upgrade7x,upgrade8x.',
      'comment' => 'Run the comment review.',
      'style' => 'Run the style review (default).',
      'i18n' => 'Run the i18n review.',
      'security' => 'Run the security review.',
      'sql' => 'Run the SQL review.',
      'upgrade7x' => 'Run the 6.x -> 7.x upgrade review.',
      'upgrade8x' => 'Run the 7.x -> 8.x upgrade review.',
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
  $severity = drush_get_option('severity');
  foreach (array('major', 'minor', 'critical') as $option) {
    if ($severity == $option || drush_get_option($option)) {
      $severity_name = $option;
    }
  }
  foreach (array('active', 'core', 'contrib', 'all', 'default') as $option) {
    if (drush_get_option($option)) {
      $settings = _coder_review_get_default_settings($option);
      $settings['coder_includes'] = TRUE;
    }
  }

  $settings = _coder_review_get_default_settings();
  $ignore_option = drush_get_option('ignore');
  if ($ignore_option) {
    foreach (explode(',', $ignore_option) as $option) {
      _coder_review_set_form_modules($settings);
      unset($settings['coder_modules-' . drupal_substr($option, 3)]);
    }
  }

  $avail_reviews = _coder_review_reviews();
  $reviews = array();
  $reviews_option = drush_get_option('reviews');
  if ($reviews_option) {
    foreach (explode(',', $reviews_option) as $option) {
      if (isset($avail_reviews[$option])) {
        $reviews[$option] = $option;
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
          elseif (strpos($arg, '*') !== FALSE || strpos($arg, '%') !== FALSE) {
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
            $settings['coder_includes'] = TRUE;
          }
          else {
            $root = drupal_realpath('.');
            if (_coder_review_drush_is_patch_arg($arg, $root)) {
              $settings['coder_patches'] = 1;
              $settings['coder_patch_link'] = $arg;
            }
            else {
              $path = preg_replace(",^$root/,", '', $_SERVER['OLDPWD'] . '/' . $arg);
              if (file_exists($path)) {
                $settings['coder_files'] = 1;
                if (empty($settings['coder_file_list'])) {
                  $settings['coder_file_list'] = '';
                }
                if (is_dir($path)) {
                  $ext = variable_get('coder_review_php_ext', array('inc', 'php', 'install', 'test'));
                  $settings['coder_file_list'] .= implode("\n", array_keys(drupal_system_listing('/\.(' . implode('|', $ext) . ')$/', $path, 'filepath', 0)));
                }
                else {
                  $settings['coder_file_list'] .= $path . "\n";
                }
              }
              else {
                $settings['coder_modules-' . $arg] = 1;
                $settings['coder_includes'] = TRUE;
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
  if (drupal_substr($arg, 0, 7) == 'http://') {
    return TRUE;
  }
  if (preg_match('/\.(patch|diff|tmp)$/', $arg)) {
    if (isset($_SERVER['OLDPWD'])) {
      $path = preg_replace(",^$root/,", '', $_SERVER['OLDPWD'] . '/' . $arg);
      if (is_file($path)) {
        $arg = $path;
        return TRUE;
      }
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
  $warning = $variables['warning'];
  $severity_name = $variables['severity_name'];
  $lineno = isset($variables['lineno']) ? $variables['lineno'] : 0;
  $line = isset($variables['line']) ? $variables['line'] : '';
  $checkstyle_levels = array(
    'minor' => 'info',
    'normal' => 'warning',
    'critical' => 'error',
  );
  if (drush_get_option('xml') || drush_get_option('checkstyle')) {
    $attr = array(
      'line' => $lineno,
      'column' => 0,
      'severity' => $severity_name,
      'message' => $warning,
      'source' => $line,
    );
    if (drush_get_option('checkstyle')) {
      $attr['severity'] = $checkstyle_levels[$severity_name];
    }
    return '<error ' . drupal_attributes($attr) . ' />';
  }
  $output  = $lineno ? '+' . $lineno . ': ' : '';
  $output .= '[' . $severity_name . '] ';
  $output .= is_array($warning) ? $warning['#warning'] : $warning;
  return _coder_review_drush_output($output);
}

/**
 * Handle printing of drupal_set_message() messages.
 */
function coder_review_print_drush_messages() {
  foreach (drupal_get_messages() as $type => $messages) {
    $output = _coder_review_drush_output(implode("\n ", $messages));
    if (drush_get_option('xml') && !drush_get_option('checkstyle')) {
      _coder_review_drush_print('<status type="' . $type . '">' . $output . '</status>');
    }
    elseif (!drush_get_option('checkstyle')) {
      _coder_review_drush_print(dt(drupal_ucfirst($type) . ' Messages') . ":\n " . $output . "\n");
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
  switch ($type) {
    case 'checkstyle':
      _coder_review_drush_print('<checkstyle version="1.3.0RC1">');
      break;
    case 'xml':
    default:
      _coder_review_drush_print('<coderreview version="1.1">');
      break;
  }
}

/**
 * XML: output appropriate footer tag.
 */
function _coder_review_drush_xml_output_footer($type = 'xml') {
  switch ($type) {
    case 'checkstyle':
      _coder_review_drush_print('</checkstyle>');
      break;
    case 'xml':
    default:
      _coder_review_drush_print('</coderreview>');
      break;
  }
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
