<?php
// $Id$

/**
 * @file
 * This include file implements coder functionality for Drupal Standards.
 *
 * @todo The rules for this review are not yet complete.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_security_reviews() {
  $table = '\{[A-Za-z_]+\}'; // table-regex
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_title\s*\(\s*[^\$)]+.+\$',
      '#never' => '(^function\s|drupal_set_title\s*\(\s*(((st|t|\$t)\s*\()|(format_plural|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_drupal_set_title_filter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_title\s*\(\s*(st|t|\$t)\s*\(\s*((.*?\$)|([\'"].*?!\w+.*?[\'"]\s*,)|(.*?array\(.*?!\w+.*?\)))',
      '#never' => '(^function\s|drupal_set_title\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_drupal_set_title_filter_t_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_message\s*\(\s*[^\$)]+.+\$',
      '#never' => '(^function\s|drupal_set_message\s*\(\s*(((st|t|\$t)\s*\(.*?array\([^!]+\))|(format_plural|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_drupal_set_message_filter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_message\s*\(\s*(st|t|\$t)\s*\(\s*((.*?\$)|([\'"].*?!\w+.*?[\'"]\s*,)|(.*?array\(.*?!\w+.*?\)))',
      '#never' => '(^function\s|drupal_set_message\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_drupal_set_message_filter_t_warning',
    ),
    array(
      '#type' => 'regex',
      '#severity' => 'minor',
      '#value' => '[\s\(]l\(check_plain\(.*',
      '#never' => '[\'"]html[\'"]\s*=>\s*(TRUE|1)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_l_check_plain_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(?-i)\$REQUEST_URI',
      '#warning_callback' => '_coder_review_security_request_uri_warning',
      '#function-not' => '^(request_uri|drupal_detect_baseurl)$',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '(?-i)\"REQUEST_URI\"|\'REQUEST_URI\'',
      '#warning_callback' => '_coder_review_security_request_uri_warning',
      '#function-not' => '^(request_uri|drupal_detect_baseurl)$',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+' . $table . '|insert\s+into\s+' . $table . '|update\s+' . $table . '\s+set|delete\s+from\s+' . $table . ')\s+.*\$[a-z0-9_]+',
      '#not' => '\$placeholder',
      '#never' => '[\s\(]update_sql\(',
      '#source' => 'quote',
      '#warning_callback' => '_coder_review_security_sql_var_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+' . $table . '|insert\s+into\s+' . $table . '|update\s+' . $table . '\s+set|delete\s+from\s' . $table . ')\s+[^\']*?(\s+|\(|=|,)\%s',
      '#source' => 'quote',
      '#warning' => 'SQL query handling data in a potentially insecure way by using the %%s placeholder without wrapping it in single quotes.  This is a potential source of SQL injection attacks when the value can come from user data.',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp', // allow us to look inside the regex string
      '#value' => '\bpreg_replace\s*\(\s*(\'(.)([^\'\\\\]|\\\\.)*\\2([^\'\\\\]|\\\\.)*|"(.)([^"\\\\]|\\\\.)*\\5([^"\\\\]|\\\\.)*)e',
      '#warning' => "Use preg_replace_callback() instead of the 'e' modifier to preg_replace()",
      '#severity' => 'critical',
    ),
    array(
      '#type' => 'regex',
      '#value' => '.*[\'"]SELECT\s+.*\s+(FROM|JOIN)\s+\{node\}',
      '#never' => '([\s\(]db_rewrite_sql\s*\(|\s\$\w+\s*=\s*[\'"]SELECT)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_db_rewrite_sql',
      '#function-not' => '_cron$',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]db_rewrite_sql\(\s*[\'"]SELECT\s+.*\s+(FROM|JOIN)\s+\{node\}([,\'"]+|\s+(ON|WHERE|HAVING|LIMIT|ORDER|GROUP)\s+)',
      '#never' => '[\s\(]db_rewrite_sql\(\s*[\'"]SELECT\s+.*\s+(FROM|JOIN)\s+\{node\}([,\'"]+|\s+(ON|WHERE|HAVING|LIMIT|ORDER|GROUP)\s+).*?,\s*[\'"]{node}[\'"]',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_db_rewrite_sql',
      '#function-not' => '_cron$',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\'"]access callback.*=.*\(',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_menu_access_callback_warning',
      '#function' => '_menu$',
    ),
    array(
      '#type' => 'callback',
      '#value' => '_coder_review_security_callback',
    ),
  );
  $review = array(
    '#title' => 'Drupal Security Checks',
    '#link' => 'http://drupal.org/node/28984',
    '#rules' => $rules,
    '#severity' => 'critical',
    '#description' => t('very basic, needs work, but what it finds is good'),
  );
  return array('security' => $review);
}


/**
 * Define the rule callbacks.
 */

