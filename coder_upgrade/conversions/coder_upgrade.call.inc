<?php
// $Id$

/**
 * @file
 * Function call conversion routine file for the coder_upgrade module.
 * These routines upgrade function calls using the grammar parser.
 *
 * The functions in this conversion routine file correspond to topics in the
 * category roadmap at http://drupal.org/node/394070 that are marked with a
 * green check mark in the Upgrade column.
 *
 * Copyright 2009-10 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * The upgrades to these functions are documented at the following urls.
 *
 * System
 * http://drupal.org/node/224333#registry
 * http://drupal.org/node/224333#drupal_set_session (REVERTED)
 * http://drupal.org/node/224333#time
 * http://drupal.org/node/224333#rebuild_functions
 * http://drupal.org/node/224333#drupal_uninstall_modules
 * http://drupal.org/node/224333#module_implements_not_module_list
 * http://drupal.org/node/224333#drupal_http_request_parameters
 * http://drupal.org/node/224333#system_get_module_data
 * http://drupal.org/node/224333#static_variable_api (NOT IN THIS FUNCTION)
 * http://drupal.org/node/224333#drupal_set_html_head
 * http://drupal.org/node/224333#php_eval
 * http://drupal.org/node/224333#http_header_functions
 * http://drupal.org/node/224333#drupal_set_content
 * http://drupal.org/node/224333#time_limit
 *
 *
 * Database
 * http://drupal.org/node/224333#schema_ret
 *
 *
 * Menu
 * http://drupal.org/node/224333#comment_load (DUP Comments)
 *
 *
 * Comments
 * http://drupal.org/node/224333#comment_load
 * http://drupal.org/node/224333#comment_validate_removed
 * http://drupal.org/node/224333#comment_node_url
 *
 *
 * Input Sanitization and Input Formats
 * http://drupal.org/node/224333#check_markup_params
 * http://drupal.org/node/224333#drupal_set_title
 * http://drupal.org/node/224333#hook_filter_info (NOT IN THIS FUNCTION)
 * http://drupal.org/node/224333#filter_formats_parameters
 *
 *
 * Taxonomy
 * http://drupal.org/node/224333#taxonomy_get_tree
 * http://drupal.org/node/224333#taxonomy_crud
 *
 *
 * Javascript
 * http://drupal.org/node/224333#drupal_add_js_options
 * http://drupal.org/node/224333#drupal_add_js_weight (Included with above)
 * http://drupal.org/node/224333#rename-drupal-to-js
 *
 *
 * CSS
 * http://drupal.org/node/224333#drupal_add_js_options (DUP Javascript)
 * http://drupal.org/node/224333#drupal_add_css_weight (Should have a DUP in Javascript)
 *
 *
 * Theming
 * http://drupal.org/node/224333#rebuild_functions (DUP System)
 * http://drupal.org/node/224333#theme_changes
 *
 *
 * Form API
 * http://drupal.org/node/224333#drupal_execute_drupal_form_submit
 *
 *
 * File API
 * http://drupal.org/node/224333#file_set_status
 * http://drupal.org/node/224333#preg_match
 *
 *
 * User API
 * http://drupal.org/node/224333#user_cancel (ALSO in convert_functions)
 * http://drupal.org/node/224333#user_authenticate
 *
 *
 * Multi-lingual
 * http://drupal.org/node/224333#locale_context
 *
 *
 * Miscellaneous
 * http://drupal.org/node/224333#book_toc_parameters
 * http://drupal.org/node/224333#referer_uri
 * http://drupal.org/node/224333#drupal_clone
 * http://drupal.org/node/224333#actions_synchronize
 * http://drupal.org/node/224333#url_is_external
 */

/**
 * Implements hook_upgrade_call_actions_synchronize_alter().
 */
function coder_upgrade_upgrade_call_actions_synchronize_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $count = $item->parameters->count();
  if ($count > 0) {
    $item->deleteParameter();
  }
}

/**
 * Implements hook_upgrade_call_book_toc_alter().
 */
