<?php
// $Id$

/** @file
 * This include file implements coder functionality for 5.x -> 6.x upgrades
 */

function coder_6x_reviews() {
  $argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
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
      '#value' => '[\s\(]l\s*\(('. $argex .'\s*,\s*){3,}('. $argex .')\s*\)',
      '#warning_callback' => '_coder_6x_url_l_warning', 
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]url\s*\(('. $argex .'\s*,\s*){2,}('. $argex .')\s*\)',
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
      '#value' => '[\s\(]cache_set\s*\(\s*[^,]+,\s*\'',
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
      '#value' => '((function)!\s+|\()file_save_upload\s*\([^,]+\s*,\s*[^,]+\s*,',
      '#not' => 'function file_save_upload',
      '#warning_callback' => '_coder_6x_file_check_upload_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\{file_revisions\}', 
      '#warning' => 'The {'.'file_revisions} table is now {upload}',
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
      '#value' => '\$form\s*\[\s*[\'"]#validate[\'"]\]\[[\'"]',
      '#warning' => 'Validation for specific form elements now uses the #element_validate property',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '(\$form\s*\[\s*[\'"]#multistep[\'"]|[\'"]#multistep[\'"]\s*=>)',
      '#warning' => '#multistep is gone, use $form_state instead',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(validate|submit)\s*\(\s*\$form_id',
      '#warning_callback' => '_coder_6x_form_validate_and_submit_warning',
    ),    
    array(
      '#type' => 'regex',
      '#value' => '\$form\[[\'"]#submit[\'"]\]\[[\'"]',
      '#source' => 'all',
      '#warning' => '$form[\'#submit\'] and $form[\'#validate\'] no longer support custom parameters',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]form_set_value\s*\([^,]+(,\s*[^,\)]+){0,1}\)',
      '#warning_callback' => '_coder_6x_form_set_value_warning',
    ),
