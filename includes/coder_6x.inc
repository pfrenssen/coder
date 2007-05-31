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
      '#value' => '$form\s*\[\s*\'#base\'',
      '#warning' => 'FormAPI\'s #base property is gone',
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
      '#value' => '[\s\(](l|url)\s*\(('. $argex .',\s*){2,}('. $argex .')\s*\)',
      '#warning_callback' => '_coder_6x_url_l_warning', 
    ),
    array(
      '#type' => 'regex',
      '#value' => 'global \$locale',
      '#warning' => '$locale become $language',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(taxonomy_node_get_terms|taxonomy_node_get_terms_by_vocabulary|taxonomy_node_delete_revision)\s*\(\$nid',
      '#warning_callback' => '_coder_6x_taxonomy_node_get_terms_warning',
    ),
    array(
      '#type' => 'callback',
      '#value' => '_coder_6x_callback',
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