function coder_upgrade_upgrade_call_book_toc_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // Adjust parameters.
  $count = $item->parameters->count();
  if ($count > 2) {
    // Switch places.
    $p1 = $item->getParameter(1);
    $p2 = $item->getParameter(2);
    $item->setParameter(1, $p2);
    $item->setParameter(2, $p1);
  }
  // Remove default parameter.
  if ($count == 3) {
    $value = $item->printParameter(2);
    cdp("value = $value");
    if ($value == 'array()') {
      $item->deleteParameter(2);
    }
  }
}

/**
 * Implements hook_upgrade_call_check_markup_alter().
 */
function coder_upgrade_upgrade_call_check_markup_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  if ($item->parameters->count() > 2) {
    $editor->insertParameter($item, 2, '$langcode = \'\' /* TODO Set this variable. */');
  }
}

/**
 * Implements hook_upgrade_call__comment_load_alter().
 */
function coder_upgrade_upgrade_call__comment_load_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // TODO The comment_wildcard change in hook_menu.
  $name['value'] = 'comment_load';
}

/**
 * Implements hook_upgrade_call_comment_node_url_alter().
 */
function coder_upgrade_upgrade_call_comment_node_url_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $parent = $item->parent;
  $temp = $editor->statementsToText($parent);
  $from = '@comment_node_url()@';
  $to = "'comment/' . \$comment->cid";
  $temp = preg_replace($from, $to, $temp);
  $temp = $editor->textToStatements($temp);
  $parent->data = $temp->getElement(0);
}

/**
 * Implements hook_upgrade_call_comment_validate_alter().
 */
function coder_upgrade_upgrade_call_comment_validate_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'comment_form_validate';
  $editor->setParameters($item, array('$form', '$form_state /* TODO Set these variables. */'));
}

/**
 * Implements hook_upgrade_call_db_add_field_alter().
 */
function coder_upgrade_upgrade_call_db_add_field_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Add a new field to a table.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_add_index_alter().
 */
function coder_upgrade_upgrade_call_db_add_index_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Add an index.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_add_primary_key_alter().
 */
function coder_upgrade_upgrade_call_db_add_primary_key_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Add a primary key.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_add_unique_key_alter().
 */
function coder_upgrade_upgrade_call_db_add_unique_key_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Add a unique key.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_change_field_alter().
 */
function coder_upgrade_upgrade_call_db_change_field_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Change a field definition.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_create_table_alter().
 */
function coder_upgrade_upgrade_call_db_create_table_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.inc Create a new table from a Drupal table definition.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_create_table_sql_alter().
 */
function coder_upgrade_upgrade_call_db_create_table_sql_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Generate SQL to create a new table from a Drupal schema definition.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_drop_field_alter().
 */
function coder_upgrade_upgrade_call_db_drop_field_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Drop a field.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_drop_index_alter().
 */
function coder_upgrade_upgrade_call_db_drop_index_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Drop an index.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_drop_primary_key_alter().
 */
function coder_upgrade_upgrade_call_db_drop_primary_key_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Drop the primary key.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_drop_table_alter().
 */
function coder_upgrade_upgrade_call_db_drop_table_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Drop a table.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_drop_unique_key_alter().
 */
function coder_upgrade_upgrade_call_db_drop_unique_key_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Drop a unique key.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_field_names_alter().
 */
function coder_upgrade_upgrade_call_db_field_names_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.inc Return an array of field names from an array of key/index column specifiers.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_field_set_default_alter().
 */
function coder_upgrade_upgrade_call_db_field_set_default_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Set the default value for a field.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_field_set_no_default_alter().
 */
function coder_upgrade_upgrade_call_db_field_set_no_default_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Set a field to have no default value.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_db_rename_table_alter().
 */
function coder_upgrade_upgrade_call_db_rename_table_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // includes/database.pgsql.inc Rename a table.
  $item->deleteParameter();
}

/**
 * Implements hook_upgrade_call_drupal_add_css_alter().
 */
