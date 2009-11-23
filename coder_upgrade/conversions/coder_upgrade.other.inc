<?php
// $Id$

/**
 * @file
 * Other conversion routine file for the coder_upgrade module.
 *
 * The functions in these conversion routine files correspond to the topics in
 * the category roadmap at http://drupal.org/node/394070 that are marked with
 * a "Yes" in the Upgrade column.
 *
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Function call changes using grammar parser.
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
 * Comments
 * http://drupal.org/node/224333#comment_load
 * http://drupal.org/node/224333#comment_validate_removed
 * http://drupal.org/node/224333#comment_node_url
 *
 *
 * Input Sanitization and Input Formats
 * http://drupal.org/node/224333#check_markup_params
 * http://drupal.org/node/224333#drupal_set_title
 * http://drupal.org/node/224333#hook_filter_params (NOT IN THIS FUNCTION)
 *
 *
 * Taxonomy
 * http://drupal.org/node/224333#taxonomy_get_tree
 * http://drupal.org/node/224333#taxonomy_crud
 *
 *
 * Theming
 * http://drupal.org/node/224333#rebuild_functions (DUP System)
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
 * Miscellaneous
 * http://drupal.org/node/224333#referer_uri
 * http://drupal.org/node/224333#drupal_clone
 *
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function coder_upgrade_convert_function_calls(&$reader) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $editor = new PGPEditor();
  $types = array(T_STRING);
  $editor->traverseCallback($reader->getFunctionCalls(), 'coder_upgrade_callback_function_calls', $types);
}

/**
 * Callback routine for function call changes using grammar parser.
 *
 * @param PGPFunctionCall2 $item
 *   A function call object of the expression or statement.
 */