function _coder_review_security_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $severity_name = _coder_severity_name($coder_args, $review, $rule);
  $ignores = $coder_args['#ignore_lines'][$review['#review_name']];
  /*
  if (!isset($coder_args['#tokens'])) {
    $source = implode('', $lines);
    $coder_args['#tokens'] = token_get_all($source);
  }
  */

  // Check for calls to drupal_set_title() and drupal_set_message() on $vars
  // without a sanitizing function. If found, look back up within current
  // function.
  $this_function = '';
  $function_paren = 0;
  $forward_matches = array();
  foreach ($coder_args['#all_lines'] as $lineno => $line) {
    // Start of a function, store the line.
    if (preg_match('/function (\w+)\(/', $line, $match)) {
      $this_function = $line;
    }
    // Within a function.
    elseif ($function_paren > 0) {
      $this_function .= $line;
    }
    // Check to see if we're still in a function by counting number of {} chars.
    $tmp_line = preg_replace(array('/([^\\\])\'.+?[^\\\]\'/', '/([^\\\])".+?[^\\\]"/'), '$1', $line);
    if (preg_match('/([{}])/', $tmp_line, $match)) {
      $function_paren += ($match[0] == '{') ? 1 : -1;
      // If we've just exited a function, run forward match checks.
      if ($function_paren <= 0) {
        // db_rewrite_sql()
        if (isset($forward_matches['db_rewrite_sql'])) {
          $after_never_regex = '/[\s\(]db_rewrite_sql\s*\(\s*\$' . $forward_matches['db_rewrite_sql'] . '[\s,\)]/';
          if (!preg_match($after_never_regex, $this_function, $sanitized_matches)) {
            $msg = _coder_review_security_db_rewrite_sql();
            $rule = array('#warning' => $msg);
            _coder_error($results, $rule, $severity_name, $lineno, $line, $ignores);
          }
        }
        $forward_matches = array();
      }
    }
    // If we're not in a function, continue.
    if ($function_paren < 0 || $this_function == '') {
      continue;
    }

    // Run our multi-line reviews.
    // drupal_set_title()
    $regex = '/[\s\(]drupal_set_title\s*\(\s*\$(\w+)\s*[,\)]/';
    $never_regex = '/(^function\s|drupal_set_title\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss_admin|check_plain|check_markup)\s*\().*$)/';
    if (preg_match($regex, $line, $matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[1] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $msg = _coder_review_security_drupal_set_title_filter_warning();
          $rule = array('#warning' => $msg);
          _coder_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    // drupal_set_message()
    $regex = '/[\s\(]drupal_set_message\s*\(\s*\$(\w+)\s*[,\)]/';
    $never_regex = '/(^function\s|drupal_set_message\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss_admin|check_plain|check_markup)\s*\().*$)/';
    if (preg_match($regex, $line, $matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[1] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $msg = _coder_review_security_drupal_set_message_filter_warning();
          $rule = array('#warning' => $msg);
          _coder_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    // db_rewrite_sql()
    $regex = '/\$(\w+)\s*=\s*[\'"]SELECT\s+.*\s+(FROM|JOIN)\s+\{node\}/';
    if (preg_match($regex, $line, $matches)) {
      $forward_matches['db_rewrite_sql'] = $matches[1];
    }
  }
}

/**
 * Define the warning callbacks.
 */

function _coder_review_security_drupal_set_title_filter_warning() {
  return t('Potential problem: !drupal_set_title() only accepts filtered text, be sure to use !check_plain(), !filter_xss_admin() or similar to ensure your $variable is fully sanitized.',
    array(
      '!drupal_set_title' => theme('drupalapi', 'drupal_set_title'),
      '!check_plain' => theme('drupalapi', 'check_plain'),
      '!filter_xss_admin' => theme('drupalapi', 'filter_xss_admin'),
    )
  );
}

function _coder_review_security_drupal_set_title_filter_t_warning() {
  return t('Potential problem: !drupal_set_title() only accepts filtered text, be sure all !placeholders for $variables in !t() are fully sanitized using !check_plain(), !filter_xss_admin() or similar.',
    array(
      '!drupal_set_title' => theme('drupalapi', 'drupal_set_title'),
      '!t' => theme('drupalapi', 't'),
      '!check_plain' => theme('drupalapi', 'check_plain'),
      '!filter_xss_admin' => theme('drupalapi', 'filter_xss_admin'),
    )
  );
}

function _coder_review_security_drupal_set_message_filter_warning() {
  return t('Potential problem: !drupal_set_message() only accepts filtered text, be sure to use !check_plain(), !filter_xss_admin() or similar to ensure your $variable is fully sanitized.',
    array(
      '!drupal_set_message' => theme('drupalapi', 'drupal_set_message'),
      '!check_plain' => theme('drupalapi', 'check_plain'),
      '!filter_xss_admin' => theme('drupalapi', 'filter_xss_admin'),
    )
  );
}

function _coder_review_security_drupal_set_message_filter_t_warning() {
  return t('Potential problem: !drupal_set_message() only accepts filtered text, be sure all !placeholders for $variables in !t() are fully sanitized using !check_plain(), !filter_xss_admin() or similar.',
    array(
      '!drupal_set_message' => theme('drupalapi', 'drupal_set_message'),
      '!t' => theme('drupalapi', 't'),
      '!check_plain' => theme('drupalapi', 'check_plain'),
      '!filter_xss_admin' => theme('drupalapi', 'filter_xss_admin'),
    )
  );
}

function _coder_review_security_l_check_plain_warning() {
  return t('!l() already contains a !check_plain() call by default. See http://drupal.org/node/28984',
    array(
      '!l' => theme('drupalapi', array('function' => 'l')),
      '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
    )
  );
}

function _coder_review_security_request_uri_warning() {
  return t('the use of REQUEST_URI is prone to XSS exploits and does not work on IIS; use !request_uri() instead',
    array(
      '!request_uri' => theme('drupalapi', array('function' => 'request_uri')),
    )
  );
}

function _coder_review_security_sql_var_warning() {
  return array(
    '#warning' => t('In SQL strings, Use !db_query() placeholders in place of variables.  This is a potential source of SQL injection attacks when the variable can come from user data.',
      array(
        '!db_query' => theme('drupalapi', array('function' => 'db_query')),
      )
    ),
    '#link' => 'http://drupal.org/writing-secure-code',
    '#description' => t('Use %s and %d variable substitution.  When inserting an array of values use <code>$placeholders = implode(\',\', array_fill(0, count($args), "\'%s\'"));</code>'),
  );
}

function _coder_review_security_db_rewrite_sql() {
  return array(
    '#warning' => t('Potential problem: "SELECT FROM {node}" statements should probably be wrapped in !db_rewrite_sql and with the alias for {node} table defined (e.g. {node} n)',
      array(
        '!db_rewrite_sql' => theme('drupalapi', 'db_rewrite_sql'),
      )
    ),
  );
}

function _coder_review_security_menu_access_callback_warning() {
  return array(
    '#warning' => t("The value for the 'access callback' must always be a string which is the the name of the function - never a function call. It may also be assigned the value TRUE or FALSE if the callback is always (or never) accessible."),
    '#link' => 'http://drupal.org/node/109157',
  );
}