function coder_upgrade_upgrade_call_drupal_add_css_alter(&$item, &$reader) { // DONE (UPDATED)
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // A similar comment to that in 'drupal_add_js' below applies here.
  cdp($item->parameters->print_r());

  $count = $item->parameters->count();
  if ($count == 1) {
    return;
  }
  if ($count == 2) {
    $type = trim($item->printParameter(1), "'\"");
    if ($type == 'module') {
      $item->deleteParameter(1);
      return;
    }
  }

  // Insert a new weight parameter.
  $type = trim($item->printParameter(1), "'\"");
  $weight = coder_upgrade_css_weight($type);
  $editor->insertParameter($item, 2, "$weight");

  // Arrayitize the parameters.
  $keys = array('type', 'weight', 'media', 'preprocess');
  $defaults = array("'module'", 'CSS_DEFAULT', "'all'", 'TRUE');
  $string = $editor->arrayitize($item, 1, $keys, $defaults);
  $string = preg_replace('@[\'"]theme[\'"]@', "'file'", $string); // Could be deleted.

  if ($string != 'array()') {
    $temp = $editor->expressionToStatement($string);
    $temp->getElement(0)->multiline = 0;
    cdp($temp->print_r());
    $item->setParameter(1, $temp);
  }
}

function coder_upgrade_css_weight($type) {
  switch ($type) {
    case 'module':
      return 'CSS_DEFAULT';

    case 'theme':
      return 'CSS_THEME';

    default:
      return 'CSS_DEFAULT';
  }
}

/**
 * Implements hook_upgrade_call_drupal_add_js_alter().
 */
function coder_upgrade_upgrade_call_drupal_add_js_alter(&$item, &$reader) { // DONE (UPDATED)
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  /*
   * With
   * drupal_add_js('misc/collapse.js', 'core', 'header', FALSE, TRUE, TRUE);
   * we will output
   * drupal_add_js('misc/collapse.js', array('type' => 'file', 'weight' => JS_LIBRARY));
   * which is correct, although the function will also accept
   * drupal_add_js('misc/collapse.js', array('weight' => JS_LIBRARY));
   * The example begs the question why someone would have included all
   * the default parameters.
   *
   * A type of 'core', 'module' or 'theme' all convert to 'file' which is
   * the new default. The weight parameter then corresponds to the old type.
   */
  cdp($item->parameters->print_r());

  $count = $item->parameters->count();
  if ($count == 1) {
    return;
  }
  if ($count == 2) {
    $type = trim($item->printParameter(1), "'\"");
    if ($type == 'module') {
 // if (in_array($type, array('module', ''))) {
      $item->deleteParameter(1);
      return;
    }
    elseif (in_array($type, array('core', 'theme'))) {
      // Add a default value for the scope parameter (removed below).
      $editor->insertParameter($item, 2, "'header'");
    }
  }

  // Insert a new weight parameter.
  $type = trim($item->printParameter(1), "'\"");
  $weight = coder_upgrade_js_weight($type);
  $editor->insertParameter($item, 3, "$weight");

  // Arrayitize the parameters.
  $keys = array('type', 'scope', 'weight', 'defer', 'cache', 'preprocess');
  $defaults = array("'module'", "'header'", 'JS_DEFAULT', 'FALSE', 'TRUE', 'TRUE');
  $string = $editor->arrayitize($item, 1, $keys, $defaults);
  $string = preg_replace('@[\'"](core|theme)[\'"]@', "'file'", $string); // Could be deleted.

  if ($string != 'array()') {
    $temp = $editor->expressionToStatement($string);
    $temp->getElement(0)->multiline = 0;
    cdp($temp->print_r());
    $item->setParameter(1, $temp);
  }
}

function coder_upgrade_js_weight($type) {
  switch ($type) {
    case 'core':
      return 'JS_LIBRARY';

    case 'module':
      return 'JS_DEFAULT';

    case 'theme':
      return 'JS_THEME';

    default:
      return 'JS_DEFAULT';
  }
}

/**
 * Implements hook_upgrade_call_drupal_clone_alter().
 */
