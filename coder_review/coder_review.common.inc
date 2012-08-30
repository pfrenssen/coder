<?php
/**
 * @file
 */

define('SEVERITY_MINOR', 1);
define('SEVERITY_NORMAL', 5);
define('SEVERITY_CRITICAL', 9);

/**
 * Get all of the code review modules, including contributions.
 */
function _coder_review_reviews() {
  return function_exists('module_invoke_all') ? module_invoke_all('reviews') : coder_review_reviews();
}

/**
 * Implements hook_reviews().
 *
 * Returns all of the reviews implemented by the coder module.
 */
function coder_review_reviews() {
  static $cache = array();
  if (!$cache) {
    $path = _coder_review_file_path() . '/includes';
    foreach (scandir($path) as $file) {
      $pathinfo = pathinfo("$path/$file");
      if ($pathinfo['extension'] == 'inc') {
        require_once "$path/$file";
        $function = $pathinfo['filename'] . '_reviews';
        if (function_exists($function)) {
          $review = $function();
          if ($review) {
            foreach (array_keys($review) as $review_name) {
              $review[$review_name]['#file'] = "$path/$file";
            }
            $cache += $review;
          }
        }
      }
    }
  }
  return $cache;
}

/**
 * Helper functions for settings form.
 */
function _coder_review_default_reviews() {
  // Don't use drupal_map_assoc() so that this will run without Drupal bootstraped.
  return array(
    'style' => 'style',
    'sql' => 'sql',
    'comment' => 'comment',
    'security' => 'security',
    'i18n' => 'i18n',
  );
}

/**
 * Return the extensions used by the reviews, that aren't part of the default extensions.
 */
function _coder_review_get_reviews_extensions($defaults, $reviews) {
  $extensions = array();
  foreach ($reviews as $key => $review) {
    foreach ($review['#rules'] as $rule) {
      if (isset($rule['#filename'])) {
        foreach ($rule['#filename'] as $ext) {
          if (!in_array($ext, $defaults) && !in_array($ext, $extensions)) {
            $extensions[] = $ext;
          }
        }
      }
    }
  }
  return $extensions;
}

/**
 * Return last modification timestamp of coder_review and all of its dependencies.
 */
function _coder_review_modified() {
  static $cached_mtime;
  if (!isset($cached_mtime)) {
    $cached_mtime = filemtime(_coder_review_file_path() . '/coder_review.module');

    foreach (coder_review_reviews() as $review) {
      $mtime = filemtime($review['#file']);
      if ($mtime > $cached_mtime) {
        $cached_mtime = $mtime;
      }
    }
  }
  return $cached_mtime;
}

/**
 * Perform batch coder reviews for multiple files.
 *
 * @param $coder_args
 *   Array of coder arguments, valid arguments are:
 *     - '#reviews' => array list of reviews to perform, see _coder_review_reviews();
 *     - '#severity' => integer magic number, see constants SEVERITY_*;
 *     - '#filename' => string filename to check,
 *     - '#patch' => string patch lines to check,
 *
 * @return
 *   Array of results, in form:
 *     - '#stats' => Array with error counts for all severities, in form
 *         'minor' => integer count, 'normal' => integer count;
 *         'critical' => integer count;
 *     - integer ID => HTML error for display.
 */
function do_coder_reviews($coder_args) {
  // Load the cached results if they exist, but not for patches.
  if (empty($coder_args['#patch']) && empty($coder_args['#test']) && $coder_args['#cache']) {
    $cache_key = 'coder:' . md5(implode(':', array_keys($coder_args['#reviews']))) . $coder_args['#severity'] . ':' . $coder_args['#filename'];
    if (_drush()) {
      if (drush_get_option('checkstyle')) {
        $cache_key .= ':drushcheckstyle';
      }
      elseif (drush_get_option('xml')) {
        $cache_key .= ':drushxml';
      }
    }
    $filepath = realpath($coder_args['#filename']);
    if (file_exists($filepath)) {
      $cache_mtime = filemtime($filepath);
      $cache_results = _cache_get($cache_key, 'cache_coder');
      if ($cache_results && $cache_results->data['mtime'] == $cache_mtime && _coder_review_modified() < $cache_results->created) {
        return $cache_results->data['results'];
      }
    }
  }

  $results['#stats'] = array('minor' => 0, 'normal' => 0, 'critical' => 0, 'ignored' => 0);

  // Skip PHP include files when the user requested severity is above minor.
  if (isset($coder_args['#php_minor']) && _substr($coder_args['#filename'], -4) == '.php') {
    if ($coder_args['#severity'] > 1) {
      return $results;
    }
  }

  // Read the file.
  if (_coder_review_read_and_parse_file($coder_args)) {
    // Do all of the code reviews.
    $errors = array();
    foreach ($coder_args['#reviews'] as $review_name => $review) {
      $review['#review_name'] = $review_name;
      $result = do_coder_review($coder_args, $review);
      if ($result) {
        // ignored isn't a severity level, but is a stat we need to track.
        foreach (array('critical', 'normal', 'minor', 'ignored') as $severity_level) {
          if (isset($result['#stats'][$severity_level])) {
            $results['#stats'][$severity_level] += $result['#stats'][$severity_level];
          }
        }
        $errors += $result;
      }
    }

    // Theme the error messages.
    foreach ($errors as $key => $error) {
      if (is_numeric($key)) {
        $error['warning'] = _coder_review_warning($error['rule']);
        $results[$key] = theme_coder_review_warning_msg(array('error' => $error));
      }
    }

    // Sort the results.
    ksort($results, SORT_NUMERIC);
  }
  else {
    $results[] = theme('coder_review_warning', array('warning' => _t('Could not read the file'), 'severity_name' => 'critical'));
  }

  // Save the results in the cache if we're not reviewing a patch.
  if (empty($coder_args['#patch']) && empty($coder_args['#test']) && $coder_args['#cache'] && isset($cache_mtime)) {
    $cache_results = array(
      'mtime' => $cache_mtime,
      'results' => $results,
    );
    _cache_set($cache_key, $cache_results, 'cache_coder');
  }

  return $results;
}