/*  from Delete API, which has been reverted
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]confirm_form\s*\([^,]+(,\s*[^,]+){4,}',
      '#warning_callback' => '_coder_6x_confirm_form_warning',
    ),
 */
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]custom_url_rewrite\s*\(',
      '#warning_callback' => '_coder_6x_custom_url_rewrite_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(info)\s*\(\s*\$field',
      '#warning_callback' => '_coder_6x_hook_info_auth_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(auth)\s*\(\s*\$user',
      '#warning_callback' => '_coder_6x_hook_info_auth_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(help)\s*\(\s*[^,]+\)',
      '#warning_callback' => '_coder_6x_hook_help_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '\$form(_values)?\s*\[\s*[\'"]?op[\'"]?\s*\]',
      '#warning' => 'replace $form[\'op\'] with $form_state[\'values\'][\'op\']',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_confirm_submit$',
      '#value' => 'return [\'"]',
      '#warning' => 'in confirm_form, use $form_state[\'redirect\'] instead of return (remember to change function prototype to use &$form_state) ',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_confirm_submit$',
      '#value' => 'return [\'"]',
      '#warning' => 'in confirm_form, use $form_state[\'redirect\'] instead of return (remember to change function prototype to use &$form_state) ',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#function' => '_(un)?install$|_update_[0-9]+$',
      '#value' => '\$GLOBALS\[[\'"]?db_type[\'"]?\]',
      '#warning_callback' => '_coder_6x_schema_api_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#function' => '_submit$|_validate$',
      '#value' => '\$form(_values)?\[[\'"][A-Za-z]',
      '#warning' => 'use $form_state[\'values\'] instead, which is where the values are stored after a submit',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]theme_get_function\s*\(',
      '#warning_callback' => '_coder_6x_theme_get_function_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]db_num_rows\s*\(',
      '#warning_callback' => '_coder_6x_db_num_rows_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_retrieve_form\s*\('. $argex .'\s*\)',
      '#warning_callback' => '_coder_6x_drupal_retrieve_form_warning',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_menu$',
      '#value' => 'node\/add\/',
      '#source' => 'all',
      '#warning_callback' => '_coder_6x_menu_node_add_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^comment_form_location$|admin\/content\/comment\/settings',
      '#source' => 'quote',
      '#warning_callback' => '_coder_6x_comment_form_location_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_access_rebuild\s*\(',
      '#function' => '_enable$',
      '#warning_callback' => '_coder_6x_node_access_rebuild_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_access_rebuild\s*\(',
      '#function' => '_disable$',
      '#warning_callback' => '_coder_6x_node_access_rebuild_warning',
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
          if (preg_match('/^dependencies\s*=/', $line)) {
            $severity_name = _coder_severity_name($coder_args, $review, $rule);
            $warning = t('New syntax for .info files, use dependencies[]');
            $results[-1] = theme('coder_warning', $warning, $severity_name, $lineno, $line);
          }
          if (preg_match('/^core\s*=/', $line)) {
            $core = TRUE;
          }
        }
        if (!isset($core)) {
          $severity_name = _coder_severity_name($coder_args, $review, $rule);
          $warning = t('New syntax for .info files requires core=6.x');
          $results[-1] = theme('coder_warning', $warning, $severity_name, $lineno, $line);
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
  return array(
    '#warning' => t('The menu system has been completely over-hauled in 6.x.'),
    '#link' => 'http://drupal.org/node/102338',
  );
}

function _coder_6x_form_alter_warning() {
  return array(
    '#warning' => t('!hook_form_alter() parameters have changed',
      array(
        '!hook_form_alter' => theme('drupalapi', 'hook_form_alter', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/144132#form-alter',
  );
}

function _coder_6x_link_alter_warning() {
  return array(
    '#warning' => t('!hook_link_alter() parameters have changed',
      array(
        '!hook_link_alter' => theme('drupalapi', 'hook_link_alter', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#link-alter',
  );
}

function _coder_6x_profile_alter_warning() {
  return array(
    '#warning' => t('!hook_profile_alter() parameters have changed',
      array(
        '!hook_profile_alter' => theme('drupalapi', 'hook_profile_alter', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#profile-alter',
  );
}

function _coder_6x_mail_alter_warning() {
  return array(
    '#warning' => t('!hook_mail_alter() parameters have changed',
      array(
        '!hook_mail_alter' => theme('drupalapi', 'hook_mail_alter', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#mail-alter',
  );
}

function _coder_6x_hook_theme_warning() {
  return array(
    '#warning' => t('new !hook_theme() function is required to register theme_ functions',
      array(
        '!hook_theme' => theme('drupalapi', 'hook_theme', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#theme_registry',
  );
}

function _coder_6x_url_l_warning() {
  return array(
    '#warning' => t('!url() and !l() arguments changed, if you have a lot of these use <a href="@script">this conversion script</a>',
      array(
        '!url' => theme('drupalapi', 'url', '6'),
        '!l' => theme('drupalapi', 'l', '6'),
        '@script' => 'http://drupal.org/files/issues/replace.php_.txt',
      )
    ),
    '#link' => 'http://drupal.org/node/114774#url',
  );
}

function _coder_6x_taxonomy_node_get_terms_warning() {
  return array(
    '#warning' => t('!taxonomy_node_get_terms(), !taxonomy_node_get_terms_by_vocabulary(), and !taxonomy_node_delete() now take a full $node object, not just a nid (node id).',
      array(
        '!taxonomy_node_get_terms' => theme('drupalapi', 'taxonomy_node_get_terms', '6'),
        '!taxonomy_node_get_terms_by_vocabulary' => theme('drupalapi', 'taxonomy_node_get_terms_by_vocabulary', '6'),
        '!taxonomy_node_delete' => theme('drupalapi', 'taxonomy_node_delete', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#taxonomy-revisions',
  );
}

function _coder_6x_format_plural_warning() {
  return array(
    '#warning' => t('!format_plural() accepts replacements, you no longer need to use !strtr',
      array(
        '!format_plural' => theme('drupalapi', 'format_plural', '6'),
        '!strtr' => theme('phpapi', 'strtr'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#format-plural',
  );
}

function _coder_6x_watchdog_warning() {
  return array(
    '#warning' => t('Parameters of !watchdog() changed, you no longer need to use !t',
      array(
        '!watchdog' => theme('drupalapi', 'watchdog', '6'),
        '!t' => theme('drupalapi', 't', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#watchdog_parameters',
  );
}

function _coder_6x_cache_set_warning() {
  return array(
    '#warning' => t('Changes to !cache_set parameter order',
      array(
        '!cache_set' => theme('drupalapi', 'cache_set', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#cache-set-parameter-order',
  );
}

function _coder_6x_cache_get_warning() {
  return array(
    '#warning' => t('!cache_set and !cache_get automatically (un)serialize complex data types',
      array(
        '!cache_set' => theme('drupalapi', 'cache_set', '6'),
        '!cache_get' => theme('drupalapi', 'cache_get', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#cache-data-parameter',
  );
}

function _coder_6x_remote_addr_warning() {
  return array(
    '#warning' => t('Use new !ip_address() function instead of $_SERVER[\'REMOTE_ADDR\']',
      array(
        '!ip_address' => theme('drupalapi', 'ip_address', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#ip-address',
  );
}

function _coder_6x_file_check_upload_warning() {
  return array(
    '#warning' => t('!file_check_upload() merged into !file_save_upload()',
      array(
        '!file_check_upload' => theme('drupalapi', 'file_check_upload', '6'),
        '!file_save_upload' => theme('drupalapi', 'file_save_upload', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#file-check-upload',
  );
}

function _coder_6x_form_validate_and_submit_warning() {
  return array(
    '#warning' => t('The number of parameters for form !hook_validate and !hook_submit have changed',
      array(
        '!hook_validate' => theme('drupalapi', 'hook_validate', '6'),
        '!hook_submit' => theme('drupalapi', 'hook_submit', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/144132#process-params',
  );
}

function _coder_6x_form_set_value_warning() {
  return array(
    '#warning' => t('!form_set_value() parameters have changed',
      array(
        '!form_set_value' => theme('drupalapi', 'form_set_value', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/144132#set-value',
  );
}

function _coder_6x_confirm_form_warning() {
  return array(
    '#warning' => t('The arguments to !confirm_form() have changed',
      array(
        '!confirm_form' => theme('drupalapi', 'confirm_form', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#confirm-form-args',
  );
}

function _coder_6x_custom_url_rewrite_warning() {
  return array(
    '#warning' => t('In place of !custom_url_rewrite, use !custom_url_rewrite_inbound() or !custom_url_rewrite_outbound()',
      array(
        '!custom_url_rewrite' => theme('drupalapi', 'custom_url_rewrite', '6'),
        '!custom_url_rewrite_inbound' => theme('drupalapi', 'custom_url_rewrite_inbound', '6'),
        '!custom_url_rewrite_outbound' => theme('drupalapi', 'custom_url_rewrite_outbound', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#custom-url-rewrite',
  );
}

function _coder_6x_hook_info_auth_warning() {
  return array(
    '#warning' => t('hook no longer exists, use !hook_form_alter() to swap your own validation handler',
      array(
        '!hook_form_alter' => theme('drupalapi', 'hook_form_alter', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#dist-auth',
  );
}

function _coder_6x_hook_help_warning() {
  return array(
    '#warning' => t('The arguments to !hook_help have changed',
      array(
        '!hook_help' => theme('drupalapi', 'hook_help', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#hook-help',
    '#description' => t('Change the first two lines to <code>function yourmodule_help($path, $arg) {'."\n".' switch ($path) { ...</code>.'),
  );
}

function _coder_6x_schema_api_warning() {
  return array(
    '#warning' => t('A new schema API has been added in 6.x'),
    '#link' => 'http://drupal.org/node/146843',
    '#description' => t('This patch caused changes to the format of hook_install(), hook_uninstall(), and hook_update_N(). No longer are switch statements done on $GLOBALS[\'db_type\']; instead, use the variety of schema API functions to perform table manipulation.'),
  );
}

function _coder_6x_theme_get_function_warning() {
  return t('!theme_get_function has been deprecated because of template theming; see !theme_get_registry',
    array(
      '!theme_get_function' => theme('drupalapi', 'theme_get_function', '5'),
      '!theme_get_registry' => theme('drupalapi', 'theme_get_registry', '6'),
    )
  );
}

function _coder_6x_db_num_rows_warning() {
  return array(
    '#warning' => t('!db_num_rows has been deprecated',
      array(
        '!db_num_rows' => theme('drupalapi', 'db_num_rows', '5'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#db-num-rows',
    '#description' => t('The db_num_rows() method was removed from the database abstraction layer in 6.x core, as it was a database dependent method.  Use SELECT COUNT(*) or some other appropriate solutions'),
  );
}

function _coder_6x_drupal_retrieve_form_warning() {
  return array(
    '#warning' => t('Parameters for !drupal_retrieve_form() have changed, add $form_state as a second argument',
      array(
        '!drupal_retrieve_form' => theme('drupalapi', 'drupal_retrieve_form', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/144132#retrieve-form',
  );
}

function _coder_6x_menu_node_add_warning() {
  return array(
    '#warning' => t('The node/add/$type menu items are now auto-generated by the menu system in 6.x. You should not declare them in your menu hook. See <a href="@url">node/add is now menu generated</a>.',
      array(
        '@url' => url('http://drupal.org/node/114774#node_add_summary'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#node_add_summary',
  );
}

function _coder_6x_comment_form_location_warning() {
  return array(
    '#warning' => t('Comment settings are now per node type.'),
    '#link' => 'http://drupal.org/node/114774#comment_settings_per_node_type',
  );
}

function _coder_6x_node_access_rebuild_warning() {
  return array(
    '#warning' => t('!node_access_rebuild should not be called from !hook_enable or !hook_disable functions any more.',
      array(
        '!node_access_rebuild' => theme('drupalapi', 'node_access_rebuild', '6'),
        '!hook_enable' => theme('drupalapi', 'hook_enable', '6'),
        '!hook_disable' => theme('drupalapi', 'hook_disable', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#node_access_rebuild_batch',
  );
}
