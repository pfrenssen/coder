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
  $rules = array(
    // Core Path Changes
      // http://drupal.org/node/224333#logout_path --- Changed log out path from 'logout' to 'user/logout' for consistency

    // Core Functions Removed
      array(
        '#type' => 'regex',
        '#value' => 'referer_uri\s*\(',
        '#warning_callback' => '_coder_review_7x_referer_uri_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'drupal_clone\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_clone_warning',
      ),

    // Module Info / Install
      // http://drupal.org/node/224333#api_php --- Code documentation to module.api.php
      // http://drupal.org/node/224333#absolute_includes --- Use absolute path (constructed from DRUPAL_ROOT) when including a file
      // http://drupal.org/node/224333#update_php --- Update functions in .install files must include a Doxygen style comment
      // http://drupal.org/node/224333#registry --- Module .info files must now specify all loadable code files explicitly.

    // System
      array(
        '#type' => 'grep',
        '#value' => '$_SESSION',
        '#warning_callback' => '_coder_review_7x_session_warning',
      ),
      // http://drupal.org/node/224333#variable_get_default_null --- Default parameter when getting variables
      // http://drupal.org/node/224333#time --- Use defined constant REQUEST_TIME instead of time()
      // http://drupal.org/node/224333#rebuild-functions --- Rebuild functions have changed names
      // http://drupal.org/node/224333#hook_modules_action --- New hooks: hook_modules_installed, hook_modules_enabled, hook_modules_disabled, and hook_modules_uninstalled</a>
      // http://drupal.org/node/224333#module_implements_not_module_list --- use module_implements not module_list when calling hook implementations
      // http://drupal.org/node/224333#drupal_http_request_parameters --- Parameters for drupal_http_request() have changed

    // Permission
      // http://drupal.org/node/224333#descriptions-permissions --- Permissions are required to have descriptions
      // http://drupal.org/node/224333#descriptions-permissions --- Permissions are required to have titles additionally to descriptions
      // http://drupal.org/node/224333#bypass_node_access --- "administer nodes" permission split into "administer nodes" and "bypass node access"
      // http://drupal.org/node/224333#moved_statistics_settings --- Moved statistics settings from admin/reports/settings to admin/settings/statistics and added a new 'administer statistics' permission
      // http://drupal.org/node/224333#sorting-permissions --- Permissions are no longer sorted alphabetically
      // http://drupal.org/node/224333#permission-tables --- New permission tables.

    // Database
      // http://drupal.org/node/224333#dbtng --- A completely new database API has been added
      // http://drupal.org/node/224333#schema-translation --- Schema descriptions are no longer translated
      array(
        '#type' => 'regex',
        '#value' => 'db_rewrite_sql\s*\(',
        '#warning_callback' => '_coder_review_7x_db_rewrite_sql_warning',
      ),

    // Menu
      // http://drupal.org/node/224333#menu-file-path --- The hook_menu() and hook_theme() "file" and "file path" keys have been removed.
      // http://drupal.org/node/224333#menu_callback_array --- Menu callbacks should return an array; hello hook_page_alter()

    // Block
      // http://drupal.org/node/224333#block_optional --- Block module now optional
      // http://drupal.org/node/224333#remove_op --- Remove $op from hook_block

    // Comment
      array(
        '#type' => 'regex',
        '#value' => '_comment_load\s*\(',
        '#warning_callback' => '_coder_review_7x_comment_load_warning',
      ),
      // Any menu items that are using the %_comment wildcard will need to change to %comment.
      // http://drupal.org/node/224333#comment_status --- Comment status values in the database have flipped so they match node status

    // Statistics
      // http://drupal.org/node/224333#moved_statistics_settings --- Moved statistics settings from admin/reports/settings to admin/settings/statistics and added a new 'administer statistics' permission

    // Actions
      // http://drupal.org/node/224333#actions_synchronize --- Parameters for actions_synchronize() have changed

    // Input Sanitization
      // http://drupal.org/node/224333#check_markup_params --- Parameters to check_markup() have changed
      // http://drupal.org/node/224333#drupal_set_title --- drupal_set_title() uses check_plain() by default

    // Input Formats
      // http://drupal.org/node/224333#hook_filter_params --- Parameters to hook_filter() have changed
      // http://drupal.org/node/224333#input_format --- New #input_format to assign input format selection to fields. Changes 'body' field location in node, comment, block, etc.

    // Taxonomy
      // http://drupal.org/node/224333#taxonomy_get_tree --- taxonomy_get_tree()
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '\{(term_data|term_hierarchy|term_node|term_relation|term_synonym|vocabulary|vocabulary_node_types)\}',
        '#warning_callback' => '_coder_review_7x_taxonomy_tables_warning',
      ),
      // http://drupal.org/node/224333#taxonomy_term_load_multiple --- taxonomy_term_load() and taxonomy_term_load_multiple()
      // http://drupal.org/node/224333#term_functions --- Standardised and renamed taxonomy term save/load/delete functions</a>
      // http://drupal.org/node/224333#term_hooks --- Added taxonomy term hooks
      // http://drupal.org/node/224333#taxonomy_crud --- Taxonomy CRUD functions renamed and refactored 
      // http://drupal.org/node/224333#taxonomy_hooks --- New hooks: hook_taxonomy_term_load(), hook_taxonomy_term_insert(), hook_taxonomy_term_update(), hook_taxonomy_term_delete() and hook_taxonomy_vocabulary_load(), hook_taxonomy_vocabulary_insert(), hook_taxonomy_vocabulary_update(), hook_taxonomy_vocabulary_delete()

    // Javascript
      // http://drupal.org/node/224333#drupal-behaviors --- Changed Drupal.behaviors to objects having the methods 'attach' and 'detach'
      // http://drupal.org/node/224333#drupal_add_js_css_reset --- Ability to reset JavaScript/CSS
      // http://drupal.org/node/224333#drupal_add_js_options --- Changed parameters for drupal_add_js() and drupal_add_css()
      // http://drupal.org/node/224333#drupal_add_js_weight --- Replace 'core', 'module' and 'theme' with 'file' in drupal_add_js()
      // http://drupal.org/node/224333#hook_js_alter --- New hook_js_alter to alter JavaScript
      // http://drupal.org/node/224333#javascript_compatibility --- JavaScript should be compatible with other libraries than jQuery

    // CSS
      // http://drupal.org/node/224333#drupal_add_js_options --- Changed parameters for drupal_add_js() and drupal_add_css()

    // Theming
      // http://drupal.org/node/224333#drupal_render_theming --- drupal_render theming properties changed
      // http://drupal.org/node/224333#rebuild-functions --- Rebuild functions have changed names

    // Form API
      // http://drupal.org/node/224333#drupal_render_theming --- drupal_render theming properties changed
      // http://drupal.org/node/224333#node_form --- Simpler checking for the node form during hook_form_alter()
      // http://drupal.org/node/224333#process-functions --- Some <code>#process</code> functions have been renamed
      // http://drupal.org/node/224333#markup --- Use <code>'#markup'</code> not <code>'#value'</code> for markup.

    // File API
      // http://drupal.org/node/224333#unmanaged_files --- File operations that don't affect the database have been renamed
      // http://drupal.org/node/224333#user_picture --- User pictures are now managed files
      // http://drupal.org/node/224333#file_load_multiple --- file_load_multiple()
      // http://drupal.org/node/224333#file_scan_directory_array-itize --- file_scan_directory()'s optional parameters are now an array
      // http://drupal.org/node/224333#file_scan_directory_nomatch --- file_scan_directory() now uses a preg regular expression for the no match parameter
      array(
        '#type' => 'regex',
        '#value' => 'file_set_status\s*\(',
        '#warning_callback' => '_coder_review_7x_file_set_status_warning',
      ),
      // http://drupal.org/node/224333#file_validate_extensions --- file_validate_extensions() enforces check for uid=1
      // http://drupal.org/node/224333#remove_FILE_STATUS_TEMPORARY --- Removed FILE_STATUS_TEMPORARY
      // http://drupal.org/node/224333#preg_match --- file_scan_directory() and drupal_system_listing() use preg regular expressions

    // User API
      array(
        '#type' => 'regex',
        '#value' => 'user_delete\s*\(',
        '#warning_callback' => '_coder_review_7x_user_delete_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'hook_user_delete\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_user_delete_warning',
      ),
      // http://drupal.org/node/224333#logout_path --- Changed log out path from 'logout' to 'user/logout' for consistency
      // http://drupal.org/node/224333#remove_op --- Remove $op from hook_nodeapi and hook_user

    // Node API
      // http://drupal.org/node/224333#node_links --- Move node links into $node->content
      // http://drupal.org/node/224333#node_load_multiple --- node_load() and node_load_multiple()
      // http://drupal.org/node/224333#node_type_base --- In hook_node_info() change 'module' back to 'base' and change  'node' to 'node_content'
      // http://drupal.org/node/224333#remove_op --- Remove $op from hook_nodeapi and hook_user
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
 * Define the warning callbacks.
 */