/**
 * Format a coder_review warning to be included in results, creating the text.
 *
 * @param $results
 *   Results array variable to save errors to.
 * @param $error
 *   Error array from _coder_review_error().
 */
function theme_coder_review_warning_msg($variables) {
  $error = $variables['error'];
  $error['warning'] = _coder_review_warning($error['rule']);
  return theme_coder_review_warning($error);
}

/**
 * Format a coder_review warning to be included in results.
 *
 * @param $warning
 *   Either summary warning description, or an array in format:
 *     - '#warning' => Summary warning description;
 *     - '#description' => Detailed warning description;
 *     - '#link' => Link to an explanatory document.
 * @param $severity_name
 *   String severity name.
 * @param $lineno
 *   Integer line number of error.
 * @param $line
 *   String contents of line.
 */
function theme_coder_review_warning($variables) {
  // Theme the output for the Drupal shell.
  if (_drush()) {
    return theme_drush_coder_review_warning($variables);
  }

  $warning = $variables['warning'];
  $severity_name = $variables['severity_name'];
  $lineno = $variables['lineno'];
  $line = $variables['line'];

  // Extract description from warning.
  if (is_array($warning)) {
    $description = isset($warning['#description']) ? _t($warning['#description']) : '';
    if (isset($warning['#warning'])) {
      $warning_msg = _t($warning['#warning']);
    }
    elseif (isset($warning['#text'])) {
      $warning += array('#args' => array());
      $warning_msg = _t($warning['#text'], $warning['#args']);
    }
    if (isset($warning['#link'])) {
      $warning_msg .= ' (' . l(_t('Drupal Docs'), $warning['#link']) . ')';
    }
    $warning = $warning_msg;
  }
  if ($lineno) {
    if ($lineno > 0) {
      $warning = _t('Line @number: !warning', array('@number' => $lineno, '!warning' => $warning));
    }
    else {
      $warning = _t('File: !warning', array('!warning' => $warning));
    }
    if ($line) {
      $warning .= '<pre>' . check_plain($line) . '</pre>';
    }
  }
  $class = 'coder-warning';
  if ($severity_name) {
    $class .= " coder-$severity_name";
  }
  $path = _coder_review_file_path() . '/..';
  $title = _t('severity: @severity', array('@severity' => $severity_name));
  $img = theme('image', array(
    'path' => $path . "/images/$severity_name.png",
    'alt' => $title, 'title' => $title,
    'attributes' => array('align' => 'right', 'class' => 'coder'),
    'getsize' => FALSE,
  ));
  if (!empty($description)) {
    $img .= theme('image', array('path' => $path . '/images/more.png', 'alt' => t('click to read more'), 'atributes' => array('align' => 'right', 'class' => 'coder-more'), 'getsize' => FALSE));
    $warning .= '<div class="coder-description">' . t('Explanation: @description', array('@description' => $description)) . '</div>';
  }
  return '<div class="' . $class . '">' . $img . $warning . '</div>';
}

/**
 * Parse and read a file into a format easy to validate.
 *
 * @param $coder_args
 *   Coder arguments array variable to add file lines of code (with
 *   trailing newlines. The following array indices are added:
 *     '#all_array_lines', '#php_array_lines', '#allphp_array_lines',
 *     '#html_array_lines', '#quote_array_lines', '#doublequote_array_lines',
 *     '#comment_array_lines', and #all_lines.
 *   The _array_ variants are multidimensional arrays, the first index for
 *   the line number, and the second index for each occurance within the line.
 *   #all_lines is a simple array, with each line from the file as an index.
 *
 * @return
 *   Integer 1 if success.
 */
