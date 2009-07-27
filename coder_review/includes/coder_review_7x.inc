<?php
// $Id$

/**
 * @file
 * This include file implements coder functionality for 6.x -> 7.x upgrades.
 */

/**
 * Implementation of hook_reviews().
 */
function coder_review_7x_reviews() {
  $argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
  $taxonomy_tables = '\{(term_data|term_hierarchy|term_node|term_relation|term_synonym|vocabulary|vocabulary_node_types)\}';

  $rules = array(
   // .info file parsing and checking
      array(
        '#type' => 'callback',
        '#value' => '_coder_review_7x_info_file_review_callback',
        // @NOTE: this warning_callback isn't used.  It only exists to catch
        // potential errors in the code.
        '#warning_callback' => '_coder_review_7x_info_file_review_callback_warning',
      ),
    // Core Functions Removed
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]referer_uri\s*\(',
        '#warning_callback' => '_coder_review_7x_referer_uri_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_clone\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_clone_warning',
      ),
      // http://drupal.org/node/224333#node_view --- node_view was removed in favor of node_build
      // http://drupal.org/node/224333#comment_validate_removed --- comment_validate() has been removed

    // Module Info / Install
      // http://drupal.org/node/224333#api_php --- Code documentation to module.api.php
      // http://drupal.org/node/224333#update_php --- Update functions in .install files must include a Doxygen style comment
      // http://drupal.org/node/224333#registry --- Module .info files must now specify all loadable code files explicitly.


    // System
      // http://drupal.org/node/224333#absolute_includes --- Use absolute path (constructed from DRUPAL_ROOT) when including a file
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]function_exists\s*\(',
        '#warning_callback' => '_coder_review_7x_function_exists_warning',
      ),
      array(
        '#type' => 'grep',
        '#value' => '$_SESSION',
        '#warning_callback' => '_coder_review_7x_session_warning',
      ),
      // http://drupal.org/node/224333#variable_get_default_null --- Default parameter when getting variables
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]time\s*\([\w\d\'"]*\)',
        '#warning_callback' => '_coder_review_7x_time_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_rebuild_code_registry\s*\(',
        '#warning_callback' => '_coder_review_7x_code_registry_rebuild_function_warning',
      ),
      // http://drupal.org/node/224333#hook_modules_action --- New hooks: hook_modules_installed, hook_modules_enabled, hook_modules_disabled, and hook_modules_uninstalled</a>
      // http://drupal.org/node/224333#drupal_uninstall_modules --- drupal_uninstall_module() is now drupal_uninstall_modules()
      // http://drupal.org/node/224333#module_implements_not_module_list --- use module_implements not module_list when calling hook implementations
      // http://drupal.org/node/224333#drupal_http_request_parameters --- Parameters for drupal_http_request() have changed
      // http://drupal.org/node/224333#moved_statistics_settings --- Moved statistics settings from admin/reports/settings to admin/settings/statistics and added a new 'administer statistics' permission

    // Permissions and Access
      // http://drupal.org/node/224333#descriptions-permissions --- Permissions are required to have descriptions
      // http://drupal.org/node/224333#descriptions-permissions --- Permissions are required to have titles additionally to descriptions
      // http://drupal.org/node/224333#bypass_node_access --- "administer nodes" permission split into "administer nodes" and "bypass node access"
      // http://drupal.org/node/224333#moved_statistics_settings --- Moved statistics settings from admin/reports/settings to admin/settings/statistics and added a new 'administer statistics' permission
      // http://drupal.org/node/224333#sorting-permissions --- Permissions are no longer sorted alphabetically
      // http://drupal.org/node/224333#permission-tables --- New permission tables.
      // http://drupal.org/node/224333#node_access_alter_hooks --- Node access hooks now have drupal_alter() functions

    // Database
      // http://drupal.org/node/224333#dbtng --- A completely new database API has been added
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_schema$',
        '#value' => '[\'"]description[\'"]\s*=>\s*(t|st)\(',
        '#warning_callback' => '_coder_review_7x_schema_translation_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]db_rewrite_sql\s*\(',
        '#warning_callback' => '_coder_review_7x_db_rewrite_sql_warning',
      ),

    // Menu
      // http://drupal.org/node/224333#menu-file-path --- The hook_menu() and hook_theme() "file" and "file path" keys have been removed.
      // http://drupal.org/node/224333#menu_callback_array --- Menu callbacks should return an array; hello hook_page_alter()

    // Block
      // http://drupal.org/node/224333#block_optional --- Block module now optional
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_block$',
        '#value' => '[\s\(](switch\s*\(\$op\)\s*{|if\s*\(\$op\s)',
        '#warning_callback' => '_coder_review_7x_hook_block_op_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(block)\s*\(\s*\$op\s*,',
        '#warning_callback' => '_coder_review_7x_hook_block_op_warning',
      ),

    // Comment
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]_comment_load\s*\(',
        '#warning_callback' => '_coder_review_7x_comment_load_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_menu$|_menu_alter$',
        '#value' => '%_comment',
        '#warning_callback' => '_coder_review_7x_comment_wildcard_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '\{comments\}\s+WHERE\s+.*?\s*status\s*(!=|=|<>|>|<)\s*(0|1)',
        '#warning_callback' => '_coder_review_7x_comment_status_warning',
      ),
      // http://drupal.org/node/224333#comment_save_refactor --- comment_save() now supports programatic saving


    // Actions
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]actions_synchronize\s*\((\$actions|[^,]+,)',
        '#warning_callback' => '_coder_review_7x_actions_synchronize_warning',
      ),

    // Input Sanitization & Input Formats
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]check_markup\s*\((' . $argex . '\s*,\s*){2}(\$check|FALSE|TRUE)',
        '#warning_callback' => '_coder_review_7x_check_markup_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]drupal_set_title\s*\(\s*(check_plain\s*\(|t\s*\(\s*[\'"][^@%]*(@\w|\%\w))',
        '#never' => 'PASS_THROUGH',
        '#warning_callback' => '_coder_review_7x_drupal_set_title_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'function\s+\w+_filter\s*\(([^,]+?,\s*){4}' . $argex . '[^,]*\)',
        '#warning_callback' => '_coder_review_7x_hook_filter_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '\$form\[.+?=\s*filter_form\(',
        '#warning_callback' => '_coder_review_7x_input_format_warning',
      ),

    // Taxonomy
      // http://drupal.org/node/224333#taxonomy_get_tree --- taxonomy_get_tree()
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '^(select\s+.*\s+from\s+' . $taxonomy_tables .  '|insert\s+into\s+' .  $taxonomy_tables . '|update\s+' .  $taxonomy_tables . '\s+set|delete\s+from\s+' . $taxonomy_tables . ')',
        '#warning_callback' => '_coder_review_7x_taxonomy_tables_warning',
      ),
      // http://drupal.org/node/224333#taxonomy_term_load_multiple --- taxonomy_term_load() and taxonomy_term_load_multiple()
      // ? http://drupal.org/node/224333#term_functions --- Standardised and renamed taxonomy term save/load/delete functions</a>
      // ? http://drupal.org/node/224333#term_hooks --- Added taxonomy term hooks
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_get_term\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_get_term_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_save_term\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_save_term_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_term_save\s*\(\s*array\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_save_term_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_del_term\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_del_term_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_save_vocabulary\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_save_vocabulary_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_vocabulary_save\s*\(\s*array\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_save_vocabulary_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_del_vocabulary\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_del_vocabulary_warning',
      ),

      // http://drupal.org/node/224333#taxonomy_hooks --- New hooks: hook_taxonomy_term_load(), hook_taxonomy_term_insert(), hook_taxonomy_term_update(), hook_taxonomy_term_delete() and hook_taxonomy_vocabulary_load(), hook_taxonomy_vocabulary_insert(), hook_taxonomy_vocabulary_update(), hook_taxonomy_vocabulary_delete()

    // Javascript
      // http://drupal.org/node/224333#drupal-behaviors --- Changed Drupal.behaviors to objects having the methods 'attach' and 'detach'
      // http://drupal.org/node/224333#drupal_add_js_css_reset --- Ability to reset JavaScript/CSS
      // http://drupal.org/node/224333#drupal_add_js_options --- Changed parameters for drupal_add_js() and drupal_add_css()
      // http://drupal.org/node/224333#drupal_add_js_weight --- Replace 'core', 'module' and 'theme' with 'file' in drupal_add_js()
      // http://drupal.org/node/224333#hook_js_alter --- New hook_js_alter to alter JavaScript
      // http://drupal.org/node/224333#javascript_compatibility --- JavaScript should be compatible with other libraries than jQuery
      // http://drupal.org/node/224333#drupal_add_js_external --- External JavaScript can now be referenced through drupal_add_js()
      // http://drupal.org/node/224333#local_settings_behaviors --- Settings passed locally to JavaScript Behaviors
      // http://drupal.org/node/224333#jquery_13 --- jQuery 1.3.x

    // CSS
      // http://drupal.org/node/224333#drupal_add_js_options --- Changed parameters for drupal_add_js() and drupal_add_css()
      // http://drupal.org/node/224333#drupal_add_js_css_reset --- Ability to reset JavaScript/CSS

    // Theming
      // http://drupal.org/node/224333#drupal_render_theming --- drupal_render theming properties changed
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_rebuild_theme_registry\s*\(',
        '#warning_callback' => '_coder_review_7x_theme_rebuild_function_warning',
      ),

    // Form API
      // http://drupal.org/node/224333#element_theme_properties --- Element theming properties used by drupal_render() have changed
      // http://drupal.org/node/224333#drupal_render_theming --- drupal_render theming properties changed
      // http://drupal.org/node/224333#node_form --- Simpler checking for the node form during hook_form_alter()
      // http://drupal.org/node/224333#process-functions --- Some #process functions have been renamed
      // http://drupal.org/node/224333#markup --- Use '#markup' not '#value' for markup.
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_execute\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_execute_renamed_warning',
      ),

    // File API
      // http://drupal.org/node/224333#unmanaged_files --- File operations that don't affect the database have been renamed
      // http://drupal.org/node/224333#user_picture --- User pictures are now managed files
      // http://drupal.org/node/224333#file_load_multiple --- file_load_multiple()
      // http://drupal.org/node/224333#file_scan_directory_array-itize --- file_scan_directory()'s optional parameters are now an array
      // http://drupal.org/node/224333#file_scan_directory_nomatch --- file_scan_directory() now uses a preg regular expression for the no match parameter
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]file_set_status\s*\(',
        '#warning_callback' => '_coder_review_7x_file_set_status_warning',
      ),
      // http://drupal.org/node/224333#file_validate_extensions --- file_validate_extensions() enforces check for uid=1
      array(
        '#type' => 'regex',
        '#value' => '[^\w]FILE_STATUS_TEMPORARY[^\w]',
        '#warning_callback' => '_coder_review_7x_file_status_temporary_warning',
      ),
      // http://drupal.org/node/224333#preg_match --- file_scan_directory() and drupal_system_listing() use preg regular expressions

    // User API
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]user_delete\s*\(',
        '#warning_callback' => '_coder_review_7x_user_delete_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\'"]logout[\'"]',
        '#warning_callback' => '_coder_review_7x_user_logout_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_user$',
        '#value' => '[\s\(](switch\s*\(\$op\)\s*{|if\s*\(\$op\s)',
        '#warning_callback' => '_coder_review_7x_hook_user_op_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(user)\s*\(\s*\$op\s*,',
        '#warning_callback' => '_coder_review_7x_hook_user_op_warning',
      ),

    // Node API
      // http://drupal.org/node/224333#node_links --- Move node links into $node->content
      // http://drupal.org/node/224333#node_load_multiple --- node_load() and node_load_multiple()
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_node_info$',
        '#value' => '[\'"](module)[\'"]\s*=>\s*[\'"].*?[\'"]',
        '#warning_callback' => '_coder_review_7x_hook_node_info_module_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_node_info$',
        '#value' => '[\'"](module|base)[\'"]\s*=>\s*[\'"]node[\'"]',
        '#warning_callback' => '_coder_review_7x_hook_node_info_node_content_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_nodeapi$',
        '#value' => '[\s\(](switch\s*\(\$op\)\s*{|if\s*\(\$op\s)',
        '#warning_callback' => '_coder_review_7x_hook_nodeapi_op_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(nodeapi)\s*\(\s*&' . $argex . '\s*,\s*\$op\s*,',
        '#warning_callback' => '_coder_review_7x_hook_nodeapi_op_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]node_invoke_nodeapi\s*\(',
        '#warning_callback' => '_coder_review_7x_node_invoke_nodeapi_warning',
      ),
  );
  $review = array(
    '#title' => t('Converting 6.x modules to 7.x'),
    '#link' => 'http://drupal.org/node/394066',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade7x' => $review);
}

