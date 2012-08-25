<?php
/**
 * @file
 * This include file implements coder_review functionality for 4.6 -> 4.7 upgrades.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_47_reviews() {
  $rules[] = array(
    '#type' => 'regex',
    '#value' => 'function\s+\w+(_node_name|_node_type)\s*\(',
    '#warning' => 'coder_review_47_hook_node_info_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]node_load\s*\(\s*array\s*\(',
    '#warning' => '_coder_review_47_node_load_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]node_list\s*\(',
    '#warning' => '_coder_review_47_node_list_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]module_get_node_name\s*\(',
    '#warning' => '_coder_review_47_module_get_name_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]format_name\s*\(',
    '#warning' => '_coder_review_47_format_name_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '^\s*(taxonomy_save_vocabulary|taxonomy_save_term)\s*\(',
    '#warning' => 'check the return and display a status message',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]message_access\s*\(',
    '#warning' => '_coder_review_47_message_access_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(](strlen|strtolower|strtoupper|substr|ucfirst)\s*\(',
    '#warning' => 'in most cases, replace the string function with the drupal_ equivalent string functions',
    '#severity' => 'minor',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]conf_url_rewrite\s*\(',
    '#warning' => '_coder_review_47_conf_url_rewrite_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]node_delete\s*\(\s*array\s*\(',
    '#warning' => '_coder_review_47_node_delete_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#source' => 'allphp',
    '#value' => '[\s\(]variable_get\s*\(\s*[\'"]+file_directory_temp[\'"]+\s*,',
    '#warning' => '_coder_review_47_file_directory_temp_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#source' => 'allphp',
    '#value' => '[\s\(]variable_get\s*\(\s*[\'"]+file_directory_path[\'"]+\s*,',
    '#warning' => '_coder_review_47_file_directory_path_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]array2object\s*\(',
    '#warning' => '_coder_review_47_array2object_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => 'function\s+\w+_onload\s*\(',
    '#warning' => '_coder_review_47_hook_onload_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]node_validate_title\s*\(',
    '#warning' => '_coder_review_47_node_validate_title_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]tablesort_pager\s*\(',
    '#warning' => '_coder_review_47_tablesort_pager_warning',
  );
  $rules[] = array(
    '#type' => 'regex',
    '#value' => '[\s\(]form_(textfield|radio|group|select|checkbox|textarea)\s*\(',
    '#warning' => 'replace form_ functions with the forms api',
  );

  $review = array(
    '#title' => 'Converting 4.6.x modules to 4.7.x',
    '#link' => 'http://drupal.org/node/22218',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade47' => $review);
}

/**
 * Define the warning callbacks.
 */

function _coder_review_47_hook_node_info_warning() {
  return _t('implement !hook_node_info() to create a module which defines node type(s)',
    array(
      '!hook_node_info' => _drupalapi('hook_node_info', 4.7),
    )
  );
}

function _coder_review_47_node_load_warning() {
  return _t('use !node_load($nid) instead of !node_load(array(\'nid\' => $nid))',
    array(
      '!node_load' => _drupalapi('node_load', 4.7),
    )
  );
}

function _coder_review_47_node_list_warning() {
  return _t('!node_list() became node_get_types and now returns an associative array about node types',
    array(
      '!node_list' => _drupalapi('node_list', 4.7),
    )
  );
}

function _coder_review_47_module_get_name_warning() {
  return _t('!module_get_node_name() deprecated and now handled by !node_get_base().',
    array(
      '!module_get_node_name' => _drupalapi('module_get_node_name', 4.6),
      '!node_get_base' => _drupalapi('node_get_base', 4.7),
    )
  );
}

function _coder_review_47_format_name_warning() {
  return _t('!format_name() was renamed to !theme_username()',
    array(
      '!format_name' => _drupalapi('format_name', 4.6),
      '!theme_username' => _drupalapi('theme_username', 4.7),
    )
  );
}

function _coder_review_47_message_access_warning() {
  return _t('!message_access() was removed, replace with a nice case error message',
    array(
      '!message_access' => _drupalapi('message_access', 4.6),
    )
  );
}

function _coder_review_47_conf_url_rewrite_warning() {
  return _t('!conf_url_rewrite() became !custom_url_rewrite()',
    array(
      '!conf_url_rewrite' => _drupalapi('conf_url_rewrite', 4.6),
      '!custom_url_rewrite' => _drupalapi('custom_url_rewrite', 4.7),
    )
  );
}

function _coder_review_47_node_delete_warning() {
  return _t('use !node_delete($nid) instead of !node_delete(array(\'nid\' => $nid))',
    array(
      '!node_delete' => _drupalapi('node_delete', 4.7),
    )
  );
}

function _coder_review_47_file_directory_temp_warning() {
  return _t('use !file_directory_temp() function instead of variable',
    array(
      '!file_directory_temp' => _drupalapi('file_directory_temp', 4.7),
    )
  );
}

function _coder_review_47_file_directory_path_warning() {
  return _t('use !file_directory_path() function instead of variable',
    array(
      '!file_directory_path' => _drupalapi('file_directory_path', 4.7),
    )
  );
}

function _coder_review_47_array2object_warning() {
  return _t('!array2object() replaced by native PHP type conversion (typecase to (object)',
    array(
      '!array2object' => _drupalapi('array2object', 4.6),
    )
  );
}

function _coder_review_47_hook_onload_warning() {
  return _t('!hook_onload() replaced by javascript addLoadEvent()',
    array(
      '!hook_onload' => _drupalapi('hook_onload', 4.6),
    )
  );
}

function _coder_review_47_node_validate_title_warning() {
  return _t('!node_validate_title() was removed',
    array(
      '!node_validate_title' => _drupalapi('node_validate_title', 4.6),
    )
  );
}

function _coder_review_47_node_tablesort_pager_warning() {
  return _t('!tablesort_pager() was removed',
    array(
      '!tablesort_pager' => _drupalapi('tablesort_pager', 4.6),
    )
  );
}