function _coder_review_read_and_parse_file(&$coder_args) {
  $regex = '/(' . implode('|', array_merge(array('module'), $coder_args['#php_extensions'])) . ')$/';
  $is_php_file = preg_match($regex, $coder_args['#filename']);

  // Get the path to the module file.
  $filepath = realpath($coder_args['#filename']);
  if (!empty($coder_args['#patch']) || !empty($coder_args['#test']) || file_exists($filepath)) {
    $in_php = ($is_php_file ? 0 : 1);
    $in_allphp = ($is_php_file ? 0 : 1);
    $in_comment = 0;

    if (!empty($coder_args['#patch'])) {
      $content = $coder_args['#patch'];
      if (preg_match('/^\s*\*/', $content)) {
        $in_comment = '*';
      }
      else {
        $content = preg_replace('/^(function\s.*?(\r\n|\n)+)(\s*\*)/', '${1}/*', $content);
        $in_php = 1;
        $in_allphp = 1;
      }
    }
    elseif (!empty($coder_args['#test'])) {
      $content = $coder_args['#test'];
      $in_php = 1;
      $in_allphp = 1;
    }
    else {
      $content = file_get_contents($filepath) . "\n";
    }
    $content_length = _strlen($content);

    $in_comment = 0;
    $beginning_of_line = 0;
    $in_quote_html = 0;
    $in_backslash = 0;
    $in_quote = 0;
    $in_heredoc = 0;
    $in_heredoc_html = '';
    $heredoc = '';
    $all_lines = array();
    $full_lines = array();
    $php_lines = array();
    $allphp_lines = array();
    $html_lines = array();
    $quote_lines = array();
    $doublequote_lines = array();
    $comment_lines = array();
    $this_all_lines = '';
    $this_php_lines = '';
    $this_allphp_lines = '';
    $this_html_lines = '';
    $this_quote_lines = array('');
    $this_quote_index = -1;
    $this_quote_sep = FALSE;
    $this_doublequote_lines = array('');
    $this_doublequote_index = -1;
    $this_comment_lines = '';

    // Parse the file:
    // - Strip comments,
    // - Strip quote content,
    // - Strip stuff not in php,
    // - Break into lines.
    $lineno = 1;
    for ($pos = 0; $pos < $content_length; ++$pos) {
      // Get the current character.
      $char = $content[$pos];
      if ($char == "\n") {
        if ($in_comment === '/') { // End C++ style comments on newline.
          $in_comment = 0;
        }

        // Assume that html inside quotes doesn't span newlines.
        $in_quote_html = 0;

        // Remove coder's simpletests assertions as they validly contain bad
        // code, for testing the review rules.
        if (preg_match('/assertCoderReview(Fail|Pass)/', $this_all_lines)) {
          $lineno++;
          $this_all_lines = '';
          $this_php_lines = '';
          $this_allphp_lines = '';
          $this_html_lines = '';
          $this_comment_lines = '';
          $this_quote_lines = array('');
          continue;
        }

        // Remove blank lines now, so we avoid processing them over-and-over.
        if ($this_all_lines != '') {
          if (trim($this_all_lines, "\r\n") != '') {
            $all_lines[$lineno] = array($this_all_lines);
            $full_lines[$lineno] = $this_all_lines;
          }
          if (trim($this_php_lines, "\r\n") != '') {
            $php_lines[$lineno] = array($this_php_lines);
          }
          if (trim($this_allphp_lines, "\r\n") != '') {
            $allphp_lines[$lineno] = array($this_allphp_lines);
          }
          if (trim($this_html_lines, "\r\n") != '') {
            $html_lines[$lineno] = array($this_html_lines);
          }
          $quotes = array();
          foreach ($this_quote_lines as $quote_line) {
            if (trim($quote_line, "\r\n") != '') {
              $quotes[] = $quote_line;
            }
          }
          if ($quotes) {
            $quote_lines[$lineno] = $quotes;
          }
          $quotes = array();
          foreach ($this_doublequote_lines as $quote_line) {
            if (trim($quote_line, "\r\n") != '') {
              $quotes[] = $quote_line;
            }
          }
          if ($quotes) {
            $doublequote_lines[$lineno] = $quotes;
          }
          if (trim($this_comment_lines, "\r\n") != '') {
            $comment_lines[$lineno] = array($this_comment_lines);
          }
        }

        // Save this line and start a new line.
        $lineno++;
        $this_all_lines = '';
        $this_php_lines = '';
        $this_allphp_lines = '';
        $this_html_lines = '';
        $this_quote_lines = array('');
        $this_doublequote_lines = array('');
        $this_quote_index = -1;
        $this_quote_sep = FALSE;
        $this_doublequote_index = -1;
        $this_comment_lines = '';
        $beginning_of_line = 1;
        continue;
      }
      if ($this_all_lines != '') {
        $beginning_of_line = 0;
      }
      $this_all_lines .= $char;

      if ($in_php || $in_allphp) {
        // When in a quoted string, look for the trailing quote
        // strip characters in the string, replacing with '' or "".
        if ($in_quote) {
          if ($in_backslash) {
            $in_backslash = 0;
          }
          elseif ($char == '\\') {
            $in_backslash = 1;
          }
          elseif ($char == $in_quote && !$in_backslash) {
            $in_quote = 0;
          }
          elseif ($char == '<') {
            $in_quote_html = '>';
          }
          if ($in_quote) {
            if ($this_quote_index == -1) {
              $this_quote_index = 0;
            }
            $this_quote_lines[$this_quote_index] .= $char;
            if ($in_quote == '"') {
              if ($this_doublequote_index == -1) {
                $this_doublequote_index = 0;
              }
              $this_doublequote_lines[$this_doublequote_index] .= $char;
            }
            if ($in_quote_html) {
              $this_html_lines .= $char;
            }
          }
          if ($char == $in_quote_html) {
            $in_quote_html = 0;
          }
          $this_allphp_lines .= $char;
          // @note: Trailing char output with starting one.
          unset($char);
        }

        elseif ($in_heredoc) {
          // @note: drupal_substr does not properly handle multi-byte characters in this string.
          // @todo: check other places where the drupal_ string functions fail.
          if ($beginning_of_line && $char == $in_heredoc[0] && substr($content, $pos, $in_heredoc_length) == $in_heredoc) {
            $this_all_lines .= _substr($content, $pos + 1, $in_heredoc_length - 1);
            $in_heredoc = 0;
            $pos += $in_heredoc_length;
          }
          elseif ($char == '<') {
            $in_heredoc_html = '>';
          }
          if ($in_heredoc && $in_heredoc_html) {
            $this_html_lines .= $char;
          }
          if ($in_heredoc_html && $char == $in_heredoc_html) {
            $in_heredoc_html = '';
          }
          unset($char);
        }

        // Look for the ending php tag.
        elseif ($char == '?' && $content[$pos + 1] == '>' && $in_comment !== '*') {
          unset($char);
          $in_php = 0;
          $in_allphp = 0;
          $this_all_lines .= '>';
          ++$pos;
        }

        // When in a comment look for the trailing comment.
        elseif ($in_comment) {
          $this_comment_lines .= $char;
          if ($in_comment == '*' && $char == '*' && $content[$pos + 1] == '/') {
            $in_comment = 0;
            $this_all_lines .= '/';
            $this_comment_lines .= '/';
            ++$pos;
          }
          unset($char); // Don't add comments to php output.
        }

        else {
          switch ($char) {
            case ',':
            case ')':
            case '(':
            case '>': // For 'foo' => 'bar' type syntax.
            case ':':
              // Look for separators which force a new quote string.
              if ($this_quote_index < 0 || !empty($this_quote_lines[$this_quote_index])) {
                $this_quote_sep = TRUE;
              }
              break;

            case '\'':
            case '"':
              if ($content[$pos - 1] != '\\') {
                $this_php_lines .= $char;
                $in_quote = $char;
                if ($this_quote_sep) {
                  $this_quote_lines[++ $this_quote_index] = '';
                  if ($char == '"') {
                    $this_doublequote_lines[++ $this_doublequote_index] = '';
                  }
                }
                $this_quote_sep = FALSE;
              }
              break;

            case '/':
              $next_char = $content[$pos + 1];
              if ($next_char == '/' || $next_char == '*') {
                unset($char);
                $in_comment = $next_char;
                $this_all_lines .= $next_char;
                $this_comment_lines .= '/' . $next_char;
                ++$pos;
              }
              break;

            case '<':
              if ($content[$pos + 1] == '<' && $content[$pos + 2] == '<') {
                unset($char);
                $this_all_lines .= '<<';

                // Get the heredoc word.
                // Read until the end-of-line.
                for ($pos += 3; $pos < $content_length; ++$pos) {
                  $char = $content[$pos];
                  if ($char == "\n") {
                    $pos --;
                    if (preg_match('/^\s*(\w+)/', $heredoc, $match)) {
                      $in_heredoc = $match[1];
                      $in_heredoc_length = _strlen($in_heredoc);
                    }
                    break;
                  }
                  $this_all_lines .= $char;
                  $heredoc .= $char;
                }
                $heredoc = '';

                // Replace heredoc's with an empty string.
                $this_php_lines .= '\'\'';
                $this_allphp_lines .= '\'\'';
                unset($char);
              }
              break;
          }
        }
        if (isset($char)) {
          $this_php_lines .= $char;
          $this_allphp_lines .= $char;
        }
      }
      else {
        switch ($char) {
          case '<':
            if ($content[$pos + 1] == '?') {
              if ($content[$pos + 2] == ' ') {
                $in_php = 1;
                $in_allphp = 1;
                $this_all_lines .= '? ';
                $pos += 2;
              }
              elseif (_substr($content, $pos + 2, 3) == 'php') {
                $in_php = 1;
                $in_allphp = 1;
                $this_all_lines .= '?php';
                $pos += 4;
              }
              break;
            }
            // FALLTHROUGH
          default:
            $this_html_lines .= $char;
            break;
        }
      }
    }

    if (trim($this_all_lines) != '') {
      $all_lines[$lineno] = array($this_all_lines);
      $full_lines[$lineno] = $this_all_lines;
    }
    if (trim($this_php_lines) != '') {
      $php_lines[$lineno] = array($this_php_lines);
    }
    if (trim($this_allphp_lines) != '') {
      $allphp_lines[$lineno] = array($this_allphp_lines);
    }
    if (trim($this_html_lines) != '') {
      $html_lines[$lineno] = array($this_html_lines);
    }
    $quotes = array();
    foreach ($this_quote_lines as $quote_line) {
      if (trim($quote_line, "\r\n") != '') {
        $quotes[] = $quote_line;
      }
    }
    if ($quotes) {
      $quote_lines[$lineno] = $quotes;
    }
    $quotes = array();
    foreach ($this_doublequote_lines as $quote_line) {
      if (trim($quote_line, "\r\n") != '') {
        $quotes[] = $quote_line;
      }
    }
    if ($quotes) {
      $doublequote_lines[$lineno] = $quotes;
    }
    if (trim($this_comment_lines) != '') {
      $comment_lines[$lineno] = array($this_comment_lines);
    }

    // Add the files lines to the arguments.
    $coder_args['#all_array_lines'] = $all_lines;
    $coder_args['#php_array_lines'] = $php_lines;
    $coder_args['#allphp_array_lines'] = $allphp_lines;
    $coder_args['#html_array_lines'] = $html_lines;
    $coder_args['#quote_array_lines'] = $quote_lines;
    $coder_args['#doublequote_array_lines'] = $doublequote_lines;
    $coder_args['#comment_array_lines'] = $comment_lines;
    $coder_args['#all_lines'] = $full_lines;

    // Given the sanitized PHP lines, determine the class and function for each line.
    $stack = array();
    $class_stack = $class_stack_paren = array();
    $function_stack = $function_stack_paren = array();
    $paren_depth = 0;
    foreach ($php_lines as $lineno => $line_array) {
      foreach ($line_array as $line) {
        // Check if this line is the beginning of a function definition.
        if (preg_match('/function (\w+)\s*\(/', $line, $match) && !preg_match('/;/', $line)) {
          array_unshift($function_stack, $match[1]);
          array_unshift($function_stack_paren, $paren_depth);
//print "$lineno: " . str_repeat(' ', $function_stack_paren[0]) . "function $function_stack[0] {\n";
        }

        // Check if this line is the beginning of a class definition.
        if (preg_match('/class (\w+)/', $line, $match) || preg_match('/interface (\w+)/', $line, $match)) {
          array_unshift($class_stack, $match[1]);
          array_unshift($class_stack_paren, $paren_depth);
//print "$lineno: " . str_repeat(' ', $classstack_paren[0]) . "class $class_stack[0] {\n";
        }

        // Check if this line changes the parenthesis depth.
        if (preg_match_all('/([{}])/', $line, $match)) {
          foreach ($match[0] as $paren_match) {
            $paren_depth += ($paren_match == '{') ? 1 : -1;
          }

          // If the depth is now less than then current function depth, pop the function from the stack.
          if ($function_stack_paren && $paren_depth <= $function_stack_paren[0]) {
//print "$lineno: " . str_repeat(' ', $function_stack_paren[0]) . "} // function $function_stack[0] {\n";
            array_shift($function_stack);
            array_shift($function_stack_paren);
          }

          // If the depth is now less than the current class depth, pop the class from the stack.
          if ($class_stack_paren && $paren_depth <= $class_stack_paren[0]) {
//print "$lineno: " . str_repeat(' ', $class_stack_paren[0]) . "} // class $class_stack[0] {\n";
            array_shift($class_stack);
            array_shift($class_stack_paren);
          }
        }

        // Cache the current function and class for each line of each file.
        $stack[$lineno] = array($class_stack ? $class_stack[0] : '', $function_stack ? $function_stack[0] : '');
      }
    }
    $coder_args['#stack'] = $stack;
    return 1;
  }
}