function coder_upgrade_upgrade_call_drupal_clone_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'clone';
  $item->noparens = 1;
}

/**
 * Implements hook_upgrade_call_drupal_eval_alter().
 */
function coder_upgrade_upgrade_call_drupal_eval_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  /*
   * Examine the statement containing the function call.
   * Wrap the containing statement in an "if (module_exists('php'))" block.
   * The function call may be the containing statement.
   */

  // Set the name of the function call.
  $name['value'] = 'php_eval';

  // Get the parent = statement (i.e. node) this function call is part of.
  $parent = $item->parent;
  $temp = $editor->statementsToText($parent);
  $temp = $editor->textToStatements("if (module_exists('php')) {\n\t$temp\n}");
  $parent->data = $temp->getElement(0);
}

/**
 * Implements hook_upgrade_call_drupal_execute_alter().
 */
function coder_upgrade_upgrade_call_drupal_execute_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_form_submit';
}


/**
 * Implements hook_upgrade_call_drupal_get_content_alter().
 */
function coder_upgrade_upgrade_call_drupal_get_content_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_get_region_content';
}

/**
 * Implements hook_upgrade_call_drupal_get_headers_alter().
 */
function coder_upgrade_upgrade_call_drupal_get_headers_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_get_header';
}

/**
 * Implements hook_upgrade_call_drupal_http_request_alter().
 */
function coder_upgrade_upgrade_call_drupal_http_request_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  cdp($item->parameters->print_r());

  $count = $item->parameters->count();
  if ($count == 1) {
    return;
  }

  $keys = array('headers', 'method', 'data', 'max_redirects');
  $defaults = array('xxx_YYY_zzz', "'GET'", 'NULL', 3);
  $string = $editor->arrayitize($item, 1, $keys, $defaults);

  $temp = $editor->expressionToStatement($string);
  $temp->getElement(0)->multiline = 0;
  cdp($temp->print_r());
  $item->setParameter(1, $temp);
}

/**
 * Implements hook_upgrade_call_drupal_json_alter().
 */
function coder_upgrade_upgrade_call_drupal_json_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_json_output';
}

/**
 * Implements hook_upgrade_call_drupal_rebuild_code_registry_alter().
 */
function coder_upgrade_upgrade_call_drupal_rebuild_code_registry_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'registry_rebuild';
}

/**
 * Implements hook_upgrade_call_drupal_rebuild_theme_registry_alter().
 */
function coder_upgrade_upgrade_call_drupal_rebuild_theme_registry_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_theme_rebuild';
}

/**
 * Implements hook_upgrade_call_drupal_set_content_alter().
 */
function coder_upgrade_upgrade_call_drupal_set_content_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_add_region_content';
}

/**
 * Implements hook_upgrade_call_drupal_set_header_alter().
 */
function coder_upgrade_upgrade_call_drupal_set_header_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // TODO the hook_file_download() changes. Use parser on the entire function.
  $temp = $item->printParameters();
  if (strpos($temp, 'Content-Type:') !== FALSE) {
    $temp = explode(':', $temp);
    foreach ($temp as $key => $string) {
      $temp[$key] = "'" . trim($string, "' ") . "'";
    }
    $editor->setParameters($item, $temp);
  }
  elseif (strpos($temp, "\$_SERVER['SERVER_PROTOCOL']") !== FALSE ||
              strpos($temp, '\$_SERVER["SERVER_PROTOCOL"]') !== FALSE) {
    $from = '@\$_SERVER\[(\'|")SERVER_PROTOCOL(\'|")\]\s*\.\s*(\'|")\s*(.*?)(\'|")@';
    $to = "$3$4$3";
    $temp = preg_replace($from, $to, $temp);
    $editor->setParameters($item, array($temp));
  }
}

/**
 * Implements hook_upgrade_call_drupal_set_html_head_alter().
 */
