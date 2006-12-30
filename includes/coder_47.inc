<?php
// $Id$

/** @file
 * This include file implements coder functionality for 4.6 -> 4.7 upgrades
 *
 * Todo: The rules for this review have not yet been defined.
 */ 

function coder_47_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => 'function\s+\w+(_node_name|_node_type)\s*\(',
      '#warning' => t('implement hook_node_info to create a module which defines node type(s)'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\snode_load\s*\(\s*array\s*\(',
      '#warning' => t('use node_load($nid) instead of node_load(array(\'nid\' => $nid))'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\snode_list\s*\(',
      '#warning' => t('node_list() became node_get_types and now returns an associative array about node types'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\smodule_get_node_name\s*\(',
      '#warning' => t('module_get_node_name deprecated and now handled by node_get_base function.'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\sformat_name\s*\(',
      '#warning' => t('format_name() was renamed to theme_username()'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '^\s*(taxonomy_save_vocabulary|taxonomy_save_term)\s*\(',
      '#warning' => t('check the return and display a status message'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\smessage_access\s*\(',
      '#warning' => t('message_access() was removed, replace with a nice case error message'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s(strlen|strtolower|strtoupper|substr|ucfirst)\s*\(',
      '#warning' => t('in most cases, replace the string funciton with the drupal_ equivalent string functions'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\sconf_url_rewrite\s*\(',
      '#warning' => t('conf_url_rewrite() became custom_url_rewrite()'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\snode_delete\s*\(\s*array\s*\(',
      '#warning' => t('use node_delete($nid) instead of node_delete(array(\'nid\' => $nid))'),
    ),
    array(
      '#type' => 'regex',
      '#original' => 1,
      '#value' => '\svariable_get\s*\(\s*[\'"]+file_directory_temp[\'"]+\s*,',
      '#warning' => t('use file_directory_temp()'),
    ),
    array(
      '#type' => 'regex',
      '#original' => 1,
      '#value' => '\svariable_get\s*\(\s*[\'"]+file_directory_path[\'"]+\s*,',
      '#warning' => t('use file_directory_path()'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\sarray2object\s*\(',
      '#warning' => t('array2object replaced by native PHP type conversion (typecase to (object)'),
    ),
    array(
      '#type' => 'regex',
      '#value' => 'function\s+\w+_onload\s*\(',
      '#warning' => t('hook_onload replaced by addLoadEvent()'),
    ),
    array(
      '#type' => 'regex',
      '#value' => 'function\s+\w+_search_item\s*\(',
      '#warning' => t('hook_search_item replaced by hook_search_page'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\snode_validate_title\s*\(',
      '#warning' => t('node_validate_title() was removed'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\stablesort_pager\s*\(',
      '#warning' => t('tablesort_pager() was removed'),
    ),
    array(
      '#type' => 'regex',
      '#value' => '\sform_(textfield|radio|group|select|checkbox|textarea)\s*\(',
      '#warning' => t('replace form_ functions with the forms api'),
    ),
  );
  $review = array(
    '#title' => t('Converting 4.6.x modules to 4.7.x'),
    '#link' => 'http://drupal.org/node/22218',
    '#rules' => $rules,
  );
  return array('upgrade47' => $review);
}