/**
 * Return the integer severity magic number for a string severity.
 *
 * @param $severity_name
 *   String severity name 'minor', 'normal', or 'critical'.
 * @param $default_value
 *   Integer magic number to use if severity string is not recognized.
 *
 * @return
 *   Integer magic number, see SEVERITY_* constants.
 */
function _coder_review_severity($severity_name, $default_value = SEVERITY_NORMAL) {
  // @note: Implemented this way in hopes that it is faster than a PHP switch.
  static $severity_names = array();
  if (!$severity_names) {
    $severity_names = array(
      'minor' => SEVERITY_MINOR,
      'normal' => SEVERITY_NORMAL,
      'critical' => SEVERITY_CRITICAL,
    );
  }
  return isset($severity_names[$severity_name]) ? $severity_names[$severity_name] : $default_value;
}

/**
 * Return string severity for a given error.
 *
 * @param $coder_args
 *   Coder settings array, see do_coder_reviews().
 * @param $review
 *   Review array, see hook_reviews(), contains rule arrays.
 * @param $rule
 *   Rule array that was triggered, see individual entries from hook_reviews().
 *
 * @return
 *   String severity of error.
 */
function _coder_review_severity_name($coder_args, $review, $rule) {
  // NOTE: Warnings in php includes are suspicious because
  // PHP includes are frequently 3rd party products.
  if (isset($coder_args['#php_minor']) && _substr($coder_args['#filename'], -4) == '.php') {
    return 'minor';
  }

  // Get the severity as defined by the rule.
  if (isset($rule['#severity'])) {
    return $rule['#severity'];
  }

  // If it's not defined in the rule, then it can be defined by the review.
  if (isset($review['#severity'])) {
    return $review['#severity'];
  }

  // Use the default.
  return 'normal';
}

