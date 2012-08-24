<?php
/**
 * @file
 * This include file implements coder functionality to check for Internationalization issues.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_i18n_reviews() {
  $argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';

  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]l\s*\(\s*["\']',
    '#filename-not' => array('install'),
    '#warning' => function () {
      return array(
        '#warning' => t('The $text argument to !l() should be enclosed within !t() so that it is translatable.',
          array(
            '!l' => theme('drupalapi', array('function' => 'l')),
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
      );
    },
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]l\s*\(\s*[\'"]',
    '#function' => '_install$',
    '#warning' => function () {
      return array(
        '#warning' => t('The $text argument to !l() should be enclosed within !st() so that it is translatable from within the install.',
          array(
            '!l' => theme('drupalapi', array('function' => 'l')),
            '!st' => theme('drupalapi', array('function' => 'st')),
          )
        ),
      );
    },
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]t\s*\(\s*[\'"]',
    '#function' => '_install$',
    '#warning' => function () {
      return array(
        '#warning' => t('Use !st() instead of !t() in !hook_install(), !hook_uninstall() and !hook_update_N()',
          array(
            '!st' => theme('drupalapi', array('function' => 'st')),
            '!t' => theme('drupalapi', array('function' => 't')),
            '!hook_install' => theme('drupalapi', array('function' => 'hook_install')),
            '!hook_uninstall' => theme('drupalapi', array('function' => 'hook_uninstall')),
            '!hook_update_N' => theme('drupalapi', array('function' => 'hook_update_N')),
          )
        ),
      );
    },
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]alert\s*\(\s*[\'"]',
    '#filename' => array('js'),
    '#warning' => 'Javascript strings should be passed through Drupal.t().',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '#title\s*=>\s*[\'"][\'"]',
    '#warning' => function () {
      return array(
        '#warning' => t('The FAPI #title should be enclosed within !t() so that it is translatable.',
          array(
            '!l' => theme('drupalapi', array('function' => 'l')),
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
      );
    },
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]form_error\s*\(\s*' . $argex . '\s*,\s*[\'"]',
    '#warning' => function () {
      return array(
        '#warning' => t('The $message argument to !form_error() should be enclosed within !t() so that it is translatable.',
          array(
            '!form_error' => theme('drupalapi', array('function' => 'form_error')),
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
      );
    },
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\'"]title[\'"]\s*=>\s*[\'"][^<]',
    '#warning' => function () {
      return array(
        '#warning' => t("The 'title' option should be enclosed within !t() so that it is translatable.",
          array(
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
      );
    },
    '#source' => 'allphp',
    '#function' => '_link$',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]drupal_set_title\s*\(\s*[\'"]',
    '#warning' => function () {
      return array(
        '#warning' => t('The $title argument to !drupal_set_title() should be enclosed within !t() so that it is translatable.',
          array(
            '!drupal_set_title' => theme('drupalapi', array('function' => 'drupal_set_title')),
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
      );
    },
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]drupal_set_message\s*\(\s*[\'"]',
    '#warning' => function () {
      return array(
        '#warning' => t('The $message argument to !drupal_set_message() should be enclosed within !t() so that it is translatable.',
          array(
            '!drupal_set_message' => theme('drupalapi', array('function' => 'drupal_set_message')),
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
      );
    },
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]watchdog\s*\(\s*' . $argex . '\s*,\s*(t|st)\(',
    '#warning' => function () {
      return array(
        '#warning' => t('The $message argument to !watchdog() should NOT be enclosed within !t(), so that it can be properly translated at display time.',
          array(
            '!watchdog' => theme('drupalapi', array('function' => 'watchdog')),
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
      );
    },
  );
  // @NOTE: Add duplicate of the 6.x upgrade rule.
  $rules[] = array(
    '#type' => 'regex',
    '#function' => '_menu$',
    '#source' => 'allphp',
    '#value' => '\'title\'\s*=>\s*t\(|\'description\'\s*=>\s*t\(',
    '#warning' => function () {
      return array(
        '#warning' => t('Menu item titles and descriptions should NOT be enclosed within !t().',
          array(
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
        '#link' => 'http://drupal.org/node/140311',
      );
    },
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(](t|st)\s*\(\s*[\'"](\s+|[^\)]*?\s+[\'"]\s*[,\)])',
    '#not' => '[\s\(](t|st)\s*\(\s*[\'"][^\s].*?([\'"]\s+[^,\)])*.*[^\s][\'"][,\)]',
    '#source' => 'allphp',
    '#warning' => function () {
      return array(
        '#warning' => t('The $string argument to !t() should not begin or end with a space.',
          array(
            '!t' => theme('drupalapi', array('function' => 't')),
          )
        ),
        '#link' => 'http://drupal.org/node/304150',
      );
    },
  );

  $review = array(
    '#title' => t('Internationalization'),
    '#rules' => $rules,
  );
  return array('i18n' => $review);
}