function _coder_review_7x_referer_uri_warning() {
  return array(
    '#warning' => t('referer_uri() has been removed'),
    '#link' => 'http://drupal.org/node/224333#referer_uri',
    '#description' => t('referer_uri() has been removed and replaced with the PHP-provided global variable $_SERVER[\'HTTP_REFERER\']. If there is no referrer, Drupal will automatically set $_SERVER[\'HTTP_REFERER\'] to an empty string.'),
  );
}

function _coder_review_7x_drupal_clone_warning() {
  return array(
    '#warning' => t('referer_uri() has been removed'),
    '#link' => 'http://drupal.org/node/224333#drupal_clone',
    '#description' => t('Since Drupal 7 requires at least PHP 5, it allowed the removal of the drupal_clone function for a direct call to clone.'),
  );
}

function _coder_review_7x_session_warning() {
  return array(
    '#warning' => t('!drupal_set_session() replaces $_SESSION',
      array(
        '!drupal_set_session()' => theme('drupalapi', 'drupal_set_session', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_session',
    '#description' => t('The $_SESSION superglobal should not be accessed directly. Instead, use !drupal_set_session($name, $value). This is a performance optimization so that no session is started for anonymous users unless a module has put data into the user\'s session. Note that it is important that modules try not to use $_SESSION for anonymous users. Such use ruins the optimization here. This optimization is particularly impressive when Drupal is coupled with a reverse proxy like Squid or Varnish.',
      array(
        '!drupal_set_session()' => theme('drupalapi', 'drupal_set_session', '7'),
      )
    ),
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
    '#description' => t('The string-parsing-based db_rewrite_sql() has been removed in favor of hook_query_alter(). Instead of passing a literal query through db_rewrite_sql() in order to add node access restrictions, for instance, in Drupal 7 use a <a href="http://drupal.org/node/310075">dynamic query</a> and tag it "node_access".',array('html'=>true)),
  );
}

function _coder_review_7x_comment_load_warning() {
  return array(
    '#warning' => t('!_comment_load() replaced with !comment_load()',
      array(
        '!_comment_load()' => theme('drupalapi', '_comment_load', '6'),
        '!comment_load()' => theme('drupalapi', 'comment_load', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#comment-load',
    '#description' => t('_comment_load() was renamed to comment_load() as it is an external-facing API function.'),
  );
}

function _coder_review_7x_taxonomy_tables_warning() {
  return array(
    '#warning' => t('Taxonomy DB table names have changed to begin with \'taxonomy_\'',
      array(
        '!file_set_status()' => theme('drupalapi', 'file_set_status', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#file_set_status',
    '#description' => t('The names of the taxonomy database tables have changed to be consistent with our naming conventions. Each table has been prefixed with \'taxonomy_\'. Table vocabulary_node_types has also changed to the singular form: taxonomy_vocabulary_node_type.'),
  );
}

function _coder_review_7x_file_set_status_warning() {
  return array(
    '#warning' => t('!file_set_status() has been removed',
      array(
        '!file_set_status()' => theme('drupalapi', 'file_set_status', '6'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#file_set_status',
    '#description' => t('The !file_set_status() function which was used, primarily, to make temporary files permanent has been removed.  !file_save() should be used in its place.',
      array(
        '!file_set_status()' => theme('drupalapi', 'file_set_status', '6'),
        '!file_save()' => theme('drupalapi', 'file_save', '7'),
      )
    ),
  );
}

function _coder_review_7x_user_delete_warning() {
  return array(
    '#warning' => t('!user_delete() replaced with !user_cancel()',
      array(
        '!user_delete()' => theme('drupalapi', 'user_delete', '6'),
        '!user_cancel()' => theme('drupalapi', 'user_cancel', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#user_cancel',
    '#description' => t('Users can now cancel their own accounts, when the corresponding "cancel account" permission has been granted. There are multiple account cancellation methods, from which one can be configured as default. Users with the "administer users" permission are able to override the default method, f.e. to completely delete a user including all of the related content.'),
  );
}

function _coder_review_7x_hook_user_delete_warning() {
  return array(
    '#warning' => t('!hook_user_delete() replaced with !hook_user_cancel()',
      array(
        '!hook_user_delete()' => theme('drupalapi', 'hook_user_delete', '6'),
        '!hook_user_cancel()' => theme('drupalapi', 'hook_user_cancel', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#user_cancel',
    '#description' => t('Modules that implemented hook_user_delete() (resp. hook_user($op == \'delete\')) previously, need to update their code to use hook_user_cancel() instead, which takes an (internal) account cancellation method name as third argument.'),
  );
}