/**
 * Perform code review for a review array.
 *
 * @param $coder_args
 *   Array coder_review settings, must have been prepared with _coder_review_read_and_parse_file(),
 *   see do_coder_reviews() for format.
 * @param $review
 *   Review array, see hook_review().
 *
 * @return
 *   Array results, see do_coder_reviews() return value for format.
 */
function do_coder_review($coder_args, $review) {
  $results = array('#stats' => array('minor' => 0, 'normal' => 0, 'critical' => 0, 'ignored' => 0));
  $allowed_extensions = array_merge(
    $coder_args['#php_extensions'],
    $coder_args['#include_extensions'],
    array('module')
  );
  if ($review['#rules']) {
    // Get the review's severity, used when the rule severity is not defined.
    $default_severity = isset($review['#severity']) ? _coder_review_severity($review['#severity']) : SEVERITY_NORMAL;

    // Get filename of current file.
    $filename = empty($coder_args['#patch']) ? $coder_args['#filename'] : 'coder_review.patch';
    $is_patch = 0;
    // Handle special case filename for patch files, if available.
    if (!empty($coder_args['#patch']) && preg_match('/(.+?):/', $coder_args['#filename'], $matches)) {
      $filename = empty($matches) ? 'coder_review.patch' : $matches[1];
      $is_patch = 1;
    }

    foreach ($review['#rules'] as $rule) {
      // Ignore rules that don't match the file extension.
      $filename_includes = isset($rule['#filename']) ? $rule['#filename'] : (isset($rule['#filename-not']) ? $coder_args['#php_extensions'] : NULL);
      if ($filename_includes) {
        $regex = '/(' . implode('|', $filename_includes) . ')$/i';
        if (!preg_match($regex, $filename, $matches)) {
          continue;
        }
      }

      // Ignore rules that do match the file extension, javascript files are excluded by default.
      $not = isset($rule['#filename-not']) ? $rule['#filename-not'] : (isset($rule['#filename']) ? NULL : $coder_args['#include_extensions']);
      if ($not) {
        $regex = '/(' . implode('|', $not) . ')$/i';
        // Check filename. If a patch, also check the .patch extension.
        if (preg_match($regex, $filename, $matches) || ($is_patch && preg_match($regex, 'coder_review.patch', $matches))) {
          continue;
        }
      }

      // If it's a patch, it can contain any sort of file, so ensure the file
      // being processed is either PHP or one of the supported extensions.
      if ($is_patch) {
        $regex = '/(' . implode('|', $allowed_extensions) . ')$/i';
        if (!preg_match($regex, $filename, $matches)) {
          continue;
        }
      }

      // Perform the review if above the user requested severity.
      $severity = _coder_review_severity(isset($rule['#severity']) ? $rule['#severity'] : '', $default_severity);
      if ($severity >= $coder_args['#severity']) {
        if (isset($rule['#source'])) { // Values: all, html, comment, allphp or php.
          $source = '#' . $rule['#source'] . '_array_lines';
          $lines = isset($coder_args[$source]) ? $coder_args[$source] : array();
        }
        else {
          $lines = isset($coder_args['#php_array_lines']) ? $coder_args['#php_array_lines'] : array();
        }
        switch ($rule['#type']) {
          case 'regex':
            do_coder_review_regex($coder_args, $review, $rule, $lines, $results);
            break;
          case 'grep':
            do_coder_review_grep($coder_args, $review, $rule, $lines, $results);
            break;
          case 'grep_invert':
            do_coder_review_grep_invert($coder_args, $review, $rule, $lines, $results);
            break;
          case 'callback':
            do_coder_review_callback($coder_args, $review, $rule, $lines, $results);
            break;
          // @todo:
          case 'token':
            break;
        }
      }
    }
  }
  return $results;
}

