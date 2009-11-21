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
 * http://drupal.org/node/224333#registry
 * http://drupal.org/node/224333#drupal_set_session (REVERTED)
 * http://drupal.org/node/224333#time
 * http://drupal.org/node/224333#rebuild_functions
 * http://drupal.org/node/224333#drupal_uninstall_modules
 * http://drupal.org/node/224333#module_implements_not_module_list
 * http://drupal.org/node/224333#system_get_module_data
 * http://drupal.org/node/224333#drupal_set_html_head
 * http://drupal.org/node/224333#php_eval
 * http://drupal.org/node/224333#http_header_functions
 * http://drupal.org/node/224333#drupal_set_content
 * http://drupal.org/node/224333#time_limit
 *
 *
 * http://drupal.org/node/224333#comment_load
 * http://drupal.org/node/224333#comment_validate_removed
 * http://drupal.org/node/224333#comment_node_url
 *
 *
 * http://drupal.org/node/224333#check_markup_params
 *
 *
 * http://drupal.org/node/224333#taxonomy_get_tree
 * http://drupal.org/node/224333#taxonomy_crud
 *
 *
 * http://drupal.org/node/224333#file_set_status
 * http://drupal.org/node/224333#preg_match
 *
 *
 * http://drupal.org/node/224333#referer_uri
 * http://drupal.org/node/224333#drupal_clone
 *
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function coder_upgrade_convert_function_calls(&$reader) {
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
  $editor = new PGPEditor();
  $class = new PGPFunctionCall();
  $list = new PGPList();

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

    case 'drupal_get_content': // DONE
      $name['value'] = 'drupal_get_region_content';
      break;

    case 'drupal_get_headers': // DONE
      $name['value'] = 'drupal_get_header';
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

    case 'taxonomy_get_tree':
      // Assume they are not default values if present.
      if ($item['parameters']->count() > 3) {
        $p2 = $item['parameters']->get(2)->data;
        $p3 = $item['parameters']->get(3)->data;
        $item['parameters']->updateKey(2, $p3);
        $item['parameters']->updateKey(3, $p2);
      }
      elseif ($item['parameters']->count() > 2) {
        $class->insertParameter($item, 2, '$max_depth = NULL /* TODO Set this variable. */');
      }
      // TODO Eliminate default variables from right to left.
      // TODO Do we need to change module_invoke('taxonomy', 'get_tree', ...)? See patch code in example function.
      break;

    case 'taxonomy_save_term':
      $name['value'] = 'taxonomy_term_save';
      $temp = $class->printParameters($item);
      $class->setParameters($item, array('$term /* TODO Term object replaces array ' . $temp . ' */)'));
      break;

    case 'taxonomy_save_vocabulary':
      $name['value'] = 'taxonomy_vocabulary_save';
      $temp = $class->printParameters($item);
      $class->setParameters($item, array('$vocabulary /* TODO Vocabulary object replaces array ' . $temp . ' */)'));
      break;

    case 'time':
      $temp = $editor->expressionToStatement('REQUEST_TIME');
      $item = $temp['operand_1'];
      break;
  }
}

/**
 * Simple regular expression changes.
 *
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

  // Replace referer_uri() with PHP-provided global variable.
  $from[] = '/(\'|")use PHP for block visibility(\'|")/';
  $to[] = "$1use PHP for settings$1";

  coder_upgrade_do_conversions($from, $to, $new);
  coder_upgrade_save_changes($cur, $new, $file, $hook);
}
