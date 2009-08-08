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
    // Module Info / Install
      // N/A - in comment review - http://drupal.org/node/224333#afile --- Add Doxygen @file tag to all install files

      array(
        '#type' => 'callback',
        '#value' => '_coder_review_7x_info_file_review_callback',
        // @NOTE: this warning_callback isn't used.  It only exists to catch
        // potential errors in the code.
        '#warning_callback' => '_coder_review_7x_info_file_review_callback_warning',
      ),

      // http://drupal.org/node/224333#api_php --- Code documentation to module.api.php
      // http://drupal.org/node/224333#update_php --- Update functions in .install files must include a Doxygen style comment


    // System
      // N/A - http://drupal.org/node/224333#variable_get_default_null --- Default parameter when getting variables
      // N/A - http://drupal.org/node/224333#hook_modules_action --- New hooks: hook_modules_installed, hook_modules_enabled, hook_modules_disabled, and hook_modules_uninstalled</a>
      // N/A - http://drupal.org/node/224333#static_variable_api --- Standardized API for static variables and resetting them

      // UNCLEAR - http://drupal.org/node/224333#module_implements_not_module_list --- use module_implements not module_list when calling hook implementations

      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '((require|include)(_once)?).+[\'"]\.\/',
        '#warning_callback' => '_coder_review_7x_absolute_includes_warning',
      ),
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
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_uninstall_module\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_uninstall_modules_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_http_request\s*\((' . $argex . '\s*,\s*){2,}(' . $argex .  ')\s*\)',
        '#warning_callback' => '_coder_review_7x_drupal_http_request_parameters_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => 'admin\/reports\/settings',
        '#warning_callback' => '_coder_review_7x_moved_statistics_settings_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]module_rebuild_cache\s*\(',
        '#warning_callback' => '_coder_review_7x_module_rebuild_cache_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]system_theme_data\s*\(',
        '#warning_callback' => '_coder_review_7x_system_theme_data_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_set_html_head\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_set_html_head_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_eval\s*\(',
        '#warning_callback' => '_coder_review_7x_php_eval_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]drupal_set_header\s*\(\s*(\$_SERVER\[[\'"]SERVER_PROTOCOL[\'"]|[\'"][a-zA-Z_-\s]+:.*?[\'"])',
        '#warning_callback' => '_coder_review_7x_http_header_functions_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_set_content\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_set_content_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_get_content\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_get_content_warning',
      ),
      array(
        '#type' => 'callback',
        '#value' => '_coder_review_7x_cache_implementation_review_callback',
        // @NOTE: this warning_callback isn't used.  It only exists to catch
        // potential errors in the code.
        '#warning_callback' => '_coder_review_7x_cache_implementation_review_callback_warning',
      ),


    // Permissions and Access
      // N/A - http://drupal.org/node/224333#moved_statistics_settings --- Added a new 'administer statistics' permission

      // http://drupal.org/node/224333#descriptions-permissions --- Permissions are required to have titles additionally to descriptions
      // http://drupal.org/node/224333#bypass_node_access --- "administer nodes" permission split into "administer nodes" and "bypass node access"
      // http://drupal.org/node/224333#sorting-permissions --- Permissions are no longer sorted alphabetically
      // http://drupal.org/node/224333#permission-tables --- New permission tables.
      // http://drupal.org/node/224333#node_access_alter_hooks --- Node access hooks now have drupal_alter() functions

      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(perm)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_perm_warning',
      ),

      // http://drupal.org/node/224333#php_permissions --- "use PHP for settings" permission should be used for all PHP settings rights (replaces "use PHP for block visibility")
      // http://drupal.org/node/224333#system_admin_menu_block_access --- Hide empty menu categories with access callback
      // http://drupal.org/node/224333#hook_node_access_records --- hook_node_access_records() now applies to unpublished nodes; 'view own unpublished content' permission added


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
      // N/A - http://drupal.org/node/224333#international_item --- Added a new top level admin item, 'international'.

      // http://drupal.org/node/224333#menu-file-path --- The hook_menu() and hook_theme() "file" and "file path" keys have been removed.
      // http://drupal.org/node/224333#menu_callback_array --- Menu callbacks should return an array; hello hook_page_alter()
      // http://drupal.org/node/224333#hook_menu_link_alter --- Changed hook_menu_link_alter() (removed the $menu parameter)
      // http://drupal.org/node/224333#system_admin_menu_block_access --- Hide empty menu categories with access callback


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
      // http://drupal.org/node/224333#hook_block_list_alter --- Added hook_block_list_alter()


    // Comment
      // N/A - http://drupal.org/node/224333#comment_load_multiple --- New comment_load_multiple()
      // N/A - http://drupal.org/node/224333#comment_save_refactor --- comment_save() now supports programatic saving

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
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]comment_validate\s*\(',
        '#warning_callback' => '_coder_review_7x_comment_validate_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]comment_node_url\s*\(',
        '#warning_callback' => '_coder_review_7x_comment_node_url_warning',
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
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => 'admin\/settings\/filters(\/.*?)?',
        '#warning_callback' => '_coder_review_7x_filter_urls_warning',
      ),


    // Taxonomy
      // N/A - http://drupal.org/node/224333#taxonomy_term_load_multiple --- taxonomy_term_load() and taxonomy_term_load_multiple()
      // N/A - http://drupal.org/node/224333#taxonomy_hooks --- New hooks: hook_taxonomy_term_load(), hook_taxonomy_term_insert(), hook_taxonomy_term_update(), hook_taxonomy_term_delete() and hook_taxonomy_vocabulary_load(), hook_taxonomy_vocabulary_insert(), hook_taxonomy_vocabulary_update(), hook_taxonomy_vocabulary_delete()
      // N/A - http://drupal.org/node/224333#vocabulary_load_multiple --- Added taxonomy_vocabulary_load_multiple()

      array(
        '#type' => 'regex',
        '#source' => 'php',
        '#value' => '[\s\(]taxonomy_get_tree\s*\(\s*(' . $argex . '\s*,\s*){2}(\$depth|-1)\s*[,\)]',
        '#warning_callback' => '_coder_review_7x_taxonomy_get_tree_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '^(select\s+.*\s+from\s+' . $taxonomy_tables .  '|insert\s+into\s+' .  $taxonomy_tables . '|update\s+' .  $taxonomy_tables . '\s+set|delete\s+from\s+' . $taxonomy_tables . ')',
        '#warning_callback' => '_coder_review_7x_taxonomy_tables_warning',
      ),
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
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => 'taxonomy\/term\/\d+(\+\d+|\,\d+|\/\d+)',
        '#warning_callback' => '_coder_review_7x_taxonomy_paths_warning',
      ),


    // Javascript
      // N/A - http://drupal.org/node/224333#jquery_ui --- jQuery UI (1.7) was added into core
      // N/A - http://drupal.org/node/224333#attached_js --- Attached JavaScript and CSS for forms
      // N/A - http://drupal.org/node/224333#drupal_add_library --- Ability to add multiple JavaScript/CSS files at once
      // N/A - http://drupal.org/node/224333#drupal_add_js_css_reset --- Ability to reset JavaScript/CSS

      // http://drupal.org/node/224333#drupal-behaviors --- Changed Drupal.behaviors to objects having the methods 'attach' and 'detach'

      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_add_js\s*\((' . $argex . '\s*,\s*){2,}(' . $argex . ')\s*\)',
        '#warning_callback' => '_coder_review_7x_drupal_add_js_parameters_warning',
      ),

      // http://drupal.org/node/224333#drupal_add_js_weight --- Replace 'core', 'module' and 'theme' with 'file' in drupal_add_js()
      // http://drupal.org/node/224333#hook_js_alter --- New hook_js_alter to alter JavaScript
      // http://drupal.org/node/224333#javascript_compatibility --- JavaScript should be compatible with other libraries than jQuery
      // http://drupal.org/node/224333#drupal_add_js_external --- External JavaScript can now be referenced through drupal_add_js()
      // http://drupal.org/node/224333#local_settings_behaviors --- Settings passed locally to JavaScript Behaviors
      // http://drupal.org/node/224333#jquery_13 --- jQuery 1.3.x


    // CSS
      // N/A - http://drupal.org/node/224333#attached_js --- Attached JavaScript and CSS for forms
      // N/A - http://drupal.org/node/224333#drupal_add_css_inline --- Inline cascading style sheets from drupal_add_css()
      // N/A - http://drupal.org/node/224333#drupal_add_library --- Ability to add multiple JavaScript/CSS files at once
      // N/A - http://drupal.org/node/224333#drupal_add_js_css_reset --- Ability to reset JavaScript/CSS

      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_add_css\s*\((' . $argex . '\s*,\s*){2,}(' . $argex . ')\s*\)',
        '#warning_callback' => '_coder_review_7x_drupal_add_css_parameters_warning',
      ),


    // Theming
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_rebuild_theme_registry\s*\(',
        '#warning_callback' => '_coder_review_7x_theme_rebuild_function_warning',
      ),
      // http://drupal.org/node/224333#theme_page
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]theme\s*\(\s*[\'"]page[\'"]',
        '#warning_callback' => '_coder_review_7x_theme_page_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]theme\s*\(\s*[\'"]closure[\'"]',
        '#warning_callback' => '_coder_review_7x_hook_footer_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(closure|footer)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_footer_warning',
      ),
      // @NOTE: no simpletest for this because of #filename setting.
      array(
        '#type' => 'regex',
        '#value' => '\$closure[^a-zA-Z_]',
        '#filename' => array('tpl.php'),
        '#warning_callback' => '_coder_review_7x_closure_warning',
      ),
      // http://drupal.org/node/224333#element_theme_properties --- Element theming properties used by drupal_render() have changed
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_elements$',
        '#value' => '[\'"]#type[\'"]',
        '#warning_callback' => '_coder_review_7x_element_theme_properties_warning',
      ),

      // http://drupal.org/node/224333#drupal_render_children --- Element theme functions should call drupal_render_children()



    // Form API
      // http://drupal.org/node/224333#poundtheme --- #theme recommended for specifying theme function
      // http://drupal.org/node/224333#node_form --- Simpler checking for the node form during hook_form_alter()
      // http://drupal.org/node/224333#process-functions --- Some #process functions have been renamed
      // http://drupal.org/node/224333#markup --- Use '#markup' not '#value' for markup.
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_execute\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_execute_renamed_warning',
      ),
      // http://drupal.org/node/224333#unrendered --- drupal_get_form() returns an array
      // http://drupal.org/node/224333#ahah_changes --- AHAH Processing has changed; new 'callback' member of the array, and the callback must be rewritten


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
      // http://drupal.org/node/224333#file_scan_directory_property_names --- file_scan_directory() now uses same property names as file_load()
      // http://drupal.org/node/224333#file_check_directory_recursive --- file_check_directory() will now recursively create directories


    // User API
      // N/A - http://drupal.org/node/224333#specified_ids --- Save new users and nodes with specified IDS

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
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]user_load\s*\(\s*(array\s*\(|\(\s*array\s*\))',
        '#warning_callback' => '_coder_review_7x_user_load_warning',
      ),


    // Node API
      // N/A - http://drupal.org/node/224333#delete_multiple --- Add node_delete_multiple()
      // N/A - http://drupal.org/node/224333#specified_ids --- Save new users and nodes with specified IDS

      // No link - node_revisions table name changed.
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '^(select\s+.*\s+from\s+{node_revisions}|insert\s+into\s+{node_revisions}|update\s+{node_revisions}\s+set|delete\s+from\s+{node_revisions})',
        '#warning_callback' => '_coder_review_7x_node_revisions_table_warning',
      ),

      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => 'function\s+[a-z0-9_]+_(link)\s*\(\s*([\'"](node|taxonomy terms)[\'"]|' . $argex . '\s*,\s*\$node\s*,)',
        '#warning_callback' => '_coder_review_7x_node_links_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' =>'[\s\(]node_load\s*\(\s*(\$param\s*[,\)]|array\s*\()',
        '#warning_callback' => '_coder_review_7x_node_load_warning',
      ),
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
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]node_view\s*\(',
        '#warning_callback' => '_coder_review_7x_node_view_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_nodeapi$',
        '#value' => '[\'"]rss item[\'"]',
        '#warning_callback' => '_coder_review_7x_node_build_rss_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]node_get_types\s*\(',
        '#warning_callback' => '_coder_review_7x_node_type_get_functions_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]\$node->build_mode',
        '#warning_callback' => '_coder_review_7x_build_mode_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]NODE_BUILD_PREVIEW',
        '#warning_callback' => '_coder_review_7x_node_build_preview_warning',
      ),


    // Multilingual
      // http://drupal.org/node/224333#locale_context --- Added string context support to t() and format_plural(), changed parameters


    // Misc
      // N/A - http://drupal.org/node/224333#sticky_headers --- Make sticky tableheaders optional
      // N/A - http://drupal.org/node/224333#implementation_hook_comment --- Commenting style - use 'Implement hook_foo().' when documenting hooks.

      array(
        '#type' => 'regex',
        '#value' => '[\s\(]book_toc\s*\(\s*' . $argex . '\s*,\s*(array\s*\(|(\$?)[a-zA-Z_]+\s*,\s*\d+)',
        '#warning_callback' => '_coder_review_7x_book_toc_warning',
      ),
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
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]actions_synchronize\s*\((\$actions|[^,]+,)',
        '#warning_callback' => '_coder_review_7x_actions_synchronize_warning',
      ),
      // http://drupal.org/node/224333#distauth --- Login validation change for distributed authentication modules
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