/**
 * Implements do_coder_review_* for regex match.
 *
 * @param $coder_args
 *   Coder settings array variable, see do_coder_review() for format.
 * @param $review
 *   Review array the current rule belongs to, used by _coder_review_severity_name().
 * @param $rule
 *   Rule array being checked.
 * @param $lines
 *   Pertinent source file lines according to rule's '#source' value.
 * @param $results
 *   Results array variable to save errors to.
 */
function do_coder_review_regex(&$coder_args, $review, $rule, $lines, &$results) {
  if (isset($rule['#value']) && !empty($lines)) {
    $regexflags = isset($rule['#case-sensitive']) ? '' : 'i';
    $regex = '/' . $rule['#value'] . '/' . $regexflags;
    $class_regex = isset($rule['#class']) ? '/' . $rule['#class'] . '/' : '';
    $class_not_regex = isset($rule['#class-not']) ? '/' . $rule['#class-not'] . '/' : '';
    $function_regex = isset($rule['#function']) ? '/' . $rule['#function'] . '/' : '';
    $function_not_regex = isset($rule['#function-not']) ? '/' . $rule['#function-not'] . '/' : '';
    $not_regex = isset($rule['#not']) ? '/' . $rule['#not'] . '/' . $regexflags : '';
    $never_regex = isset($rule['#never']) ? '/' . $rule['#never'] . '/' . $regexflags : '';

    foreach ($lines as $lineno => $line_array) {
      foreach ($line_array as $line) {
        if (preg_match($regex, $line, $matches)) {
          // Some rules apply only within certain functions and classes.
          list($class_current, $function_current) = isset($coder_args['#stack'][$lineno]) ? $coder_args['#stack'][$lineno] : array('', '');
          if (   ($function_regex && (!$function_current || !preg_match($function_regex, $function_current)))
              || ($function_not_regex && $function_current && preg_match($function_not_regex, $function_current))
              || ($class_regex && (!$class_current || !preg_match($class_regex, $class_current[0])))
              || ($class_not_regex && $class_current && preg_match($class_not_regex, $class_current[0]))
            ) {
            continue;
          }

          // Don't match some regex's.
          if ($not_regex) {
            foreach ($matches as $match) {
              if (preg_match($not_regex, $match)) {
                continue 2;
              }
            }
          }
          if ($never_regex) {
            if (preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
              continue;
            }
          }

          $line = $coder_args['#all_lines'][$lineno];
          $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
          if (!isset($coder_args['#ignore_lines'][$review['#review_name']])) {
            $coder_args['#ignore_lines'][$review['#review_name']] = array();
          }
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $coder_args['#ignore_lines'][$review['#review_name']]);
        }
      }
    }
  }
}

