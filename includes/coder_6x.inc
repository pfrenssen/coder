<?php
// $Id$

/** @file
 * This include file implements coder functionality for 5.x -> 6.x upgrades
 */

function coder_6x_reviews() {
  $argex = '(([a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
  $rules = array(
    array(
      '#type' => 'regex',
      '#function' => '_menu$',
      '#value' => '\$items\[\]\s*=|if\s*\(\$may_cache\)',
      '#warning_callback' => '_coder_6x_new_menu_system_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_form_alter\s*\(\$',
      '#warning_callback' => '_coder_6x_form_alter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_link_alter\s*\(\$',
      '#warning_callback' => '_coder_6x_link_alter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_profile_alter\s*\(\$',
      '#warning_callback' => '_coder_6x_profile_alter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_mail_alter\s*\([^,]+,',
      '#warning_callback' => '_coder_6x_mail_alter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](l|url)\s*\(('. $argex .',\s*){2,}('. $argex .')\s*\)',
      '#warning_callback' => '_coder_6x_url_l_warning', 
    ),
    array(
      '#type' => 'regex',
      '#value' => 'global \$locale',
      '#warning' => '$locale became $language',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(taxonomy_node_get_terms|taxonomy_node_get_terms_by_vocabulary|taxonomy_node_delete_revision)\s*\(\$(nid|node->nid)',
      '#warning_callback' => '_coder_6x_taxonomy_node_get_terms_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]strtr\s*\(format_plural\s*\(',
      '#warning_callback' => '_coder_6x_format_plural_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'watchdog\s*\(\s*\'\',\s*t\(\'',
      '#warning_callback' => '_coder_6x_watchdog_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]cache_set\s*\(\s*\'\',\s*\'',
      '#warning_callback' => '_coder_6x_cache_set_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]unserialize\s*\(\s*cache_get\s*\(',
      '#warning_callback' => '_coder_6x_cache_get_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\$_SERVER\s*\[\s*[\'"]REMOTE_ADDR[\'"]\s*\]',
      '#source' => 'all',
      '#warning_callback' => '_coder_6x_remote_addr_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]file_check_upload\s*\(',
      '#warning_callback' => '_coder_6x_file_check_upload_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]file_save_upload\s*\([^,]+\s*,\s*[^,]+\s*,',
      '#warning_callback' => '_coder_6x_file_check_upload_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\{file_revisions\}', 
      '#warning' => 'The {file_revisions} table is now {upload}',
      '#source' => 'quote',
    ),
    array(
      '#type' => 'callback',
      '#value' => '_coder_6x_callback',
    ),
    // FAPI Rules
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '(\$form\s*\[\s*[\'"]#base[\'"]|[\'"]#base[\'"]\s*=>)',
      '#warning' => 'FormAPI\'s #base property is gone, manually populate the $form[\'#submit\'], $form[\'#validate\'], and $form[\'#theme\'] elements with the proper function names',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '(\$form\s*\[\s*[\'"]#pre_render[\'"]|[\'"]#pre_render[\'"]\s*=>)',
      '#warning' => 'FormAPI\'s #pre_render property is gone',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '\$form\s*\[\s*[\'"]#validate[\'"]\]\[[\'"]',
      '#warning' => 'Validation for specific form elements now uses the #element_validate property',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '(\$form\s*\[\s*[\'"]#multistep[\'"]|[\'"]#multistep[\'"]\s*=>)',
      '#warning' => '#multistep is gone, use $form_state instead',
    ),
  );
  $review = array(
    '#title' => t('Converting 5.x modules to 6.x'),
    '#link' => 'http://drupal.org/node/114774',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade6x' => $review);
}

function _coder_6x_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // only perform this check on module's (not includes)
  $filename = $coder_args['#filename'];
  if (substr($filename, -7) == '.module') {
    // if there are any theme functions, make sure that a hook_theme exists
    $theme = false;
    $hook_theme = false;
    foreach ($lines as $lineno => $line) {
      if (preg_match('/function theme_/', $line)) {
        if (!$theme) {
          $theme = true;
          $theme_line = $line;
          $theme_lineno = $lineno;
        }
      }
      if (preg_match('/function \w+_theme\(/', $line)) {
        $hook_theme = true;
      }
    }
    if ($theme && !$hook_theme) {
      $severity_name = _coder_severity_name($coder_args, $review, $rule);
      $warning = _coder_6x_hook_theme_warning();
      $results[0] = theme('coder_warning', $warning, $severity_name, $theme_lineno, $theme_line);
    }

    // read the .info file
    $filename = drupal_substr(realpath($filename), 0, -7) .'.info';
    if (file_exists($filename)) {
      if ($lines = file($filename)) {
        foreach ($lines as $lineno => $line) {
          if (preg_match('/^dependencies\s*=\s*\w+\s+\w+/', $line)) {
            $severity_name = _coder_severity_name($coder_args, $review, $rule);
            $warning = t('New syntax for .info files, use dependencies[]');
            $results[-1] = theme('coder_warning', $warning, $severity_name, $lineno, $line);
          }
        }
      }
    }
  }
}

/**
 * Define the rule callbacks
 */

/**
 * Define the warning callbacks
 */

function _coder_6x_new_menu_system_warning() {
  return t('The menu system has been completely re-hauled in 6.x. See the <a href="@url">Menu system overview</a>.',
    array(
      '@url' => url('http://drupal.org/node/109131'),
    )
  );
}

function _coder_6x_form_alter_warning() {
  return t('!hook_form_alter() parameters have changed',
    array(
      '!hook_form_alter' => theme('drupalapi', 'hook_form_alter'),
    )
  );
}

function _coder_6x_link_alter_warning() {
  return t('!hook_link_alter() parameters have changed',
    array(
      '!hook_link_alter' => theme('drupalapi', 'hook_link_alter'),
    )
  );
}

function _coder_6x_profile_alter_warning() {
  return t('!hook_profile_alter() parameters have changed',
    array(
      '!hook_profile_alter' => theme('drupalapi', 'hook_profile_alter'),
    )
  );
}

function _coder_6x_mail_alter_warning() {
  return t('!hook_mail_alter() parameters have changed',
    array(
      '!hook_mail_alter' => theme('drupalapi', 'hook_mail_alter'),
    )
  );
}

function _coder_6x_hook_theme_warning() {
  return t('new !hook_theme() function is required to register theme_ functions',
    array(
      '!hook_theme' => theme('drupalapi', 'hook_theme'),
    )
  );
}

function _coder_6x_url_l_warning() {
  return t('!url() and !l() arguments changed, if you have a lot of these use <a href="@script">this conversion script</a>',
    array(
      '!url' => theme('drupalapi', 'url'),
      '!l' => theme('drupalapi', 'l'),
      '@script' => 'http://drupal.org/files/issues/replace.php_.txt',
    )
  );
}

function _coder_6x_taxonomy_node_get_terms_warning() {
  return t('!taxonomy_node_get_terms(), !taxonomy_node_get_terms_by_vocabulary(), and !taxonomy_node_delete() now take a full $node object, not just a nid (node id).',
    array(
      '!taxonomy_node_get_terms' => theme('drupalapi', 'taxonomy_node_get_terms'),
      '!taxonomy_node_get_terms_by_vocabulary' => theme('drupalapi', 'taxonomy_node_get_terms_by_vocabulary'),
      '!taxonomy_node_delete' => theme('drupalapi', 'taxonomy_node_delete'),
    )
  );
}

function _coder_6x_format_plural_warning() {
  return t('!format_plural() accepts replacements, you no longer need to use !strtr',
    array(
      '!format_plural' => theme('drupalapi', 'format_plural'),
      '!strtr' => theme('phpapi', 'strtr'),
    )
  );
}

function _coder_6x_watchdog_warning() {
  return t('Parameters of !watchdog() changed, you no longer need to use !t',
    array(
      '!watchdog' => theme('drupalapi', 'watchdog'),
      '!t' => theme('drupalapi', 't'),
    )
  );
}

function _coder_6x_cache_set_warning() {
  return t('Changes to !cache_set parameter order',
    array(
      '!cache_set' => theme('drupalapi', 'cache_set'),
    )
  );
}

function _coder_6x_cache_get_warning() {
  return t('!cache_set and !cache_get automatically (un)serialize complex data types',
    array(
      '!cache_set' => theme('drupalapi', 'cache_set'),
      '!cache_get' => theme('drupalapi', 'cache_get'),
    )
  );
}

function _coder_6x_remote_addr_warning() {
  return t('Use new !ip_address() function instead of $_SERVER[\'REMOTE_ADDR\']',
    array(
      '!ip_address' => theme('drupalapi', 'ip_address'),
    )
  );
}

function _coder_6x_file_check_upload_warning() {
  return t('!file_check_upload() merged into !file_save_upload()',
    array(
      '!file_check_upload' => theme('drupalapi', 'file_check_upload'),
      '!file_save_upload' => theme('drupalapi', 'file_save_upload'),
    )
  );
}