function coder_upgrade_upgrade_call_drupal_set_html_head_alter(&$item, &$reader) { // DONE (UPDATED)
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_add_html_head';

  $count = $item->parameters->count();
  if ($count == 1) {
    $key = '$key = NULL /* TODO Set this variable. */';
    // Insert new required parameter when first parameter is set.
    $editor->insertParameter($item, 1, $key);
  }
}

/**
 * Implements hook_upgrade_call_drupal_set_title_alter().
 */
function coder_upgrade_upgrade_call_drupal_set_title_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $temp = $item->printParameters();
  cdp("temp = $temp");

  if (strpos($temp, 'check_plain') !== FALSE) {
    // TODO Could check for isset, count of parameters, etc.???
    // check_plain could be part of an expression (e.g. concatenation).
    $temp = $item->getParameter(0);
    $temp = $temp->getElement(0);
    cdp("Printing the first ");
    cdp($temp->print_r());
    $editor->setParameter($item, 0, $temp->printParameter(0));
    cdp($item->print_r());
    return;
  }
  $parameters = &$item->parameters;
  if ($call = & $parameters->search('PGPFunctionCall', 'name', 'value', 't')) {
    $temp = $call->toString();
    cdp("temp = $temp");
    if (preg_match('#(\'|")@\w+(\'|")\s*=>\s*#', $temp)) {
      $editor->setParameter($item, 1, 'PASS_THROUGH');
      cdp($item->print_r(0, $item));
    }
  }
}

/**
 * Implements hook_upgrade_call_drupal_system_listing_alter().
 */
function coder_upgrade_upgrade_call_drupal_system_listing_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $temp = $item->printParameter(0);
  // Check for type == T_CONSTANT_ENCAPSED_STRING
  // Check for a '/' in the mask and use a different mask or delimit the '/' with '\/'.
  if ($temp[0] == "'") {
    $editor->setParameter($item, 0, "'/" . substr($temp, 1, -1) . "/'");
  }
  elseif ($temp[0] == '"') {
    $editor->setParameter($item, 0, '"/' . substr($temp, 1, -1) . '/"');
  }
  // else if type == T_VARIABLE, find the $mask used in the call and examine its value.
}

/**
 * Implements hook_upgrade_call_drupal_to_js_alter().
 */
function coder_upgrade_upgrade_call_drupal_to_js_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_json_encode';
}

/**
 * Implements hook_upgrade_call_drupal_uninstall_module_alter().
 */
function coder_upgrade_upgrade_call_drupal_uninstall_module_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_uninstall_modules';
  $temp = $item->printParameters();
  $editor->setParameters($item, array('array(' . $temp . ')'));
}

/**
 * Implements hook_upgrade_call_file_scan_directory_alter().
 */
function coder_upgrade_upgrade_call_file_scan_directory_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // TODO Part of http://drupal.org/node/224333#preg_match
  // TODO Other changes apply to this function call.
  $temp = $item->printParameter(1);
  // Check for type == T_CONSTANT_ENCAPSED_STRING
  // Check for a '/' in the mask and use a different mask or delimit the '/' with '\/'.
  if ($temp[0] == "'") {
    $editor->setParameter($item, 1, "'/" . substr($temp, 1, -1) . "/'");
  }
  elseif ($temp[0] == '"') {
    $editor->setParameter($item, 1, '"/' . substr($temp, 1, -1) . '/"');
  }
  // else if type == T_VARIABLE, find the $mask used in the call and examine its value.
}

/**
 * Implements hook_upgrade_call_file_set_status_alter().
 */
function coder_upgrade_upgrade_call_file_set_status_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;

  if ($item->parameters->count() == 2) {
    $p0 = $item->printParameter(0);
    $p1 = $item->printParameter(1);

    // Insert statement.
    $temp = $editor->textToStatements("{$p0}->status &= $p1");
    $parent = $item->parent;
    $parent->container->insertBefore($parent, $temp->getElement(0));

    // Change statement.
    $from = $temp1;
    $to = "$p0 = file_save($p0)";
    $temp = str_replace($from, $to, $temp2);
    $temp = $editor->textToStatements($temp);
    $parent->data = $temp->getElement(0);
  }
}

