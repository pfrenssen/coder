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
 * Miscellaneous routine for simple changes.
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
 * http://drupal.org/node/224333#drupal_clone
 *
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function coder_upgrade_convert_simple_changes(&$reader) {
  $editor = new PGPEditor();
  $types = array(T_STRING);
  $editor->traverse_callback($reader->getFunctionCalls(), 'coder_upgrade_callback_simple_changes', $types);
}

function coder_upgrade_callback_simple_changes(&$item) {
  $name = &$item['name'];
  $value = &$name['value'];
  switch ($name['value']) {
    case '_comment_load':
      // TODO The comment_wildcard change in hook_menu.
      $name['value'] = 'comment_load';

    case 'comment_node_url':
      $parent = $item['parent'];
      $temp = $editor->statementsToText($parent);
      $from = '@comment_node_url()@';
      $to = "'comment/' . \$comment->cid";
      $temp = preg_replace($from, $to, $temp);
      $temp = $editor->textToStatements($temp);
      $parent->data = $temp->get(0)->data;
      break;

    case 'comment_validate':
      $name['value'] = 'comment_form_validate';
      $class->setParameters($item, array('$form', '$form_state /* TODO Set these variables. */'));
      break;

    case 'drupal_clone':
      $name['value'] = 'clone';
      $item['noparens'] = 1;
      break;

    case 'drupal_eval':
      /*
       * Examine the statement containing the function call.
       * Wrap the containing statement in an "if (module_exists('php'))" block.
       * The function call may be the containing statement.
       */

      // Set the name of the function call.
      $name['value'] = 'php_eval';

      // Get the parent = statement (i.e. node) this function call is part of.
      $parent = $item['parent'];
      $temp = $class->statementsToText($parent);
      $temp = $editor->textToStatements("if (module_exists('php')) {\n\t$temp\n}");
      $parent->data = $temp->get(0)->data;
      break;

    case 'drupal_get_content':
      $name['value'] = 'drupal_get_region_content';
      break;

    case 'drupal_get_headers':
      $name['value'] = 'drupal_get_header';
      break;

    case 'drupal_rebuild_code_registry':
      $name['value'] = 'registry_rebuild';
      break;

    case 'drupal_rebuild_theme_registry':
      $name['value'] = 'drupal_theme_rebuild';
      break;

    case 'drupal_set_content':
      $name['value'] = 'drupal_add_region_content';
      break;

    case 'drupal_set_header':
      // TODO the hook_file_download() changes. Use parser on the entire function.
      $temp = $class->printParameters($item);
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

    case 'drupal_set_html_head':
      $name['value'] = 'drupal_add_html_head';
      break;

    case 'drupal_uninstall_module':
      $name['value'] = 'drupal_uninstall_modules';
      $temp = $class->printParameters($item);
      $class->setParameters($item, array('array(' . $temp . ')'));
      break;

    case 'function_exists':
      $name['value'] = 'drupal_function_exists';
      break;

    case 'module_list':
      $name['value'] = 'module_implements';
      $class->setParameters($item, array('$hook /* TODO Set this variable. */'));
      break;

    case 'module_rebuild_cache':
      $name['value'] = 'system_get_module_data';
      break;

    case 'set_time_limit':
      $name['value'] = 'drupal_set_time_limit';
      break;

    case 'system_theme_data':
      $name['value'] = 'system_get_theme_data';
      break;

    case 'time':
      $temp = $editor->expressionToStatement('REQUEST_TIME');
      $item = $temp['operand_1'];
      break;
  }
}