function coder_upgrade_callback_function_calls(&$item) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");

  /*
   * The $item variable passed to this function is a PGPFunctionCall2 object.
   * When passed here via the coder_upgrade_convert_function_calls routine, the
   * variable was an entry in the function calls array filled by the PGPReader
   * class (and accessed as $reader->getFunctionCalls()).
   *
   * The function calls array contains references to PGPFunctionCall2 objects.
   * As these objects are processed, they may change or eliminate other
   * references yet to be processed (i.e. if other function call references
   * are contained in the current reference). Because of this, we need to
   * test whether $item actually refers to a PGPFunctionCall2 object before
   * attempting to do any processing.
   */
  if (!isset($item) || !is_a($item, 'PGPFunctionCall2')) {
    return;
  }

  // Create helper objects.
  $editor = new PGPEditor();
  $class = new PGPFunctionCall();
  $list = new PGPList();

  // Process function call.
  $name = &$item->name;
  switch ($name['value']) {
    case 'check_markup': // DONE
      if ($item->parameters->count() > 2) {
        $class->insertParameter($item, 2, '$langcode = \'\' /* TODO Set this variable. */');
      }
      break;

    case '_comment_load': // DONE
      // TODO The comment_wildcard change in hook_menu.
      $name['value'] = 'comment_load';
      break;

    case 'comment_node_url': // DONE
      $parent = $item->parent;
      $temp = $editor->statementsToText($parent);
      $from = '@comment_node_url()@';
      $to = "'comment/' . \$comment->cid";
      $temp = preg_replace($from, $to, $temp);
      $temp = $editor->textToStatements($temp);
      $parent->data = $temp->get(0)->data;
      break;

    case 'comment_validate': // DONE
      $name['value'] = 'comment_form_validate';
      $class->setParameters($item, array('$form', '$form_state /* TODO Set these variables. */'));
      break;

    case 'drupal_clone': // DONE
      $name['value'] = 'clone';
      $item->noparens = 1;
      break;

    case 'drupal_eval': // DONE
      /*
       * Examine the statement containing the function call.
       * Wrap the containing statement in an "if (module_exists('php'))" block.
       * The function call may be the containing statement.
       */

      // Set the name of the function call.
      $name['value'] = 'php_eval';

      // Get the parent = statement (i.e. node) this function call is part of.
      $parent = $item->parent;
      $temp = $class->statementsToText($parent);
      $temp = $editor->textToStatements("if (module_exists('php')) {\n\t$temp\n}");
      $parent->data = $temp->get(0)->data;
      break;

    case 'drupal_execute': // DONE
      $name['value'] = 'drupal_form_submit';
      break;

    case 'drupal_get_content': // DONE
      $name['value'] = 'drupal_get_region_content';
      break;

    case 'drupal_get_headers': // DONE
      $name['value'] = 'drupal_get_header';
      break;

    case 'drupal_http_request': // DONE
      coder_upgrade_debug_echo($item->parameters->print_r());

      $count = $item->parameters->count();
      if ($count == 1) {
        break;
      }

      $string = 'array(';
      if ($count > 1) {
        $params['headers'] = $item->printParameter(1);
        $string .= "'headers' => " . $item->printParameter(1);
      }
      if ($count > 2) {
        $value = $item->printParameter(2);
        if ($value != "'GET'") {
          $params['method'] = $value;
          $string .= ", 'method' => $value";
        }
      }
      if ($count > 3) {
        $value = $item->printParameter(3);
        if ($value != 'NULL') {
          $params['data'] = $value;
          $string .= ", 'data' => $value";
        }
      }
      if ($count > 4) {
        $value = $item->printParameter(4);
        if ($value != 3) {
          $params['max_redirects'] = $value;
          $string .= ", 'max_redirects' => $value";
        }
      }
      $string .= ')';
      coder_upgrade_debug_echo("params\n");
      coder_upgrade_debug_echo(print_r($params, 1));
      coder_upgrade_debug_echo("string\n");
      coder_upgrade_debug_echo($string . "\n");

      $item->deleteParameter(4);
      $item->deleteParameter(3);
      $item->deleteParameter(2);

      $temp = $editor->expressionToStatement($string);
      coder_upgrade_debug_echo($temp->print_r());
//      $reader = new PGPReader();
//      $reader->isMultiline = FALSE;
      $item->setParameter(1, $temp); // $item->parameters->get(1)->data = $temp;
      break;

    case 'drupal_rebuild_code_registry': // DONE
      $name['value'] = 'registry_rebuild';
      break;

    case 'drupal_rebuild_theme_registry': // DONE
      $name['value'] = 'drupal_theme_rebuild';
      break;

    case 'drupal_set_content': // DONE
      $name['value'] = 'drupal_add_region_content';
      break;

    case 'drupal_set_header': // DONE
      // TODO the hook_file_download() changes. Use parser on the entire function.
      $temp = $item->printParameters();
      if (strpos($temp, 'Content-Type:') !== FALSE) {
        $temp = explode(':', $temp);
        foreach ($temp as $key => $string) {
          $temp[$key] = "'" . trim($string, "' ") . "'";
        }
        $class->setParameters($item, $temp);
      }
      elseif (strpos($temp, "\$_SERVER['SERVER_PROTOCOL']") !== FALSE ||
              strpos($temp, '\$_SERVER["SERVER_PROTOCOL"]') !== FALSE) {
        $from = '@\$_SERVER\[(\'|")SERVER_PROTOCOL(\'|")\]\s*\.\s*(\'|")\s*(.*?)(\'|")@';
        $to = "$3$4$3";
        $temp = preg_replace($from, $to, $temp);
        $class->setParameters($item, array($temp));
      }
      break;

    case 'drupal_set_html_head': // DONE
      $name['value'] = 'drupal_add_html_head';
      break;

    case 'drupal_set_title': // DONE
      coder_upgrade_debug_echo($item->print_r());
      $temp = $item->printParameters();
      coder_upgrade_debug_echo("temp = $temp\n");

      if (strpos($temp, 'check_plain') !== FALSE) {
        // TODO Could check for isset, count of parameters, etc.???
        // check_plain could be part of an expression (e.g. concatenation).
        $temp = $item->getParameter(0);
        $temp = $temp->getElement(0);
        coder_upgrade_debug_echo("Printing the first \n");
        coder_upgrade_debug_echo($temp->print_r());
        $class->setParameter($item, 0, $temp->printParameter(0));
        coder_upgrade_debug_echo($item->print_r());
        break;
      }
      $parameters = &$item->parameters;
      if ($call = &$parameters->search($parameters, 'PGPFunctionCall2', 'name', 'value', 't')) {
        $temp = $call->toString();
        coder_upgrade_debug_echo("temp = $temp\n");
        if (preg_match('#(\'|")@\w+(\'|")\s*=>\s*#', $temp)) {
          $class->setParameter($item, 1, 'PASS_THROUGH');
          coder_upgrade_debug_echo($item->print_r(0, $item));
        }
      }
      break;

    case 'drupal_system_listing': // DONE
      $temp = $item->printParameter(0);
      // Check for type == T_CONSTANT_ENCAPSED_STRING
      // Check for a '/' in the mask and use a different mask or delimit the '/' with '\/'.
      if ($temp[0] == "'") {
        $class->setParameter($item, 0, "'/" . substr($temp, 1, -1) . "/'");
      }
      elseif ($temp[0] == '"') {
        $class->setParameter($item, 0, '"/' . substr($temp, 1, -1) . '/"');
      }
      // else if type == T_VARIABLE, find the $mask used in the call and examine its value.
      break;

    case 'drupal_uninstall_module': // DONE
      $name['value'] = 'drupal_uninstall_modules';
      $temp = $item->printParameters();
      $class->setParameters($item, array('array(' . $temp . ')'));
      break;

    case 'file_scan_directory': // DONE
      // TODO Part of http://drupal.org/node/224333#preg_match
      // TODO Other changes apply to this function call.
      coder_upgrade_debug_echo($item->print_r());
      $temp = $item->printParameter(1);
      // Check for type == T_CONSTANT_ENCAPSED_STRING
      // Check for a '/' in the mask and use a different mask or delimit the '/' with '\/'.
      if ($temp[0] == "'") {
        $class->setParameter($item, 1, "'/" . substr($temp, 1, -1) . "/'");
      }
      elseif ($temp[0] == '"') {
        $class->setParameter($item, 1, '"/' . substr($temp, 1, -1) . '/"');
      }
      // else if type == T_VARIABLE, find the $mask used in the call and examine its value.
      break;

    case 'file_set_status': // DONE
      $parent = $item->parent;

      if ($item->parameters->count() == 2) {
        $p0 = $item->printParameter(0);
        $p1 = $item->printParameter(1);

        // Insert statement.
        $temp = $editor->textToStatements("{$p0}->status &= $p1");
        $parent = $item->parent;
        $parent->container->insertBefore($parent, $temp->get(0)->data);

        // Change statement.
        $from = $temp1;
        $to = "$p0 = file_save($p0)";
        $temp = str_replace($from, $to, $temp2);
        $temp = $editor->textToStatements($temp);
        $parent->data = $temp->get(0)->data;
      }
      break;

    case 'function_exists':
      // Change was reverted.
//      $name['value'] = 'drupal_function_exists';
      break;

    case 'module_list': // DONE
      $name['value'] = 'module_implements';
      $class->setParameters($item, array('$hook /* TODO Set this variable. */'));
      break;

    case 'module_rebuild_cache': // DONE
      $name['value'] = 'system_get_module_data';
      break;

    case 'referer_uri': // DONE
      $expression = $editor->expressionToStatement("\$_SERVER['HTTP_REFERER']");
      $item = $expression->getElement(0);
      break;

    case 'set_time_limit': // DONE
      $name['value'] = 'drupal_set_time_limit';
      break;

    case 'system_theme_data': // DONE
      $name['value'] = 'system_get_theme_data';
      break;

    case 'taxonomy_del_term': // DONE
      $name['value'] = 'taxonomy_term_delete';
      break;

    case 'taxonomy_del_vocabulary': // DONE
      $name['value'] = 'taxonomy_vocabulary_delete';
      break;

    case 'taxonomy_get_term': // DONE
      $name['value'] = 'taxonomy_term_load';
      break;

    case 'taxonomy_get_tree': // DONE
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
        $class->insertParameter($item, 2, $depth);
        $count = $item->parameters->count();
      }

      // Remove default parameters from right to left.
      if ($count == 4) {
        $value = $item->printParameter(3);
        if ($value == '-1') {
          $item->deleteParameter(3);
          $count = $item->parameters->count();
        }
      }
      if ($count == 3) {
        $value = $item->printParameter(2);
        if ($value == 'NULL' || $value == $depth) {
          $item->deleteParameter(2);
        }
      }
      break;

    case 'taxonomy_save_term': // DONE
      $name['value'] = 'taxonomy_term_save';
      $temp = $item->printParameters();
      $class->setParameters($item, array('$term /* TODO Term object replaces array ' . $temp . ' */)'));
      break;

    case 'taxonomy_save_vocabulary': // DONE
      $name['value'] = 'taxonomy_vocabulary_save';
      $temp = $item->printParameters();
      $class->setParameters($item, array('$vocabulary /* TODO Vocabulary object replaces array ' . $temp . ' */)'));
      break;

    case 'time': // DONE
      $expression = $editor->expressionToStatement('REQUEST_TIME');
      coder_upgrade_debug_echo($expression->print_r());
      $item = $expression->getElement(0);
      break;
  }
}

/**
 * Simple regular expression changes.
 *
 * System
 * http://drupal.org/node/224333#moved_statistics_settings
 *
 *
 * Permissions and Access
 * http://drupal.org/node/224333#moved_statistics_settings (DUP System)
 * http://drupal.org/node/224333#php_permissions
 *
 * @param string $file
 *   The text of the file to convert.
 */
function coder_upgrade_convert_regex_simple_changes(&$file) {
  $hook = 'regex_simple_changes';
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  // Moved admin/reports/settings to admin/config/system/statistics (replacing admin/settings/statistics).
  $from[] = "@'admin/reports/settings'@";
  $to[] = "'admin/config/system/statistics'";

  // Replace referer_uri() with PHP-provided global variable.
  $from[] = '/(\'|")use PHP for block visibility(\'|")/';
  $to[] = "$1use PHP for settings$1";

  coder_upgrade_do_conversions($from, $to, $new);
  coder_upgrade_save_changes($cur, $new, $file, $hook);
}