/**
 * Implements hook_upgrade_call_filter_formats_alter().
 */
function coder_upgrade_upgrade_call_filter_formats_alter(&$item, &$reader) { // DONE (UPDATED)
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  cdp('filter_formats');
  /*
   * If call has a parameter, then change it to $user and add global statement.
   * If no parameter, then this still implies current user in D6.
   * Set the existing assignment variable to itself at $index.
   * Ex: $formats = filter_formats($index);
   * becomes
   * global $user;
   * $formats = filter_formats($user);
   * $formats = $formats[$index];
   */
  $index = $item->printParameter(0);
  cdp("index = '$index'");
  $p0 = $editor->expressionToStatement('$user');
  $item->setParameter(0, $p0);

  // Get the parent = statement (i.e. node) this function call is part of.
  $parent = &$item->parent;
  // Get the statement list the parent is part of.
  $container = &$parent->container;
  // Insert a statement.
  $statement = $editor->textToStatements("global \$user;")->getElement(0);
  $container->insertBefore($parent, $statement, 'global');
  if ($index) {
    // Insert a statement.
    $assignment = $parent->data;
    $assign_variable = $assignment->values->getElement()->getElement()->toString();
    cdp($parent->data->print_r());
    $statement = $editor->textToStatements("$assign_variable = $assign_variable\[$index\];")->getElement(0);
    $container->insertAfter($parent, $statement, 'assignment');
  }
}

/**
 * Implements hook_upgrade_call_format_plural_alter().
 */
function coder_upgrade_upgrade_call_format_plural_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $count = $item->parameters->count();
  if ($count < 5) {
    return;
  }

  $keys = array('langcode');
  $defaults = array("'XXX_YYY'");
  $string = $editor->arrayitize($item, 4, $keys, $defaults);

  $temp = $editor->expressionToStatement($string);
  $temp->getElement(0)->multiline = 0;
  cdp($temp->print_r());
  $item->setParameter(4, $temp);
}

/**
 * Implements hook_upgrade_call_function_exists_alter().
 */
function coder_upgrade_upgrade_call_function_exists_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // Change was reverted.
  //      $name['value'] = 'drupal_function_exists';
}

/**
 * Implements hook_upgrade_call_menu_path_is_external_alter().
 */
function coder_upgrade_upgrade_call_menu_path_is_external_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'url_is_external';
}

/**
 * Implements hook_upgrade_call_module_invoke_alter().
 */
function coder_upgrade_upgrade_call_module_invoke_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  // http://drupal.org/node/224333#taxonomy_get_tree
  $depth = '$max_depth = NULL /* TODO Set this variable. */';
  $count = $item->parameters->count();
  // Confirm this call relates to our topic.
  if ($count > 2) {
    $p0 = $item->printParameter(0);
    $p1 = $item->printParameter(1);
    if ($p0 != "'taxonomy'" || $p1 != "'get_tree'") {
      cdp("FAILED to relate");
      return;
    }
  }

  // Adjust parameters.
  if ($count > 5) {
    // Switch places.
    $p4 = $item->getParameter(4);
    $p5 = $item->getParameter(5);
    $item->setParameter(4, $p5);
    $item->setParameter(5, $p4);
  }
  elseif ($count > 4) {
    // Insert parameter due to change in parameter order.
    $editor->insertParameter($item, 4, $depth);
    $count = $item->parameters->count();
  }

  $defaults = array(array('NULL', $depth), '-1');
  $string = $editor->removeDefaults($item, 4, $defaults);
}

/**
 * Implements hook_upgrade_call_module_list_alter().
 */
function coder_upgrade_upgrade_call_module_list_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'module_implements';
  $editor->setParameters($item, array('$hook /* TODO Set this variable. */'));
}

/**
 * Implements hook_upgrade_call_module_rebuild_cache_alter().
 */
function coder_upgrade_upgrade_call_module_rebuild_cache_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'system_rebuild_module_data';
}

