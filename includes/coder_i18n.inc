<?php

/**
 * @file
 * This include file implements coder functionality to check for Internationalization issues.
 */

/**
 * Implementation of hook_reviews().
 */
function coder_i18n_reviews() {
  $argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]l\s*\(\s*["\']',
      '#filename-not' => array('install'),
      '#warning_callback' => '_coder_i18n_l_without_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]l\s*\(\s*[\'"]',
      '#function' => '_install$',
      '#warning_callback' => '_coder_i18n_in_install_l_without_st',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]t\s*\(\s*[\'"]',
      '#function' => '_install$',
      '#warning_callback' => '_coder_i18n_in_install_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]alert\s*\(\s*[\'"]',
      '#filename' => array('js'),
      '#warning' => 'Javascript strings should be passed through Drupal.t().',
    ),
    array(
      '#type' => 'regex',
      '#value' => '#title\s*=>\s*[\'"][\'"]',
      '#warning_callback' => '_coder_i18n_fapi_title_without_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]form_error\s*\(\s*'. $argex .'\s*,\s*[\'"]',
      '#warning_callback' => '_coder_i18n_form_error_without_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\'"]title[\'"]\s*=>\s*[\'"][^<]',
      '#warning_callback' => '_coder_i18n_in_hook_links_without_t',
      '#source' => 'allphp',
      '#function' => '_link$',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_title\s*\(\s*[\'"]',
      '#warning_callback' => '_coder_i18n_drupal_set_title_without_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_message\s*\(\s*[\'"]',
      '#warning_callback' => '_coder_i18n_drupal_set_message_without_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]watchdog\s*\(\s*'. $argex .'\s*,\s*(t|st)\(',
      '#warning_callback' => '_coder_i18n_watchdog_with_t',
    ),
    // @NOTE: Add duplicate of the 6.x upgrade rule.
    array(
      '#type' => 'regex',
      '#function' => '_menu$',
      '#source' => 'allphp',
      '#value' => '\'title\'\s*=>\s*t\(|\'description\'\s*=>\s*t\(',
      '#warning_callback' => '_coder_i18n_menu_with_t',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](t|st)\s*\(\s*[\'"](\s+|[^\)]*?\s+[\'"]\s*[,\)])',
      '#not' => '[\s\(](t|st)\s*\(\s*[\'"][^\s].*?([\'"]\s+[^,\)])*.*[^\s][\'"][,\)]',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_i18n_space_starts_or_ends_t',
    ),
    array(
      '#type' => 'callback',
      '#filename' => array('po'),
      '#value' => '_coder_translation_callback',
    ),
  );
  $review = array(
    '#title' => t('Internationalization'),
    '#rules' => $rules,
  );
  return array('i18n' => $review);
}

/**
 * Define the rule callbacks for style.
 */
function _coder_translation_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $severity_name = _coder_severity_name($coder_args, $review, $rule);
  $ignores = $coder_args['#ignore_lines'][$review['#review_name']];

  // Parse the translation file into msgid/msgstr pairs.
  $translations = array();
  foreach ($coder_args['#all_lines'] as $lineno => $line) {
    if (preg_match('/^(msgid|msgstr) "(.*)"$/', $line, $matches)) {
      if ($matches[1] == 'msgid') {
        $msgid = $matches[2];
      }
      elseif (!empty($msgid)) {
        $translations[$lineno] = array('msgid' => $msgid, 'msgstr' => $matches[2]);
        unset($msgid);
      }
    }
  }

  // Check each translation.
  foreach ($translations as $lineno => $translation) {
    $msgid = $translation['msgid'];
    $msgstr = $translation['msgstr'];

    // Check the translation first capitable letter.
    if (ctype_upper($msgid[0]) != ctype_upper($msgstr[0])) {
      $rule = array(
        '#warning' => "The first letter in the translation text should have the same capitalization as it's original text.",
      );
      _coder_error($results, $rule, $severity_name, $lineno, $msgstr, $ignores);
    }

    // Check the translation trailing periods.
    if (drupal_substr($msgid, -1, 1) == '.' && !drupal_substr($msgstr, -1, 1) != '.') {
      $rule = array(
        '#warning' => 'The translation text should end in a period when the same original text also ends in a period.',
      );
      _coder_error($results, $rule, $severity_name, $lineno, $msgstr, $ignores);
    }

    // Check punctuation characters.
    if (preg_match_all('/[\.,:;?!]/', $msgid, $matches)) {
      foreach ($matches[0] as $html) {
        if (stripos($msgstr, $html) === FALSE) {
          $rule = array(
            '#warning' => 'Punctuation characters (.,:;?!) from the original text should exist in the translation.',
          );
          _coder_error($results, $rule, $severity_name, $lineno, $msgstr, $ignores);
        }
      }
    }
    if (preg_match_all('/[\.,:;?!]/', $msgstr, $matches)) {
      foreach ($matches[0] as $html) {
        if (stripos($msgid, $html) === FALSE) {
          $rule = array(
            '#warning' => 'Punctuations characters (.,:;?!) in the translation should also exist in the original text.',
          );
          _coder_error($results, $rule, $severity_name, $lineno, $msgstr, $ignores);
        }
      }
    }

    // Check HTML tags.
    if (preg_match_all('/<[^>]*>/', $msgid, $matches)) {
      foreach ($matches[0] as $html) {
        if (stripos($msgstr, $html) === FALSE) {
          $rule = array(
            '#warning' => 'HTML from the original text should also exist in the translation.'
          );
          _coder_error($results, $rule, $severity_name, $lineno, $msgstr, $ignores);
        }
      }
    }

    // Check placeholders.
    if (preg_match_all('/[\!\@\%]\w+/', $msgid, $matches)) {
      foreach ($matches[0] as $placeholder) {
        if (stripos($msgstr, $placeholder) === FALSE) {
          $rule = array(
            '#warning' => 'If placeholders like !name, @name or %name exist in the original text, they must also exist in the translation.',
          );
          _coder_error($results, $rule, $severity_name, $lineno, $msgstr, $ignores);
        }
      }
    }

    // @TODO: Check translations for opening/closing tags if they contain HTML.
    // @TODO: Quotation checks.
    // @TODO: Parenthesis (()[]{}) checks.
  }
}

/**
 * Define the warning callbacks.
 */

function _coder_i18n_l_without_t() {
  return array(
    '#warning' => t('The $text argument to !l() should be enclosed within !t() so that it is translatable.',
      array(
        '!l' => theme('drupalapi', 'l'),
        '!t' => theme('drupalapi', 't'),
      )
    ),
  );
}

function _coder_i18n_in_install_l_without_st() {
  return array(
    '#warning' => t('The $text argument to !l() should be enclosed within !st() so that it is translatable from within the install.',
      array(
        '!l' => theme('drupalapi', 'l'),
        '!st' => theme('drupalapi', 'st'),
      )
    ),
  );
}

function _coder_i18n_in_install_t() {
  return array(
    '#warning' => t('Use !st() instead of !t() in !hook_install(), !hook_uninstall() and !hook_update_N()',
      array(
        '!st' => theme('drupalapi', 'st'),
        '!t' => theme('drupalapi', 't'),
        '!hook_install' => theme('drupalapi', 'hook_install'),
        '!hook_uninstall' => theme('drupalapi', 'hook_uninstall'),
        '!hook_update_N' => theme('drupalapi', 'hook_update_N'),
      )
    ),
  );
}

function _coder_i18n_fapi_title_without_t() {
  return array(
    '#warning' => t('The FAPI #title should be enclosed within !t() so that it is translatable.',
      array(
        '!l' => theme('drupalapi', 'l'),
        '!t' => theme('drupalapi', 't'),
      )
    ),
  );
}

function _coder_i18n_form_error_without_t() {
  return array(
    '#warning' => t('The $message argument to !form_error() should be enclosed within !t() so that it is translatable.',
      array(
        '!form_error' => theme('drupalapi', 'form_error'),
        '!t' => theme('drupalapi', 't'),
      )
    ),
  );
}

function _coder_i18n_in_hook_links_without_t() {
  return array(
    '#warning' => t("The 'title' option should be enclosed within !t() so that it is translatable.",
      array(
        '!t' => theme('drupalapi', 't'),
      )
    ),
  );
}

function _coder_i18n_drupal_set_message_without_t() {
  return array(
    '#warning' => t('The $message argument to !drupal_set_message() should be enclosed within !t() so that it is translatable.',
      array(
        '!drupal_set_message' => theme('drupalapi', 'drupal_set_message'),
        '!t' => theme('drupalapi', 't'),
      )
    ),
  );
}

function _coder_i18n_drupal_set_title_without_t() {
  return array(
    '#warning' => t('The $title argument to !drupal_set_title() should be enclosed within !t() so that it is translatable.',
      array(
        '!drupal_set_title' => theme('drupalapi', 'drupal_set_title'),
        '!t' => theme('drupalapi', 't'),
      )
    ),
  );
}

function _coder_i18n_watchdog_with_t() {
  return array(
    '#warning' => t('The $message argument to !watchdog() should NOT be enclosed within !t(), so that it can be properly translated at display time.',
      array(
        '!watchdog' => theme('drupalapi', 'watchdog'),
        '!t' => theme('drupalapi', 't'),
      )
    ),
  );
}

function _coder_i18n_menu_with_t() {
  return array(
    '#warning' => t('Menu item titles and descriptions should NOT be enclosed within !t().',
      array(
        '!t' => theme('drupalapi', 't'),
      )
    ),
    '#link' => 'http://drupal.org/node/140311',
  );
}

function _coder_i18n_space_starts_or_ends_t() {
  return array(
    '#warning' => t('The $string argument to !t() should not begin or end with a space.',
      array(
        '!t' => theme('drupalapi', 't'),
      )
    ),
    '#link' => 'http://drupal.org/node/304150',
  );
}