function _coder_review_7x_cache_implementation_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // @NOTE: no simpletest for this.
  $filename = $coder_args['#filename'];
  if ($filename == 'includes/cache-install.inc' || $filename == 'includes/cache.inc') {
    return;
  }
  if (file_exists($filename)) {
    // Only do it for php files.
    if (substr($filename, -7) == '.module' || substr($filename, -4) == '.php' || substr($filename, -4) == '.inc') {
      if ($lines = file($filename)) {
        foreach ($lines as $lineno => $line) {
          if (preg_match('/function\s+(cache_set|cache_get|cache_clear_all)\s*\(/', $line)) {
            $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
            $tmprule = $rule;
            $tmprule['#warning_callback'] = '_coder_review_7x_cache_implementation_warning';
            _coder_review_error($results, $tmprule, $severity_name, $lineno, $line);
          }
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

function _coder_review_7x_node_view_warning() {
  return array(
    '#warning' => t('!node_view() has been removed and !node_build() should be used instead.',
      array(
        '!node_view' => theme('drupalapi', 'node_view', 6),
        '!node_build()' => theme('drupalapi', 'node_build', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_view',
  );
}

function _coder_review_7x_comment_validate_warning() {
  return array(
    '#warning' => t('!comment_validate() has been removed and !comment_form_validate() should be used instead.',
      array(
        '!comment_validate' => theme('drupalapi', 'comment_validate', 6),
        '!comment_form_validate()' => theme('drupalapi', 'comment_form_validate', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#comment_validate_removed',
  );
}

function _coder_review_7x_comment_node_url_warning() {
  return array(
    '#warning' => t('!comment_node_url() has been removed. Use "node/" .  $comment->nid to build URLs instead.',
      array(
        '!comment_node_url' => theme('drupalapi', 'comment_node_url', 6),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#comment_node_url',
  );
}

function _coder_review_7x_node_load_warning() {
  return array(
    '#warning' => t('!node_load() now only takes nid and vid as parameters.',
      array(
        '!node_load' => theme('drupalapi', 'node_load', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_load_multiple',
  );
}

function _coder_review_7x_hook_perm_warning() {
  return array(
    '#warning' => t('!hook_perm() has been renamed to !hook_permission().',
      array(
        '!hook_perm' => theme('drupalapi', 'hook_perm', 6),
        '!hook_permission' => theme('drupalapi', 'hook_permission', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook-permission',
  );
}

function _coder_review_7x_absolute_includes_warning() {
  return array(
    '#warning' => t('Use absolute path, constructed from DRUPAL_ROOT, when including a file.'),
    '#link' => 'http://drupal.org/node/224333#absolute_includes',
  );
}

function _coder_review_7x_filter_urls_warning() {
  return array(
    '#warning' => t('Administrative URLs in the Filter module have been renamed from plural to singular.'),
    '#link' => 'http://drupal.org/node/224333#filter_urls',
  );
}

function _coder_review_7x_taxonomy_get_tree_warning() {
  return array(
    '#warning' => t('!taxonomy_get_tree()\'s $depth and $max_depth parameters have changed positions.',
      array(
        '!taxonomy_get_tree' => theme('drupalapi', 'taxonomy_get_tree', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_get_tree',
  );
}

function _coder_review_7x_taxonomy_paths_warning() {
  return array(
    '#warning' => t('Support for taxonomy term paths with multiple tids and depth specification has been removed.'),
    '#link' => 'http://drupal.org/node/224333#taxonomy_paths',
  );
}

function _coder_review_7x_book_toc_warning() {
  return array(
    '#warning' => t('!book_toc()\'s $depth_limit and $exclude parameters have changed positions.',
      array(
        '!book_toc' => theme('drupalapi', 'book_toc', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#book_toc_parameters',
  );
}

function _coder_review_7x_node_type_get_functions_warning() {
  return array(
    '#warning' => t('!node_get_types() function has been replaced by node_type_get_$op() functions.',
      array(
        '!node_get_types' => theme('drupalapi', 'node_get_types', 6),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_type_get_functions',
  );
}

function _coder_review_7x_node_build_rss_warning() {
  return array(
    '#warning' => t('Removed $op "rss item" from !hook_nodeapi() in favor of NODE_BUILD_RSS.',
      array(
        '!hook_nodeapi()' => theme('drupalapi', 'hook_nodeapi', '7'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_build_rss',
  );
}

function _coder_review_7x_build_mode_warning() {
  return array(
    '#warning' => t('$node->build_mode property removed.'),
    '#link' => 'http://drupal.org/node/224333#build_mode',
  );
}

function _coder_review_7x_node_build_preview_warning() {
  return array(
    '#warning' => t('NODE_BUILD_PREVIEW has been replaced by the $node->in_preview boolean flag.'),
    '#link' => 'http://drupal.org/node/224333#build_mode',
  );
}

function _coder_review_7x_node_links_warning() {
  return array(
    '#warning' => t('Node and taxonomy links are no longer emitted by !hook_link() and !hook_link_alter().',
      array(
        '!hook_link()' => theme('drupalapi', 'hook_link', 7),
        '!hook_link_alter()' => theme('drupalapi', 'hook_link_alter', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_links',
  );
}

function _coder_review_7x_drupal_uninstall_modules_warning() {
  return array(
    '#warning' => t('!drupal_uninstall_module() has been renamed to !drupal_uninstall_modules() and now takes an array of module names.',
      array(
        '!drupal_uninstall_module()' => theme('drupalapi', 'drupal_uninstall_module', 6),
        '!drupal_uninstall_modules()' => theme('drupalapi', 'drupal_uninstall_modules', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_uninstall_modules',
  );
}

function _coder_review_7x_drupal_http_request_parameters_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_http_request() have changed.',
      array(
        '!drupal_http_request()' => theme('drupalapi', 'drupal_http_request', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_http_request_parameters',
  );
}

function _coder_review_7x_moved_statistics_settings_warning() {
  return array(
    '#warning' => t('Statistics settings have moved from "admin/reports/settings" to "admin/settings/statistics".'),
    '#link' => 'http://drupal.org/node/224333#moved_statistics_settings',
  );
}

function _coder_review_7x_drupal_set_html_head_warning() {
  return array(
    '#warning' => t('!drupal_set_html_head() has been renamed to !drupal_add_html_head().',
      array(
        '!drupal_set_html_head()' => theme('drupalapi', 'drupal_set_html_head', 6),
        '!drupal_add_html_head()' => theme('drupalapi', 'drupal_add_html_head', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_html_head',
  );
}

function _coder_review_7x_php_eval_warning() {
  return array(
    '#warning' => t("!drupal_eval() has been renamed to !php_eval() and should be wrapped in a module_exists('php') check.",
      array(
        '!drupal_eval()' => theme('drupalapi', 'drupal_eval', 6),
        '!php_eval()' => theme('drupalapi', 'php_eval', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#php_eval',
  );
}

function _coder_review_7x_module_rebuild_cache_warning() {
  return array(
    '#warning' => t('!module_rebuild_cache() has been renamed to !system_get_module_data().',
      array(
        '!module_rebuild_cache()' => theme('drupalapi', 'module_rebuild_cache', 6),
        '!system_get_module_data()' => theme('drupalapi', 'system_get_module_data', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#system_get_module_data',
  );
}

function _coder_review_7x_system_theme_data_warning() {
  return array(
    '#warning' => t('!system_theme_data() has been renamed to !system_get_theme_data().',
      array(
        '!system_theme_data()' => theme('drupalapi', 'system_theme_data', 6),
        '!system_get_theme_data()' => theme('drupalapi', 'system_get_theme_data', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#system_get_module_data',
  );
}

function _coder_review_7x_drupal_set_content_warning() {
  return array(
    '#warning' => t('!drupal_set_content() has been renamed to !drupal_add_region_content().',
      array(
        '!drupal_set_content()' => theme('drupalapi', 'drupal_set_content', 6),
        '!drupal_add_region_content()' => theme('drupalapi', 'drupal_add_region_content', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_content',
  );
}

function _coder_review_7x_drupal_get_content_warning() {
  return array(
    '#warning' => t('!drupal_get_content() has been renamed to !drupal_get_region_content().',
      array(
        '!drupal_get_content()' => theme('drupalapi', 'drupal_get_content', 6),
        '!drupal_get_region_content()' => theme('drupalapi', 'drupal_get_region_content', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_content',
  );
}

function _coder_review_7x_http_header_functions_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_set_header() have changed.',
      array(
        '!drupal_set_header()' => theme('drupalapi', 'drupal_set_header', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#http_header_functions',
  );
}

function _coder_review_7x_cache_implementation_warning() {
  return array(
    '#warning' => t('Alternative cache implementations have changed.'),
    '#link' => 'http://drupal.org/node/224333#cache_implementation',
  );
}

// No link!
function _coder_review_7x_node_revisions_table_warning() {
  return array(
    '#warning' => t("The 'node_revisions' table has been renamed to 'node_revision'."),
    //'#link' => 'http://drupal.org/node/224333#taxonomy_tables',
  );
}

function _coder_review_7x_user_load_warning() {
  return array(
    '#warning' => t('!user_load() now only takes a user ID as its argument.',
      array(
        '!user_load()' => theme('drupalapi', 'user_load', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#user_load_multiple',
  );
}

function _coder_review_7x_hook_footer_warning() {
  return array(
    '#warning' => t('!hook_footer() and !theme_closure() were removed.',
      array(
        '!hook_footer()' => theme('drupalapi', 'hook_footer', 6),
        '!theme_closure()' => theme('drupalapi', 'theme_closure', 6),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_footer',
  );
}

function _coder_review_7x_closure_warning() {
  return array(
    '#warning' => t('$closure has been changed to $page_bottom.'),
    '#link' => 'http://drupal.org/node/224333#hook_footer',
  );
}

function _coder_review_7x_theme_page_warning() {
  return array(
    '#warning' => t('Instead of theme("page") use !drupal_set_page_content()',
      array(
        '!drupal_set_page_content()' => theme('drupalapi', 'drupal_set_page_content()', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#theme_page',
  );
}

function _coder_review_7x_drupal_add_js_parameters_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_add_js() have changed.',
      array(
        '!drupal_add_js()' => theme('drupalapi', 'drupal_add_js()', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_add_js_options',
  );
}

function _coder_review_7x_drupal_add_css_parameters_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_add_css() have changed.',
      array(
        '!drupal_add_css()' => theme('drupalapi', 'drupal_add_css()', 7),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_add_js_options',
  );
}