/**
 * Builds an error message based on the rule that failed and other information.
 *
 * @param $results
 *   Results array variable to save errors to.
 * @param $rule
 *   Rule array that triggered the error.
 * @param $severity_name
 *   String severity of error as detected by _coder_review_severity_name().
 * @param $lineno
 *   Line number of error.
 * @param $line
 *   Contents of line that triggered error.
 * @param $original
 *   Deprecated.
 */
function _coder_review_error(&$results, $rule, $severity_name, $lineno = -1, $line = '', $ignores = array()) {
  // Note: The use of the $key allows multiple errors on one line.
  // This assumes that no line of source has more than 10000 lines of code
  // and that we have fewer than 10000 errors.
  global $_coder_errno;
  // Skip warnings we've been told to ignore.
  if (is_array($ignores) && in_array($lineno, $ignores)) {
    $results['#stats']['ignored']++;
  }
  else {
    $key = ($lineno + 1) * 10000 + ($_coder_errno++);
    $results[$key] = array('rule' => $rule, 'severity_name' => $severity_name, 'lineno' => $lineno, 'line' => $line);
    ++$results['#stats'][$severity_name];
  }
}

/**
 * Search for a string.
 *
 * @see do_coder_review_regex()
 */
function do_coder_review_grep(&$coder_args, $review, $rule, $lines, &$results) {
  if (isset($rule['#value'])) {
    foreach ($lines as $lineno => $line_array) {
      foreach ($line_array as $line) {
        if (_coder_review_search_string($line, $rule)) {
          $line = $coder_args['#all_lines'][$lineno];
          $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
          if (!isset($coder_args['#ignore_lines'][$review['#review_name']])) {
            $coder_args['#ignore_lines'][$review['#review_name']] = array();
          }
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $coder_args['#ignore_lines'][$review['#review_name']]);
        }
      }
    }
  }
}

/**
 * Search for potentially missing string.
 *
 * @see do_coder_review_regex()
 */
function do_coder_review_grep_invert(&$coder_args, $review, $rule, $lines, &$results) {
  if (isset($rule['#value'])) {
    foreach ($lines as $lineno => $line_array) {
      foreach ($line_array as $line) {
        if (_coder_review_search_string($line, $rule)) {
          return;
        }
      }
    }
    $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
    _coder_review_error($results, $rule, $severity_name);
  }
}

/**
 * Allow for an arbitrary callback function to perform a review.
 *
 * @see do_coder_review_regex()
 */
function do_coder_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $function = $rule['#value'];
  if ($function && function_exists($function)) {
    call_user_func_array($function, array(&$coder_args, $review, $rule, $lines, &$results));
  }
}

/**
 * Search for a string.
 *
 * Uses the fastest available php function for searching.
 *
 * @param $line
 *   Haystack.
 * @param $rule
 *   Rule to process.
 *
 * @return
 *   TRUE if needle is in haystack.
 */
function _coder_review_search_string($line, $rule) {
  // Case-sensitive search with strpos() (supported everywhere).
  if (isset($rule['#case-sensitive'])) {
    return strpos($line, $rule['#value']) !== FALSE;
  }

  // Case-insensitive search with stripos().
  if (!isset($rule['#case-sensitive'])) {
    return stripos($line, $rule['#value']) !== FALSE;
  }

  // Case-insensitive search.
  $regex = '/' . preg_quote($rule['#value']) . '/i';
  return preg_match($regex, $line);
}

/**
 * Returns a active settings array for coder_review.
 *
 * The name is a misnomer, but is a largely correct characterization
 * for most of Coder's settings as the variables usually do not exist.
 *
 * @param $args
 *   String settings argument, can be 'settings', 'active', 'core', 'all'
 *   and 'default'.
 *
 * @return
 *   Associative array of settings in form setting name => setting value.
 */
function _coder_review_get_default_settings($arg = 'default') {
  $settings = array(
    'coder_includes' => 0,
    'coder_includes_exclusions' => '',
    'coder_active_modules' => 0,
    'coder_core' => 0,
    'coder_all' => 0,
    'coder_contrib' => 0,
    'coder_files' => 0,
    'coder_file_list' => '',
    'coder_patches' => 0,
    'coder_modules' => array(),
    'coder_themes' => array(),
  );
  $settings['coder_reviews'] = _variable_get('coder_reviews', _coder_review_default_reviews());
  $settings['coder_severity'] = _variable_get('coder_severity', SEVERITY_NORMAL);

  // Determine any options based on the passed in URL.
  switch ($arg) {
    case 'settings':
      $settings['coder_includes'] = 1;
      break;

    case 'active':
      $settings['coder_active_modules'] = 1;
      break;

    case 'core':
      $settings['coder_core'] = 1;
      $settings['coder_includes'] = 1;
      break;

    case 'all':
      $settings['coder_core'] = 1;
      $settings['coder_includes'] = 1;
      $settings['coder_all'] = 1;
      break;

    case 'contrib':
      $settings['coder_includes'] = 1;
      $settings['coder_contrib'] = 1;
      break;

    case 'files':
      $settings['coder_files'] = 1;
      break;

    case 'patches':
      $settings['coder_patches'] = 1;
      break;

    case 'default':
      $settings['coder_active_modules'] = _variable_get('coder_active_modules', 1);
      $settings['coder_core'] = _variable_get('coder_core', 0);
      $settings['coder_includes'] = _variable_get('coder_includes', 0);
      $settings['coder_includes_exclusions'] = _variable_get('coder_includes_exclusions', '');
      $settings['coder_modules'] = _variable_get('coder_modules', array());
      $settings['coder_themes'] = _variable_get('coder_themes', array());
      break;

    default:
      $settings['coder_includes'] = 1;
      $settings['coder_includes_exclusions'] = _variable_get('coder_includes_exclusions', '');
      // TODO: Does this need to go into coder_review_themes sometimes?
      $settings['coder_modules'] = array($arg => $arg);
      break;
  }
  return $settings;
}