/**
 * Implements hook_upgrade_call_referer_uri_alter().
 */
function coder_upgrade_upgrade_call_referer_uri_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $expression = $editor->expressionToStatement("\$_SERVER['HTTP_REFERER']");
  $item = $expression->getElement(0);
}

/**
 * Implements hook_upgrade_call_set_time_limit_alter().
 */
function coder_upgrade_upgrade_call_set_time_limit_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'drupal_set_time_limit';
}

/**
 * Implements hook_upgrade_call_system_theme_data_alter().
 */
function coder_upgrade_upgrade_call_system_theme_data_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'system_rebuild_theme_data';
}

/**
 * Implements hook_upgrade_call_t_alter().
 */
function coder_upgrade_upgrade_call_t_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $count = $item->parameters->count();
  if ($count < 3) {
    return;
  }

  $keys = array('langcode');
  $defaults = array("'XXX_YYY'");
  $string = $editor->arrayitize($item, 2, $keys, $defaults);

  $temp = $editor->expressionToStatement($string);
  $temp->getElement(0)->multiline = 0;
  cdp($temp->print_r());
  $item->setParameter(2, $temp);
}

/**
 * Implements hook_upgrade_call_taxonomy_del_term_alter().
 */
function coder_upgrade_upgrade_call_taxonomy_del_term_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'taxonomy_term_delete';
}

/**
 * Implements hook_upgrade_call_taxonomy_del_vocabulary_alter().
 */
function coder_upgrade_upgrade_call_taxonomy_del_vocabulary_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'taxonomy_vocabulary_delete';
}

/**
 * Implements hook_upgrade_call_taxonomy_get_term_alter().
 */
function coder_upgrade_upgrade_call_taxonomy_get_term_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'taxonomy_term_load';
}

/**
 * Implements hook_upgrade_call_taxonomy_get_tree_alter().
 */
function coder_upgrade_upgrade_call_taxonomy_get_tree_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $depth = '$max_depth = NULL /* TODO Set this variable. */';
  $count = $item->parameters->count();
  // Adjust parameters.
  if ($count > 3) {
    // Switch places.
    $p2 = $item->getParameter(2);
    $p3 = $item->getParameter(3);
    $item->setParameter(2, $p3);
    $item->setParameter(3, $p2);
  }
  elseif ($count > 2) {
    // Insert parameter due to change in parameter order.
    $editor->insertParameter($item, 2, $depth);
    $count = $item->parameters->count();
  }

  $defaults = array(array('NULL', $depth), '-1');
  $string = $editor->removeDefaults($item, 2, $defaults);
}

/**
 * Implements hook_upgrade_call_taxonomy_save_term_alter().
 */
function coder_upgrade_upgrade_call_taxonomy_save_term_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'taxonomy_term_save';
  $temp = $item->printParameters();
  $editor->setParameters($item, array('$term /* TODO Term object replaces array ' . $temp . ' */)'));
}

/**
 * Implements hook_upgrade_call_taxonomy_save_vocabulary_alter().
 */
function coder_upgrade_upgrade_call_taxonomy_save_vocabulary_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'taxonomy_vocabulary_save';
  $temp = $item->printParameters();
  $editor->setParameters($item, array('$vocabulary /* TODO Vocabulary object replaces array ' . $temp . ' */)'));
}

/**
 * Implements hook_upgrade_call_theme_alter().
 */
