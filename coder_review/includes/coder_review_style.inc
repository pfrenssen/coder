<?php
/**
 * @file
 * This include file implements coder functionality for Drupal Standards.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_style_reviews() {
  $br = 'br';

  $rules[] = array(
    '#type' => 'regex',
    '#value' => '\t',
    '#warning' => 'Use an indent of 2 spaces, with no tabs',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#never' => '<\?php',
    '#value' => '^ (  )*[^ \'".]',
    '#warning' => 'Use an indent of 2 spaces, with no tabs',
    '#severity' => 'minor',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '\s(if|elseif|while|foreach|switch|case|return|for|catch)\(',
    '#warning' => 'Control statements should have one space between the control keyword and opening parenthesis',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(](\w+)\s\(',
    '#not' => '^(if|elseif|while|foreach|switch|case|return|for|list|catch|function)$',
    '#warning' => 'Functions should be called with no spaces between the function name and opening parentheses',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '\){',
    '#warning' => 'use a space between the closing parenthesis and the open bracket',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '(\S=>|=>\S)',
    '#source' => 'php',
    '#warning' => 'Arrays should be formatted with a space separating each element and assignment operator',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '(\.(?:|\s{2,})[^\)\=\s0-9]|[^\(\s0-9](?:|\s{2,})\.)',
    '#warning' => 'String concatenation should be formatted with a space separating the operators (dot .) and the surrounding terms',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '<\?(\w+)',
    '#not' => '^(php|xml)$',
    '#warning' => 'Always use &lt;?php ?&gt; to delimit PHP code, not the &lt;? ?&gt; shorthand',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => 'global\s+\$(\w+)(,\s\$(\w+))*',
    '#not' => '^_|^(' . _coder_review_style_core_global_regex() . ')$',
    '#warning' => 'global variables should start with a single underscore followed by the module and another underscore',
  );
  $rules[] = array(
    '#type' => 'callback',
    '#source' => 'all',
    '#value' => '_coder_review_style_closing_php_callback',
    '#warning' => 'the final ?> should be omitted from all code files',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '}\s*else',
    '#warning' => 'else statements should begin on a new line',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[,][^ \n\r]',
    '#warning' => 'missing space after comma',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '^\s*{',
    '#warning' => 'curly braces { should end a line, not start one',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '(?-i)(function\s+|\$)(([a-z]+[A-Z]+([a-z]*[A-Z]*)*)|([A-Z]+[a-z]+([A-Z]*[a-z]*)*))',
    '#warning' => 'do not use mixed case (camelCase), use lower case and _',
    '#class-not' => '.+',
    '#filename-not' => array('test'), // Add this line too for patch files, where we don't have have class context.
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '\s(stdclass)\s*\(',
    '#not' => '^(?-i)stdClass$',
    '#warning' => "use stdClass caseCapitalization, it's the one exception to the mixed case style standard",
  );
  $rules[] = array(
    '#type' => 'regex',
    '#source' => 'html',
    '#value' => '<' . $br . '>', // NOTE: use $br only to avoid a warning.
    '#warning' => 'use &lt;br /&gt; instead of &lt;br&gt;',
    '#severity' => 'minor',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#source' => 'html',
    '#value' => '(?-i)<[A-Z]+',
    '#warning' => function () {
      return _t('use lowercase html tags to comply with <a href="@xhtml">XHTML</a>',
        array(
          '@xhtml' => 'http://www.w3.org/TR/xhtml1/#h-4.2',
        )
      );
    },
    '#severity' => 'minor',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '\s(if|elseif|while|foreach|switch|return|for|catch)\s*\(.*\) \s*{\s*[^\s]+',
    '#warning' => 'The control statement should be on a separate line from the control conditional',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#filename' => array('tpl.php'),
    '#value' => '\s(if|elseif)\s*\(.*\) \s*{\s*[^\s]+',
    '#warning' => 'The control statement should use ":" syntax instead of curly braces.',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#source' => 'all',
    '#value' => '[ \t]+$',
    '#warning' => 'There should be no trailing spaces',
    '#severity' => 'minor',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(](strlen|strtolower|strtoupper|substr|ucfirst)\s*\(',
    '#warning' => 'in most cases, replace the string function with the drupal_ equivalent string functions',
    '#severity' => 'minor',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '\[\s*[A-Za-z][A-Za-z0-9_]*\s*]',
    '#not' => '\[\s*[A-Z][A-Z0-9_]*\s*]',
    '#warning' => 'use quotes around a string literal array index, this is not only a style issue, but a known performance problem',
    '#case-sensitive' => TRUE,
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s=>]+(true|false|null)[\)\s;,\n\r]+',
    '#case-sensitive' => TRUE,
    '#warning' => 'Use uppercase for PHP constants, e.g. NULL, TRUE, FALSE',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '\s+else\s+if\s*\(',
    '#warning' => 'Use "elseif" in place of "else if"',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '\s*[\'"]#value[\'"]\s*=>\s*t\s*\(\s*[\'"]Submit[\'"]\s*\)',
    '#source' => 'allphp',
    '#warning' => 'When labelling buttons, make it clear what the button does, "Submit" is too generic.',
    '#severity' => 'minor',
  );

  $review = array(
    '#title' => 'Drupal Coding Standards',
    '#link' => 'http://drupal.org/node/318',
    '#rules' => $rules,
    '#description' => 'Checks for Drupal coding standards. Every developer should use this.',
  );
  return array('style' => $review);
}

/**
 * Define the rule callbacks for style.
 */
function _coder_review_style_closing_php_callback(&$coder_args, $review, $rule, $lines, &$results) {
  for ($lineno = -1; $last = array_slice($lines, $lineno); $lineno --) {
    $lastline = $last[0][0];
    if (preg_match('/\S/', $lastline)) {
      break;
    }
  }
  if ($last && $lastline && preg_match('/\?>\s*$/i', $lastline)) {
    $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
    _coder_review_error($results, $rule, $severity_name, count($lines));
  }
}

function _coder_review_style_core_global_regex() {
  static $coreglobalregex;
  if (!isset($coreglobalregex)) {
    // Note: there's a little extra overhead in formatting this list as an
    // array, but I think it makes it more readable and maintainable.
    $coreglobalvars = array(
      'base_insecure_url',
      'base_path',
      'base_root',
      'base_secure_url',
      'base_theme_info',
      'base_url',
      'channel',
      'conf',
      'cookie_domain',
      'databases',
      'db_prefix',
      'db_url',
      'drupal_hash_salt',
      'drupal_test_info',
      'element',
      'forum_topic_list_header',
      'image',
      'install_state',
      'installed_profile',
      'is_https',
      'is_https_mock',
      'item',
      'items',
      'language',
      'language_content',
      'language_url',
      'locks',
      'menu_admin',
      'multibyte',
      'pager_limits',
      'pager_page_array',
      'pager_total',
      'pager_total_items',
      'tag',
      'theme',
      'theme_engine',
      'theme_info',
      'theme_key',
      'theme_path',
      'timers',
      'update_free_access',
      'update_rewrite_settings',
      'user',
    );
    $coreglobalregex = implode('|', $coreglobalvars);
  }
  return $coreglobalregex;
}