/**
 * Return true if $module is in Drupal core.
 */
function _coder_review_is_drupal_core($system) {
  $info_file = dirname(realpath($system->filename)) . '/' . $system->name . '.info';
  $info = drupal_parse_info_file($info_file);
  return !empty($info['package']) && strtolower($info['package']) == 'core';
}

/**
 * Return the formatted warning message from the $rule.
 */
function _coder_review_warning($rule) {
  // @todo: add version check so we handle tranlations right for older rules definitions.
  // ... or should we just ignore them?
  // This will require passing the review to this function.

  // Call warning callbacks.
  if (isset($rule['#warning_callback'])) {
    // This rule definition is deprecated.
    if (is_callable($rule['#warning_callback']) || function_exists($rule['#warning_callback'])) {
      return $rule['#warning_callback']();
    }
  }
  elseif (isset($rule['#warning'])) {
    // Return array warnings as-is.
    // They get translated in theme_coder_review_warning().
    if (is_array($rule['#warning'])) {
      return $rule['#warning'];
    }

    // Warnings callback functions can now be stored as the #warning element.
    if (is_callable($rule['#warning']) || function_exists($rule['#warning'])) {
      return $rule['#warning']();
    }

    // Translate the warning message and return it.
    return _t($rule['#warning']);
  }

  // No valid warning definition, return a warning about the warning.
  return _t('Unknown warning');
}

/**
 * Wrapper t() function that works in Drush cli.
 */
function _t($string, $args = array(), $options = array()) {
  return _drush() ? dt($string, $args) : t($string, $args, $options);
}

/**
 * Wrapper l() function that works in Drush cli.
 */
function _l($text, $path, $options = array()) {
  return function_exists('l') ? l($text, $path, $options) : "$text $path";
}

/**
 * Wrapper variable_get() function that works in Drush cli.
 */
function _variable_get($var, $default = NULL) {
  return function_exists('variable_get') ? variable_get($var, $default) : $default;
}

/**
 * Return TRUE if running the Drush client.
 */
function _drush() {
  return function_exists('drush_verify_cli') && drush_verify_cli();
}

/**
 * Return a link to the Drupal node.
 *
 * @param $nid
 *   Nid to link to.
 * @param $anchor
 *   Optional anchor.
 */
function _drupalnode($nid, $anchor = '') {
  $link = "http://drupal.org/node/$nid";
  if ($anchor) {
    $link .= "#$anchor";
  }
  return $link;
}

/**
 * Return a link to the Drupal API docs.
 *
 * @param $function
 *   Function to link to.
 * @param $version
 *   Version to link to.
 */
function _drupalapi($function, $version = '') {
  return _l($function, "http://api.drupal.org/api/function/$function/$version");
}

/**
 * Return a link to the PHP API docs.
 *
 * @param $function
 *   Function to link to.
 */
function _phpapi($function) {
  return _l($function, "http://php.net/$function");
}

/**
 * Return the path to the coder_review module.
 */
function _coder_review_file_path() {
  return dirname(__FILE__);
}

/**
 * Return data from the cache.
 */
function _cache_get($cid, $bin = 'default') {
  return function_exists('cache_get') ? cache_get($cid, $bin) : array(); // drush_cache_get($cid, $bin);
}

/**
 * Store data in the cache.
 */
function _cache_set($cid, $data, $bin = 'default') {
  return function_exists('cache_set') ? cache_set($cid, $data, $bin) : array(); // drush_cache_set($cid, $data, $bin);
}

/**
 * Return a substring.
 */
function _substr($text, $start, $length = NULL) {
  return function_exists('drupal_substr') ? drupal_substr($text, $start, $length) : substr($text, $start, $length);
}

/**
 * Return the strlen.
 */
function _strlen($text) {
  return function_exists('drupal_strlen') ? drupal_strlen($text) : strlen($text);
}

/**
 * Return all available file paths matching the regular expression.
 */
function _file_list($path, $regex, $recurse = TRUE) {
  $files = array();
  foreach (scandir($path) as $file) {
    if ($file != '.' && $file != '..' && is_dir("$path/$file")) {
      if ($recurse) {
        $files += _file_list("$path/$file", $regex);
      }
    }
    elseif (preg_match($regex, $file)) {
      $files["$path/$file"] = "$path/$file";
    }
  }
  return $files;
}

/**
 * Display and retrieve messages.
 */
function _message($message = NULL, $type = 'status') {
  if (function_exists('drupal_set_message')) {
    drupal_set_message($message);
  }

  static $messages = array();
  if ($message) {
    $messages[$type][] = $message;
  }
  return $messages;
}