/**
 * Define the rule callbacks for 7.x, see do_coder_review_callback().
 */
function _coder_review_7x_info_file_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // Only perform this check for module's (not includes).
  $filename = $coder_args['#filename'];
  if (substr($filename, -7) == '.module') {

    // @NOTE: no simpletest for this.
    // Read the .info file.
    $files_line = FALSE;
    $filename = drupal_substr(realpath($filename), 0, -7) . '.info';
    if (file_exists($filename)) {
      if ($lines = file($filename)) {
        foreach ($lines as $lineno => $line) {
          if (preg_match('/^files\s*\[\s*\]\s*=/', $line)) {
            $files_line = TRUE;
          }
        }
        if (empty($files_line)) {
          $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
          $tmprule = $rule;
          $tmprule['#warning_callback'] = '_coder_review_7x_info_files_warning';
          _coder_review_error($results, $tmprule, $severity_name, $lineno, $line);
        }
      }
    }

  }
}

/**
 * Define the warning callbacks.
 */

function _coder_review_7x_referer_uri_warning() {
  return array(
    '#warning' => t('!referer_uri() has been removed and replaced with the PHP global $_SERVER[\'HTTP_REFERER\']',
      array(
        '!referer_uri' => theme('drupalapi', 'referer_uri', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#referer_uri',
  );
}

function _coder_review_7x_drupal_clone_warning() {
  return array(
    '#warning' => t('!drupal_clone() has been removed and the PHP 5 !clone should be used instead.',
      array(
        '!drupal_clone' => theme('drupalapi', 'drupal_clone', '6'),
        '!clone' => l(t('!clone', array('!clone' => 'clone')), 'http://us.php.net/language.oop5.cloning'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_clone',
  );
}

function _coder_review_7x_node_invoke_nodeapi_warning() {
  return array(
    '#warning' => t('!node_invoke_nodeapi() has been removed and !module_invoke_all(\'node_\' . $hook, $node); should be used instead.',
      array(
        '!node_invoke_nodeapi' => theme('drupalapi', 'node_invoke_nodeapi', '6'),
        '!module_invoke_all' => theme('drupalapi', 'module_invoke_all', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_invoke_nodeapi',
  );
}

function _coder_review_7x_session_warning() {
  return array(
    '#warning' => t('Use !drupal_set_session() instead of $_SESSION',
      array(
        '!drupal_set_session()' => theme('drupalapi', 'drupal_set_session', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_session',
  );
}

function _coder_review_7x_db_rewrite_sql_warning() {
  return array(
    '#warning' => t('!db_rewrite_sql() replaced with !hook_query_alter()',
      array(
        '!db_rewrite_sql()' => theme('drupalapi', 'db_rewrite_sql', '6'),
        '!hook_query_alter()' => theme('drupalapi', 'hook_query_alter', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#db_rewrite_sql',
  );
}

function _coder_review_7x_schema_translation_warning() {
  return array(
    '#warning' => t('Schema descriptions are no longer translated in !hook_schema()',
      array(
        '!hook_schema()' => theme('drupalapi', 'hook_schema', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#schema-translation',
  );
}

function _coder_review_7x_comment_load_warning() {
  return array(
    '#warning' => t('!_comment_load() has been renamed to !comment_load()',
      array(
        '!_comment_load()' => theme('drupalapi', '_comment_load', '6'),
        '!comment_load()' => theme('drupalapi', 'comment_load', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#comment-load',
  );
}

function _coder_review_7x_comment_wildcard_warning() {
  return array(
    '#warning' => t('%_comment wildcard has been renamed to %comment'),
    '#link' => 'http://drupal.org/node/224333#comment-load',
  );
}

function _coder_review_7x_taxonomy_tables_warning() {
  return array(
    '#warning' => t("Taxonomy DB table names have changed to begin with 'taxonomy_'"),
    '#link' => 'http://drupal.org/node/224333#taxonomy_tables',
  );
}

function _coder_review_7x_file_set_status_warning() {
  return array(
    '#warning' => t('!file_set_status() has been removed and !file_save() should be used in its place.',
      array(
        '!file_set_status()' => theme('drupalapi', 'file_set_status', '6'),
        '!file_save()' => theme('drupalapi', 'file_save', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#file_set_status',
  );
}

function _coder_review_7x_user_delete_warning() {
  return array(
    '#warning' => t('!user_delete() replaced by !user_cancel()',
      array(
        '!user_delete()' => theme('drupalapi', 'user_delete', '6'),
        '!user_cancel()' => theme('drupalapi', 'user_cancel', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#user_cancel',
  );
}

function _coder_review_7x_hook_user_op_warning() {
  return array(
    '#warning' => t('The $op parameter has been removed from !hook_user()',
      array(
        '!hook_user()' => theme('drupalapi', 'hook_user', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#remove_op',
  );
}

function _coder_review_7x_hook_nodeapi_op_warning() {
  return array(
    '#warning' => t('The $op parameter has been removed from !hook_nodeapi()',
      array(
        '!hook_nodeapi()' => theme('drupalapi', 'hook_nodeapi', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#remove_op',
  );
}

function _coder_review_7x_hook_node_info_node_content_warning() {
  return array(
    '#warning' => t('For content types managed by the node module, change "node" to "node_content" in !hook_node_info()',
      array(
        '!hook_node_info()' => theme('drupalapi', 'hook_node_info', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_type_base',
  );
}

function _coder_review_7x_hook_node_info_module_warning() {
  return array(
    '#warning' => t('Change "module" to "base" in !hook_node_info()',
      array(
        '!hook_node_info()' => theme('drupalapi', 'hook_node_info', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_type_base',
  );
}

function _coder_review_7x_hook_block_op_warning() {
  return array(
    '#warning' => t('The $op parameter has been removed from !hook_block()',
      array(
        '!hook_block()' => theme('drupalapi', 'hook_block', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#remove_op',
  );
}

function _coder_review_7x_user_logout_warning() {
  return array(
    '#warning' => t('Log out path changed from \'logout\' to \'user/logout\''),
    '#link' => 'http://drupal.org/node/224333#logout_path',
  );
}

function _coder_review_7x_time_warning() {
  return array(
    '#warning' => t('For improved performance, it is highly recommended that time() is replaced with REQUEST_TIME',
      array(
        '!time()' => theme('phpapi', 'time'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#time',
  );
}

function _coder_review_7x_actions_synchronize_warning() {
  return array(
    '#warning' => t('Parameters for !actions_synchronize() have changed',
      array(
        '!actions_synchronize()' => theme('drupalapi', 'actions_synchronize', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#actions_synchronize',
  );
}

function _coder_review_7x_check_markup_warning() {
  return array(
    '#warning' => t('Parameters for !check_markup() have changed',
      array(
        '!check_markup()' => theme('drupalapi', 'check_markup', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#check_markup_params',
  );
}

function _coder_review_7x_drupal_set_title_warning() {
  return array(
    '#warning' => t('!drupal_set_title() now uses !check_plain() by default',
      array(
        '!drupal_set_title()' => theme('drupalapi', 'drupal_set_title', 7),
        '!check_plain()' => theme('drupalapi', 'check_plain', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_title',
  );
}

function _coder_review_7x_hook_filter_warning() {
  return array(
    '#warning' => t('Parameters for !hook_filter() have changed',
      array(
        '!hook_filter()' => theme('drupalapi', 'hook_filter', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_filter_params',
  );
}

function _coder_review_7x_input_format_warning() {
  return array(
    '#warning' => t('New #input_format to assign input format selection to fields.'),
    '#link' => 'http://drupal.org/node/224333#input_format',
  );
}

function _coder_review_7x_comment_status_warning() {
  return array(
    '#warning' => t('Comment status values in the database have flipped so they match node status. We recommend you use constants like COMMENT_NOT_PUBLISHED in the future.'),
    '#link' => 'http://drupal.org/node/224333#comment_status',
  );
}

function _coder_review_7x_file_status_temporary_warning() {
  return array(
    '#warning' => t('Removed FILE_STATUS_TEMPORARY constant.'),
    '#link' => 'http://drupal.org/node/224333#remove_FILE_STATUS_TEMPORARY',
  );
}

function _coder_review_7x_theme_rebuild_function_warning() {
  return array(
    '#warning' => t('!drupal_rebuild_theme_registry() function has been renamed to !drupal_theme_rebuild()',
      array(
        '!drupal_rebuild_theme_registry()' => theme('drupalapi', 'drupal_rebuild_theme_registry', 6),
        '!drupal_theme_rebuild()' => theme('drupalapi', 'drupal_theme_rebuild', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#rebuild-functions',
  );
}

function _coder_review_7x_code_registry_rebuild_function_warning() {
  return array(
    '#warning' => t('!drupal_rebuild_code_registry() function has been renamed to !registry_rebuild()',
      array(
        '!drupal_rebuild_code_registry()' => theme('drupalapi', 'drupal_rebuild_code_registry', 6),
        '!registry_rebuild()' => theme('drupalapi', 'registry_rebuild', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#rebuild-functions',
  );
}

function _coder_review_7x_taxonomy_get_term_warning() {
  return array(
    '#warning' => t('!taxonomy_get_term() function has been renamed to !taxonomy_term_load()',
      array(
        '!taxonomy_get_term()' => theme('drupalapi', 'taxonomy_get_term', 6),
        '!taxonomy_term_load()' => theme('drupalapi', 'taxonomy_term_load', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_taxonomy_save_term_warning() {
  return array(
    '#warning' => t('!taxonomy_save_term() function has been renamed to !taxonomy_term_save() and takes a term object as a parameter instead of any array.',
      array(
        '!taxonomy_save_term()' => theme('drupalapi', 'taxonomy_save_term', 6),
        '!taxonomy_term_save()' => theme('drupalapi', 'taxonomy_term_save', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_taxonomy_del_term_warning() {
  return array(
    '#warning' => t('!taxonomy_del_term() function has been renamed to !taxonomy_term_delete()',
      array(
        '!taxonomy_del_term()' => theme('drupalapi', 'taxonomy_del_term', 6),
        '!taxonomy_term_delete()' => theme('drupalapi', 'taxonomy_term_delete', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_taxonomy_save_vocabulary_warning() {
  return array(
    '#warning' => t('!taxonomy_save_vocabulary() function has been renamed to !taxonomy_vocabulary_save() and takes a vocabulary object as a parameter instead of any array.',
      array(
        '!taxonomy_save_vocabulary()' => theme('drupalapi', 'taxonomy_save_vocabulary', 6),
        '!taxonomy_vocabulary_save()' => theme('drupalapi', 'taxonomy_vocabulary_save', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_taxonomy_del_vocabulary_warning() {
  return array(
    '#warning' => t('!taxonomy_del_vocabulary() function has been renamed to !taxonomy_vocabulary_delete()',
      array(
        '!taxonomy_del_vocabulary()' => theme('drupalapi', 'taxonomy_del_vocabulary', 6),
        '!taxonomy_vocabulary_delete()' => theme('drupalapi', 'taxonomy_vocabulary_delete', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_info_files_warning() {
  return array(
    '#warning' => t('Module .info files must now specify all loadable code files explicitly.'),
    '#link' => 'http://drupal.org/node/224333#registry',
  );
}

function _coder_review_7x_function_exists_warning() {
  return array(
    '#warning' => t('Please use the !drupal_function_exists() function instead of !function_exists()',
      array(
        '!function_exists()' => theme('phpapi', 'function_exists'),
        '!drupal_function_exists()' => theme('drupalapi', 'drupal_function_exists', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#registry',
  );
}

function _coder_review_7x_drupal_execute_renamed_warning() {
  return array(
    '#warning' => t('!drupal_execute() has been renamed to !drupal_submit_form()',
      array(
        '!drupal_execute()' => theme('drupalapi', 'drupal_execute'),
        '!drupal_submit_form()' => theme('drupalapi', 'drupal_submit_form', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_execute_drupal_form_submit',
  );
}