function coder_upgrade_upgrade_call_theme_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  /*
   * Create a static variable to hold the array of theme registrations.
   * Find the theme in the list so we know the names of its parameters.
   * Array-itize the parameters.
   *
   * Find the hook_theme function (using $reader passed to this function)
   * and save its contents. This assumes the hook_theme is defined in this
   * file!!!
   *
   * Add helper functions to PGPArray to get the keys and values as arrays.
   * TODO Refactor any existing code that does this.
   *
   * static $themes = array();
   *
   * $p0 = $item->getParameter(0);
   * $theme = $themes[$p0];
   * $keys = keys from $theme
   * $string = 'array(';
   * foreach ($keys as $key) {
   *   $string .= "'$key' => $p{$i},";
   * }
   * $string = ');
   */
  cdp(__FUNCTION__ . " inside theme case");
  global $_coder_upgrade_module_name;
  // Find the hook_theme function object.
  $function = $editor->findFunction($reader->getFunctions(), $_coder_upgrade_module_name . '_theme');
  if (is_null($function)) {
    return;
  }

  // Get the keys for the appropriate theme.
  $body = &$function->body;

  if (!($return = $body->find(T_RETURN, 'reverse'))) {
    clp("ERROR: return statement not found in hook_perm");
    return;
  }
  $value = &$return->value;
  $array = $value->getElement(0);
  if (get_class($array) == 'PGPArray') {
    if (!($theme = $array->findValue($item->getParameter(0)->toString()))) {
      clp("ERROR: theme entry not found in hook_theme");
      return;
    }
    $array = $theme->getElement(0);
    $arguments = $array->findValue("'arguments'");
    $keys = $arguments->getElement(0)->keysToArray();
    if (empty($keys)) {
      return;
    }
    // Remove the quotes surrounding the keys.
    foreach ($keys as $index => $key) {
      $keys[$index] = trim($key, "'\"");
    }
  }

  $count = $item->parameters->count();
  if ($count == 1) {
    return;
  }

  $defaults = array_fill(0, $count - 1, "'XXX_YYY'");
  $string = $editor->arrayitize($item, 1, $keys, $defaults);

  $temp = $editor->expressionToStatement($string);
  $temp->getElement(0)->multiline = 0;
  $item->setParameter(1, $temp);
}

/**
 * Implements hook_upgrade_call_time_alter().
 */
function coder_upgrade_upgrade_call_time_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $expression = $editor->expressionToStatement('REQUEST_TIME');
  cdp($expression->print_r());
  $item = $expression->getElement(0);
}

/**
 * Implements hook_upgrade_call_user_authenticate_alter().
 */
function coder_upgrade_upgrade_call_user_authenticate_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $count = $item->parameters->count();
  if ($count == 0) {
    $editor->setParameters($item, array('$name', '$password /* TODO Set these variables */'));
    return;
  }

  /*
   * Two cases:
   * - parameter is an array expression: extract values to use as new
   *   parameters
   * - parameter is a variable expression (not an array): assume the
   *   variable has name and pass as elements
   */

  $p0 = $item->getParameter();
  $operand = $p0->getElement();
  $class = get_class($operand);
  if ($class == 'PGPOperand') {
    // Get the variable name used as the parameter.
    $parameter = $item->getParameter()->toString();
    // Make variable assignments referring to two new parameters.
    $assign1 = $editor->textToStatements('$name = ' . $parameter . "['name']; // TODO Set these variables");
    $assign2 = $editor->textToStatements('$password = ' . $parameter . "['pass'];")->getElement(0);
    cdp($assign1->print_r());
    // Insert the assignments before this statement.
    // Get the statement (i.e. node) this function call is part of.
    $parent = &$item->parent;
    // Get the statement list the parent is part of.
    $container = &$parent->container;
    // Insert statements.
    $container->insertListBefore($parent, $assign1, 'assignment');
    $container->insertBefore($parent, $assign2, 'assignment');
    // Set the parameters on this function call.
    $editor->setParameters($item, array('$name', '$password'));
  }
  elseif ($class == 'PGPArray') {
    $name = $operand->findValue("'name'")->toString();
    $password = $operand->findValue("'pass'")->toString();
    // Set the parameters on this function call.
    $editor->setParameters($item, array($name, $password));
  }
}

/**
 * Implements hook_upgrade_call_user_delete_alter().
 */
function coder_upgrade_upgrade_call_user_delete_alter(&$item, &$reader) { // DONE
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // Process function call.
  $name = &$item->name;
  $name['value'] = 'user_cancel';
  $editor->setParameter($item, 2, "\$method = 'user_cancel_block' /* TODO Set this variable */");
}
