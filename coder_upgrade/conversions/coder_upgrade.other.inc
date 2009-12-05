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
 *
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function coder_upgrade_convert_function_calls(&$reader) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $editor = new PGPEditor();
  $types = array(T_STRING);
  $editor->traverseCallback($reader->getFunctionCalls(), 'coder_upgrade_callback_function_calls', $types, $reader);
}

/**
 * Callback routine for function call changes using grammar parser.
 *
 * Programmatically changing the source code requires changing the contents of
 * the grammar object (the grammar equivalent of the code represented as a
 * PGPList of statements). A difficult way to do this would be to manually
 * create items (arrays or objects extended from the PGPBase class) and replace
 * the existing grammar items with the new items. This would essntially
 * duplicate the functionality of the PGPReader class.
 *
 * A simpler approach takes advantage of the grammar parser API which provides
 * helper functions for working with function call objects. Some examples of
 * typical tasks follow.
 *
 * Print a grammar object
 *
 * Before making a change, it is a good idea to print the contents of the
 * grammar object to become familiar with its structure and contents. PGP
 * provides a custom print_r function for this purpose. (NOTE: calls to the
 * built-in print_r, var_dump or equivalent routines for this purpose is not
 * recommended as the output may be extremely voluminous.) All of the objects
 * (as opposed to array items) can be printed using the custom print_r routine.
 * An array item can be printed using the built-in print_r routine.
 * @code
 *   // Print the grammar item (a function call object in this case).
 *   coder_upgrade_debug_echo($item->print_r());
 *
 *   // Print the parameters to the function call.
 *   coder_upgrade_debug_echo($item->parameters->print_r());
 *   // Alternatively, with a parameters object, do this.
 *   $parameters = $item->parameters;
 *   coder_upgrade_debug_echo($parameters->print_r());
 * @endcode
 *
 * Change the name of the function being called
 * @code
 *   // Change the name of the function being called.
 *   $name['value'] = 'new_function_name';
 * @endcode
 *
 * Get a paramter object and an expression object
 * @code
 *   // Get a parameter object (PGPExpression).
 *   $param = $item->getParameter(0);
 *
 *   // Get the first element of the parameter expression (PGPOperand or array).
 *   $element = $param->getElement(0);
 * @endcode
 *
 * Reorder the parameters
 * @code
 *  // Save the current parameters to local variables.
 *  $p0 = $item->getParameter(0);
 *  $p0 = $item->getParameter(1);
 *
 *  // Swap the parameters.
 *  $item->setParameter(0, $p1);
 *  $item->setParameter(1, $p0);
 * @endcode
 *
 * Insert or delete a parameter object
 * @code
 *   $item->insertParameter(2, $expression);
 *   $item->deleteParameter(2);
 * @endcode
 *
 * Set grammar elements from a string of code
 *
 * As mentioned above, it is not recommended to manually create items (arrays
 * or objects extended from the PGPBase class) and replace the existing grammar
 * items with the new items. A simpler approach is to create the string of code
 * to be inserted and use the PGPEditor class to convert this string to its
 * grammar representation.
 *
 * @code
 *  // Set a single parameter from a string of code.
 *  $editor->setParameter($item, 0, $code);
 *
 *  // Set all of the parameters from an array of code strings.
 *  $editor->setParameters($item, array('$form', '$form_state'));
 *
 *  // Insert a parameter from a string of code.
 *  $editor->insertParameter($item, 2, '$langcode = \'en\'');
 * @endcode
 *
 * Get grammar elements as a string of code
 *
 * @code
 *  // Print a parameter (i.e. convert it from a grammar object to a string).
 *  $parameter = $item->printParameter(1);
 *
 *  // Get the grammar equivalent of a string of code.
 *  $expression = $editor->expressionToStatement($string);
 * @endcode
 *
 * @param PGPFunctionCall $item
 *   A function call object of the expression or statement.
 */
function coder_upgrade_callback_function_calls(&$item, &$reader) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
//  coder_upgrade_debug_echo($item->print_r());

  /*
   * The $item variable passed to this function is a PGPFunctionCall object.
   * When passed here via the coder_upgrade_convert_function_calls routine, the
   * variable was an entry in the function calls array filled by the PGPReader
   * class (and accessed as $reader->getFunctionCalls()).
   *
   * The function calls array contains references to PGPFunctionCall objects.
   * As these objects are processed, they may change or eliminate other
   * references yet to be processed (i.e. if other function call references
   * are contained in the current reference). Because of this, we need to
   * test whether $item actually refers to a PGPFunctionCall object before
   * attempting to do any processing.
   */
  if (!isset($item) || !is_a($item, 'PGPFunctionCall')) {
    return;
  }

  // Create helper objects.
  $editor = new PGPEditor();
  $list = new PGPList();

  // Process function call.
  $name = &$item->name;
  switch ($name['value']) {
    case 'actions_synchronize': // DONE
      $count = $item->parameters->count();
      if ($count > 0) {
        $item->deleteParameter();
      }
      break;

    case 'book_toc': // DONE
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
        coder_upgrade_debug_echo("value = $value\n");
        if ($value == 'array()') {
          $item->deleteParameter(2);
        }
      }
      break;

    case 'check_markup': // DONE
      if ($item->parameters->count() > 2) {
        $editor->insertParameter($item, 2, '$langcode = \'\' /* TODO Set this variable. */');
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
      $parent->data = $temp->getElement(0);
      break;

    case 'comment_validate': // DONE
      $name['value'] = 'comment_form_validate';
      $editor->setParameters($item, array('$form', '$form_state /* TODO Set these variables. */'));
      break;

    case 'drupal_add_css': // DONE
      // A similar comment to that in 'drupal_add_js' below applies here.
      coder_upgrade_debug_echo($item->parameters->print_r());

      $count = $item->parameters->count();
      if ($count < 3) {
        $type = trim($item->printParameter(1), "'\"");
        if (in_array($type, array('module', 'theme'))) {
          $editor->setParameter($item, 1, "'file'"); // Could simply delete it.
        }
        break;
      }

      $keys = array('type', 'media', 'preprocess');
      $defaults = array("'module'", "'all'", 'TRUE');
      $string = $editor->arrayitize($item, 1, $keys, $defaults);
      $string = preg_replace('@[\'"]theme[\'"]@', "'file'", $string); // Could be deleted.

      if ($string != 'array()') {
        $temp = $editor->expressionToStatement($string);
        $temp->getElement(0)->multiline = 0;
        coder_upgrade_debug_echo($temp->print_r());
        $item->setParameter(1, $temp);
      }
      break;

    case 'drupal_add_js': // DONE
      /*
       * With
       * drupal_add_js('misc/collapse.js', 'core', 'header', FALSE, TRUE, TRUE);
       * we will output
       * drupal_add_js('misc/collapse.js', array(
       *   'type' => 'core',
       * ));
       * which is correct, although the function will also accept
       * drupal_add_js('misc/collapse.js', 'core');
       * The example begs the question why someone would have included all
       * the default parameters.
       *
       * A type of 'core', 'module' or 'theme' all convert to 'file' which is
       * the new default. We could add a weight item based on the type?
       */
      coder_upgrade_debug_echo($item->parameters->print_r());

      $count = $item->parameters->count();
      if ($count < 3) {
        $type = trim($item->printParameter(1), "'\"");
        if (in_array($type, array('core', 'module', 'theme'))) {
          $editor->setParameter($item, 1, "'file'"); // Could simply delete it.
        }
        break;
      }

      $keys = array('type', 'scope', 'defer', 'cache', 'preprocess');
      $defaults = array("'module'", "'header'", 'FALSE', 'TRUE', 'TRUE');
      $string = $editor->arrayitize($item, 1, $keys, $defaults);
      $string = preg_replace('@[\'"](core|theme)[\'"]@', "'file'", $string); // Could be deleted.

      if ($string != 'array()') {
        $temp = $editor->expressionToStatement($string);
        $temp->getElement(0)->multiline = 0;
        coder_upgrade_debug_echo($temp->print_r());
        $item->setParameter(1, $temp);
      }
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
      $temp = $editor->statementsToText($parent);
      $temp = $editor->textToStatements("if (module_exists('php')) {\n\t$temp\n}");
      $parent->data = $temp->getElement(0);
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

      $keys = array('headers', 'method', 'data', 'max_redirects');
      $defaults = array('xxx_YYY_zzz', "'GET'", 'NULL', 3);
      $string = $editor->arrayitize($item, 1, $keys, $defaults);

      $temp = $editor->expressionToStatement($string);
      $temp->getElement(0)->multiline = 0;
      coder_upgrade_debug_echo($temp->print_r());
      $item->setParameter(1, $temp);
      break;

    case 'drupal_json': // DONE
      $name['value'] = 'drupal_json_output';
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
        $editor->setParameters($item, $temp);
      }
      elseif (strpos($temp, "\$_SERVER['SERVER_PROTOCOL']") !== FALSE ||
              strpos($temp, '\$_SERVER["SERVER_PROTOCOL"]') !== FALSE) {
        $from = '@\$_SERVER\[(\'|")SERVER_PROTOCOL(\'|")\]\s*\.\s*(\'|")\s*(.*?)(\'|")@';
        $to = "$3$4$3";
        $temp = preg_replace($from, $to, $temp);
        $editor->setParameters($item, array($temp));
      }
      break;

    case 'drupal_set_html_head': // DONE
      $name['value'] = 'drupal_add_html_head';
      break;

    case 'drupal_set_title': // DONE
      $temp = $item->printParameters();
      coder_upgrade_debug_echo("temp = $temp\n");

      if (strpos($temp, 'check_plain') !== FALSE) {
        // TODO Could check for isset, count of parameters, etc.???
        // check_plain could be part of an expression (e.g. concatenation).
        $temp = $item->getParameter(0);
        $temp = $temp->getElement(0);
        coder_upgrade_debug_echo("Printing the first \n");
        coder_upgrade_debug_echo($temp->print_r());
        $editor->setParameter($item, 0, $temp->printParameter(0));
        coder_upgrade_debug_echo($item->print_r());
        break;
      }
      $parameters = &$item->parameters;
      if ($call = &$parameters->search($parameters, 'PGPFunctionCall', 'name', 'value', 't')) {
        $temp = $call->toString();
        coder_upgrade_debug_echo("temp = $temp\n");
        if (preg_match('#(\'|")@\w+(\'|")\s*=>\s*#', $temp)) {
          $editor->setParameter($item, 1, 'PASS_THROUGH');
          coder_upgrade_debug_echo($item->print_r(0, $item));
        }
      }
      break;

    case 'drupal_system_listing': // DONE
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
      break;

    case 'drupal_to_js': // DONE
      $name['value'] = 'drupal_json_encode';
      break;

    case 'drupal_uninstall_module': // DONE
      $name['value'] = 'drupal_uninstall_modules';
      $temp = $item->printParameters();
      $editor->setParameters($item, array('array(' . $temp . ')'));
      break;

    case 'file_scan_directory': // DONE
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
      break;

    case 'file_set_status': // DONE
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
      break;

    case 'filter_formats': // DONE
      // If has a parameter, then change it to $user and add global statement.
      if ($item->parameters->count() > 0) {
        $p0 = $editor->expressionToStatement('$user');
        $item->setParameter(0, $p0);

        // Get the parent = statement (i.e. node) this function call is part of.
        $parent = &$item->parent;
        // Get the statement list the parent is part of.
        $container = &$parent->container;
        // Insert a statement.
        $statement = $editor->textToStatements("global \$user;")->getElement(0);
        $container->insertBefore($parent, $statement, 'global');
      }
      break;

    case 'format_plural': // DONE
      $count = $item->parameters->count();
      if ($count < 5) {
        break;
      }

      $keys = array('langcode');
      $defaults = array("'XXX_YYY'");
      $string = $editor->arrayitize($item, 4, $keys, $defaults);

      $temp = $editor->expressionToStatement($string);
      $temp->getElement(0)->multiline = 0;
      coder_upgrade_debug_echo($temp->print_r());
      $item->setParameter(4, $temp);
      break;

    case 'function_exists':
      // Change was reverted.
//      $name['value'] = 'drupal_function_exists';
      break;

    case 'menu_path_is_external': // DONE
      $name['value'] = 'url_is_external';
      break;

    case 'module_invoke': // DONE
      // http://drupal.org/node/224333#taxonomy_get_tree
      $depth = '$max_depth = NULL /* TODO Set this variable. */';
      $count = $item->parameters->count();
      // Confirm this call relates to our topic.
      if ($count > 2) {
        $p0 = $item->printParameter(0);
        $p1 = $item->printParameter(1);
        if ($p0 != "'taxonomy'" || $p1 != "'get_tree'") {
          coder_upgrade_debug_echo("FAILED to relate\n");
          break;
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
      break;

    case 'module_list': // DONE
      $name['value'] = 'module_implements';
      $editor->setParameters($item, array('$hook /* TODO Set this variable. */'));
      break;

    case 'module_rebuild_cache': // DONE
      $name['value'] = 'system_rebuild_module_data';
      break;

    case 'referer_uri': // DONE
      $expression = $editor->expressionToStatement("\$_SERVER['HTTP_REFERER']");
      $item = $expression->getElement(0);
      break;

    case 'set_time_limit': // DONE
      $name['value'] = 'drupal_set_time_limit';
      break;

    case 'system_theme_data': // DONE
      $name['value'] = 'system_rebuild_theme_data';
      break;

    case 't': // DONE
      $count = $item->parameters->count();
      if ($count < 3) {
        break;
      }

      $keys = array('langcode');
      $defaults = array("'XXX_YYY'");
      $string = $editor->arrayitize($item, 2, $keys, $defaults);

      $temp = $editor->expressionToStatement($string);
      $temp->getElement(0)->multiline = 0;
      coder_upgrade_debug_echo($temp->print_r());
      $item->setParameter(2, $temp);
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
        $editor->insertParameter($item, 2, $depth);
        $count = $item->parameters->count();
      }

      $defaults = array(array('NULL', $depth), '-1');
      $string = $editor->removeDefaults($item, 2, $defaults);
      break;

    case 'taxonomy_save_term': // DONE
      $name['value'] = 'taxonomy_term_save';
      $temp = $item->printParameters();
      $editor->setParameters($item, array('$term /* TODO Term object replaces array ' . $temp . ' */)'));
      break;

    case 'taxonomy_save_vocabulary': // DONE
      $name['value'] = 'taxonomy_vocabulary_save';
      $temp = $item->printParameters();
      $editor->setParameters($item, array('$vocabulary /* TODO Vocabulary object replaces array ' . $temp . ' */)'));
      break;

    case 'theme': // DONE
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
      coder_upgrade_debug_echo(__FUNCTION__ . " inside theme case\n");
      global $_coder_upgrade_module_name;
      // Find the hook_theme function object.
      $function = $editor->findFunction($reader->getFunctions(), $_coder_upgrade_module_name . '_theme');
      if (is_null($function)) {
        return;
      }

      // Get the keys for the appropriate theme.
      $body = &$function->body;

      if (!($return = $body->find(T_RETURN, 'reverse'))) {
        coder_upgrade_debug_print("ERROR: return statement not found in hook_perm");
        return;
      }
      $value = &$return['value'];
      $array = $value->getElement(0);
      if (get_class($array) == 'PGPArray') {
        $theme = $array->findValue($item->getParameter(0)->toString());
        $array = $theme->getElement(0);
        $arguments = $array->findValue("'arguments'");
        $keys = $arguments->getElement(0)->keysToArray();
        if (empty($keys)) {
          break;
        }
        print_r($keys);
        // Remove the quotes surrounding the keys.
        foreach ($keys as $index => $key) {
          $keys[$index] = trim($key, "'\"");
        }
        print_r($keys);
      }

      $count = $item->parameters->count();
      if ($count == 1) {
        break;
      }

      $defaults = array_fill(0, $count - 1, "'XXX_YYY'");
      $string = $editor->arrayitize($item, 1, $keys, $defaults);

      $temp = $editor->expressionToStatement($string);
      $temp->getElement(0)->multiline = 0;
      $item->setParameter(1, $temp);
      break;

    case 'time': // DONE
      $expression = $editor->expressionToStatement('REQUEST_TIME');
      coder_upgrade_debug_echo($expression->print_r());
      $item = $expression->getElement(0);
      break;

    case 'user_authenticate':
      $count = $item->parameters->count();
      if ($count == 0) {
        $editor->setParameters($item, array('$name', '$password /* TODO Set these variables */'));
        break;
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
        coder_upgrade_debug_echo($assign1->print_r());
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
 *
 * Input Sanitization and Input Formats
 * http://drupal.org/node/224333#filter_urls
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

  // Moved admin/settings/filters/* to admin/settings/filter/*
  $from[] = '@admin\/settings\/filters(\/.*?)@';
  $to[] = "admin\/settings\/filter$1";

  coder_upgrade_do_conversions($from, $to, $new);
  coder_upgrade_save_changes($cur, $new, $file, $hook);
}

/**
 * Function changes using grammar parser.
 *
 * Module Info / Install
 * http://drupal.org/node/224333#update_php
 *
 *
 * Permissions and Access
 * http://drupal.org/node/224333#hook_permission
 * http://drupal.org/node/224333#descriptions_permissions
 * http://drupal.org/node/224333#hook_node_access
 *
 *
 * Database
 * http://drupal.org/node/224333#schema_translation
 * http://drupal.org/node/224333#schema_html (NOT DONE)
 * http://drupal.org/node/224333#install-schema
 *
 *
 * Menus
 * http://drupal.org/node/224333#hook_menu_link_alter
 *
 *
 * Blocks
 * http://drupal.org/node/224333#remove_op (DUP Node API)
 *
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function coder_upgrade_convert_functions(&$reader) {
//  coder_upgrade_debug_print("inside " . __FUNCTION__ . "\n");
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $editor = new PGPEditor();
  $types = array(T_FUNCTION);
  $editor->traverseCallback($reader->getFunctions(), 'coder_upgrade_callback_functions', $types, $reader);
}

/**
 * Callback routine for function changes using grammar parser.
 *
 * The grammar parser API which provides helper functions for working with
 * function objects. Some examples of typical tasks follow.
 *
 * @code
 *   // Rename the function.
 *   $item->name = 'new_function_name';
 *
 *   // Update the document comment.
 *   $item->comment = preg_replace('@hook_perm([^i])@', "hook_permission$1", $item->comment);
 *
 *   // Get the list of body statements (note the use of the reference operator).
 *   $body = &$item->body;
 *
 *   // Find the return statement in the function.
 *   $return = $body->find(T_RETURN, 'reverse');
 * @endcode
 *
 * For other examples of using the grammar parser API
 * @see coder_upgrade_callback_function_calls
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_callback_functions(&$node) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $item = &$node->data;

  global $_coder_upgrade_module_name;

  $name = &$item->name;
  coder_upgrade_debug_echo("name = $name\n");

  /*
   * If the function name does not begin with the module name, then ignore it.
   * This assumes such a function would be an instance of an API hook defined
   * by the contributed module but implemented on behalf of another module. For
   * this use case, the contributed module would define upgrade routines to
   * allow other contributed modules that implement said API to upgrade their
   * code.
   *
   * Example: the Views module defines hooks and implements them on behalf of
   * core modules.
   *
   * Otherwise, strip the module name from the function name and use this as
   * the key in the switch statement. In some cases (e.g. hook_update_N), some
   * additional manipulation and testing needs to be done.
   */
  if (strpos($name, $_coder_upgrade_module_name) !== 0) {
    coder_upgrade_debug_print("Ignoring function '$name' as its name does not begin with the module name");
    return;
  }

  // By convention, the module name should be followed by an underscore.
  $key = substr($name, strlen($_coder_upgrade_module_name) + 1);
  coder_upgrade_debug_echo("key = $key\n");

  // Update hooks need additional manipulation.
  if (preg_match('@update_\d+$@', $key, $matches)) {
    coder_upgrade_debug_echo(print_r($matches, 1));
    $key = 'update_N';
  }

  switch ($key) {
    case 'access':
      // Changes: hook_node_access
      coder_upgrade_convert_access($node);
      break;

    case 'block':
      // Changes: remove_op
      $callback = 'coder_upgrade_callback_block';
      $op_index = 0;
      coder_upgrade_convert_op($node, $callback, $op_index);
      break;

    case 'install':
    case 'uninstall':
      // Changes: install-schema
      coder_upgrade_convert_install($node);
      break;

    case 'menu_link_alter':
      // Changes: hook_menu_link_alter
      coder_upgrade_convert_menu_link_alter($node);
      break;

    case 'perm':
      // Changes: hook_permission and descriptions_permissions
      coder_upgrade_convert_perm($node);
      break;

    case 'schema':
      // Changes: schema_translation and schema_html
      coder_upgrade_convert_schema($node);
      break;

    case 'update_N':
      // Changes: update_php
      coder_upgrade_convert_update_N($node);
      break;
  }
}

/**
 * Updates hook_access().
 *
 * Replace hook_access() with hook_node_access().
 * Switch places of first two parameters.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_access(&$node) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $item = &$node->data;
  coder_upgrade_debug_echo($item->print_r());

  global $_coder_upgrade_module_name;

  // Rename function.
  $item->name = $_coder_upgrade_module_name . '_node_access';
  // Update document comment.
  $item->comment = preg_replace('@hook_access([^i])@', "hook_node_access$1", $item->comment);

  // Switch places of the first two parameters.
//  echo "Printing parameters\n";
  coder_upgrade_debug_echo($item->parameters->print_r());

  $count = $item->parameters->count();
  // Adjust parameters.
  if ($count > 1) {
    // Switch places.
    $p0 = $item->getParameter(0);
    $p1 = $item->getParameter(1);
    $item->setParameter(0, $p1);
    $item->setParameter(1, $p0);
  }
}

/**
 * Updates hook_install() or hook_uninstall().
 *
 * Database schema (un)installed automatically.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_install($node) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  coder_upgrade_debug_echo($name . "\n");
  $item = &$node->data;

  // Get body statements.
  $body = &$item->body;

  /*
   * In 6.x, drupal_install_schema has a return value, but not in 7.x.
   * The code below asssumes the return value is not utilized. Otherwise,
   * set the variable to empty string.
   */

  $current = $body->first();
  while ($current->next != NULL) {
    $statement = &$current->data;
    coder_upgrade_debug_echo($statement->print_r());
    if (is_a($statement, 'PGPAssignment')) {
      $text = $statement->values->toString();
      coder_upgrade_debug_echo($text . "\n");
      if (preg_match('(drupal_install_schema|drupal_uninstall_schema)', $text)) {
        $editor = new PGPEditor();
        // Insert comment.
        $statements = $editor->textToStatements('// TODO The drupal_(un)install_schema functions are called automatically in D7.');
        $body->insertBefore($current, $statements->getElement(), 'comment');
        // Comment out the statement.
        $statements = $editor->textToStatements('// ' . $text);
        $statement = $statements->getElement();
        break;
      }
    }
    $current = &$current->next;
  }
}

/**
 * Updates hook_menu_link_alter().
 *
 * Changed hook_menu_link_alter() (removed the $menu parameter).
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_menu_link_alter($node) {
  $item = &$node->data;
  $count = $item->parameters->count();
  // Adjust parameters.
  if ($count > 1) {
    // Delete second parameter.
    $item->deleteParameter(1);
  }
  // TODO Do we need to check for $menu in the body of this hook?
}

/**
 * Updates hook_perm().
 *
 * Rename hook_perm() to hook_permission().
 * Permissions are required to have titles and descriptions.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_perm(&$node) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $item = &$node->data;
  coder_upgrade_debug_echo($item->print_r());

  // Rename function.
  $item->name .= 'ission';
  // Update document comment.
  $item->comment = preg_replace('@hook_perm([^i])@', "hook_permission$1", $item->comment);

  // Restructure the permissions array.
  $body = &$item->body;

  if (!($return = $body->find(T_RETURN, 'reverse'))) {
    coder_upgrade_debug_print("ERROR: return statement not found in hook_perm");
    return;
  }
//  echo "Printing return item\n";
  $value = &$return['value'];
  coder_upgrade_debug_echo($value->print_r());
  coder_upgrade_debug_echo($value->toString());
//  echo "Printing return item DONE\n";

  $array = $value->getElement();
  if (!is_a($array, 'PGPArray')) {
    coder_upgrade_debug_print("ERROR: return statement does not include an array of values in hook_perm");
    return;
  }
  // Grab the PGPList of values.
  $values = $array->values;

  $string = "array(\n";
  $current = $values->first();
  while ($current->next != NULL) {
    if ($current->type == 'value') {
      // If the permission is other than a string (e.g. T_VARIABLE expression),
      // then do not enclose in quotes.
      $expression = &$current->data->first()->data;
      $type = is_object($expression) ? $expression->type : $expression['type'];
      $add_quotes = $type == T_CONSTANT_ENCAPSED_STRING;

      $permission = &$current->data->toString();
//      coder_upgrade_debug_echo("permission = $permission\n");
      $permission = trim($permission, "'\"");
      if ($add_quotes) {
        $string .= "'$permission' => array('title' => t('$permission'), 'description' => t('TODO Add a description for $permission'),),\n";
      }
      else {
        $permission2 = str_replace("'", "\'", $permission);
        $string .= "$permission => array('title' => t($permission), 'description' => t('TODO Add a description for $permission2'),),\n";
      }
    }
    $current = &$current->next;
  }
  $string .= ");\n";
//  coder_upgrade_debug_echo($string);
  $editor = new PGPEditor();
  $expression = $editor->expressionToStatement($string);
//  coder_upgrade_debug_echo("PRINT NEW expression\n");
//  coder_upgrade_debug_echo($expression->print_r(3));
//  coder_upgrade_debug_echo("PRINT NEW expression DONE\n");

  // Set the return array to the new array.
  $value->setElement(0, $expression);
}

/**
 * Updates hook_schema().
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_schema(&$node) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $item = &$node->data;
  $body = &$item->body;

  if (!($return = $body->find(T_RETURN, 'reverse'))) {
    coder_upgrade_debug_print("ERROR: return statement not found in hook_schema");
    return;
  }
//  echo "Printing return item\n";
  coder_upgrade_debug_echo($item->print_r(0, $return));
//  echo "Printing return item DONE\n";
  $variable = $return['value']->first();

  /*
   * Traverse the body statements looking for:
   * - assignment to the return variable
   * - in the assignment
   *   - a PGPArray operand
   *   - in the operand
   *     - key of 'description'
   *     - value whose first operand is PGPArray (recurse into this)
   *       - if the value calls t() then remove t()
   */
  $body->searchCallback($body, 'coder_upgrade_convert_schema_callback', 'PGPFunctionCall', 'name', 'value', 't');
}

function coder_upgrade_convert_schema_callback(&$item) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
//  coder_upgrade_debug_echo($item->print_r());

  if (get_class($item) != 'PGPFunctionCall') {
    return;
  }

  // Fetch the first parameter of the t() call.
  $parameter = $item->getParameter();
  $operand = $parameter->getElement();
//  coder_upgrade_debug_echo("operand\n");
//  coder_upgrade_debug_echo(print_r($operand, 1));

  // Parent should be the value expression in an array (key, value) pair.
  $parent = &$item->parentExpression;
  // Set the value to the first parameter of the t() call.
  if ($parent->count() == 1) {
    // This is an example of changing a function call reference.
    $parent->setElement(0, $operand);
  }
}

/**
 * Updates hook_update_N().
 *
 * Check hook_update_N for a Doxygen style comment.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_update_N(&$node) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $item = &$node->data;
//  coder_upgrade_debug_echo($item->print_r());

  $comment = &$item->comment;
  if (!is_array($comment) || empty($comment)) {
    $comment = array(
      'type' => T_DOC_COMMENT,
      'value' => "/**\n * @todo Please insert a Doxygen style comment for this hook_update_N.\n *\n */",
    );
  }
  elseif ($comment['type'] != T_DOC_COMMENT) {
    $comment = array(
      'type' => T_DOC_COMMENT,
      'value' => "/**\n * @todo Please insert a Doxygen style comment for this hook_update_N.\n *\n *" . $comment['value'] . "\n */",
    );
  }
}

/**
 * Initiates the transformation of $op hook to a new hook_$op style function.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 * @param string $callback
 *   A string of the callback function for the hook.
 * @param integer $op_index
 *   An integer of the operation parameter in the function parameter list.
 */
function coder_upgrade_convert_op(&$node, $callback, $op_index) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  coder_upgrade_debug_echo("$callback\n");

  /*
   * DBTNG changes can be done in another routine
   */

  // Get the function object.
  $item = &$node->data;
  // Rename the function in case any code is left over.
  $item->name .= '_OLD';
  // Get the operation function parameter, usually called $op.
  $count = $item->parameters->count();
  // TODO This gets the entire parameter including any default value. Hook_block has $op = 'list'.
  $op = $item->printParameter($op_index);

  // Get the function body statements.
  $body = &$item->body;

  /*
   * Two likely cases: switch statement or series of if blocks.
   * Do the if blocks later.
   * Compare the second parameter to the function with the switch operand.
   */

  // Loop on the body statements looking for the $op parameter in an IF or
  // SWITCH condition.
  $current = $body->first();
  while ($current->next != NULL) {
    $statement = &$current->data;
    if (is_object($statement)) {
      coder_upgrade_debug_echo($statement->print_r());
    }
    if (is_a($statement, 'PGPConditional')) {
//      echo "inside PGPConditional check\n";
//      echo "statement->type = " . $statement->type . "\n";
      if ($statement->type == T_SWITCH) {
//        echo "inside T_SWITCH check\n";
        // Get the list of conditions.
        $conditions = $statement->conditions;
        // Get the first condition. (With a switch there should only be one condition.)
        $condition = $conditions->getElement();
        $operand = $condition->toString();
        // If the condition variable matches the $op variable, then go to work.
        if ($op == $operand) {
          $cases = $statement->body;
          $node->traverse($cases, $callback);
        }
      }
      elseif (in_array($statement->type, array(T_IF, T_ELSEIF/*, T_ELSE*/))) {
//        echo "inside T_IF check\n";
        /*
         * Extract the conditions referencing the $op variable and loop on them.
         * These are conditions of the form $op == 'operation'.
         * Replace them with condition of TRUE to not disrupt the logic.
         * Retain any other conditions as part of the body in the new hook
         * function.
         */
        $operations = coder_upgrade_extract_operations($statement->conditions, $op);
        // Loop on the extracted operations.
        foreach ($operations as $operation) {
          // Change a T_ELSEIF to a T_IF in the new hook function.
          $statement->type = T_IF; // If it isn't already.
          $block = new stdClass();
          $block->body = new PGPList();
          $block->body->insertLast($statement);
          $case_node = new PGPNode($block, $current->container); // TODO What is the correct container???
          $callback($node, $case_node, $operation);
        }
      }
    }
    // Move to next node.
    $current = &$current->next;
    // Get the statement list the switch statement (or if block) node is part of.
    $container = &$current->container;
    $container->delete($current->previous);
  }
}

/**
 * Extracts operations from conditions and replaces the conditions with TRUE.
 *
 * @param PGPList $conditions
 *   A list of conditions to an if block.
 * @param string $op
 *   A string of the hook operation.
 * @return array
 *   Array of operations referenced in the if block.
 */
function coder_upgrade_extract_operations(&$conditions, $op) {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");
  $operations = array();

  /*
   * A condition may consist of at most two operands separated by an operator.
   */
  if (is_a($conditions, 'PGPList')) {
    // Iterate over the conditions of the condition list.
    $current = $conditions->first();
    while ($current->next != NULL) {
      $type = $current->type;
      if ($type == 'condition') {
        // Get the condition object of the current node.
        $condition = &$current->data;
        // Iterate over elements of the condition expression.
        $found = FALSE;
        $current2 = $condition->first();
        while ($current2->next != NULL) {
          if ($current2->type == 'operand') {
            // Get the operand (object or array) of the current node.
            $element = &$current2->data;
            // Inspect the element looking for $op.
            if (is_a($element, 'PGPOperand')) {
              // Inspect the operand looking for $op.
              $text = $element->toString();
              if (strpos($text, $op) !== FALSE) {
                $found = TRUE;
              }
              else {
                $operation = $element->toString();
              }
            }
            elseif (is_array($element)) {
              // This should have type = T_CONSTANT_ENCAPSED_STRING.
              $operation = $element['value'];
            }
          }
          // An interesting effect takes place with an & on the next line.
          $current2 = /*&*/$current2->next;
        }
        if ($found) {
          // Replace condition with TRUE so the logic remains the same.
          $condition->clear();
          $data = array(
            'type' => T_STRING,
            'value' => 'TRUE',
          );
          $condition->insertLast($data, 'operand');

          // Add operation to list.
          $operations[] = trim($operation, "'\"");
        }
      }
      $current = /*&*/$current->next;
    }
  }

  return $operations;
}

/**
 * Prepares the information needed to create a new hook_$op style function.
 *
 * This is a series of functions -- one for each existing hook to be modified.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 * @param PGPNode $case_node
 *   A node object containing a PGPCase (or PGPConditional) item.
 * @param string $operation
 *   A string of the operation to create a new hook for.
 */

/**
 * Updates hook_block().
 *
 * hook_nodeapi, hook_node_type, hook_user, and hook_block removed and replaced
 * with families of related functions
 */
function coder_upgrade_callback_block($node, $case_node, $operation = '') {
  coder_upgrade_debug_echo("inside " . __FUNCTION__ . "\n");

  if (!$operation) {
    $case = &$case_node->data;
    if (!is_a($case, 'PGPCase')) {
      coder_upgrade_debug_echo("Houston, we've got an unexpected statement\n");
      return;
    }
    $operation = $case->case->toString();
    $operation = trim($operation, "'\"");
  }

  $hook = '_block_' . str_replace(' ', '_', $operation);
  $parameters = array('$delta');

  switch ($operation) {
    case 'configure':
      // This block becomes example_block_configure
      break;
    case 'list':
      // This block becomes example_block_list
      $hook = '_block_info';
      $parameters = array();
      break;
    case 'save':
      // This block becomes example_block_save
      $parameters = array('$delta', '$edit');
      break;
    case 'view':
      // This block becomes example_block_view
      break;
    default:
      coder_upgrade_debug_echo("ERROR: Invalid case value\n");
      return;
  }

  // Create the new hook function.
  coder_upgrade_op_to_hook($node, $case_node, $hook, $parameters);
}

/**
 * Creates hook_$op function from the case (of if) block of an $op-style hook.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 * @param PGPNode $case_node
 *   A node object containing a PGPCase item.
 * @param string $hook
 *  A string of the new function name.
 * @param array $parameters
 *  An array of function parameters.
 */
function coder_upgrade_op_to_hook($node, $case_node, $hook, $parameters) {
  /*
   * Copy the case body to the new hook function.
   * Insert before (or after) the $item function.
   *
   * When case body is empty (e.g. insert, update), then use next reference
   * until a non-empty body is found.
   *
   * TODO
   * Add the new function to the list of functions.
   * This is useful when we may need to check for the existence of a function
   * on another upgrade.
   * Example: hook_link() becomes part of hook_node_view()
   * or hook_comment_view() based on $type parameter. Also hook_link_alter()
   * code goes in hook_node_view_alter() or hook_comment_view_alter().
   * See http://drupal.org/node/224333#node_links.
   */

  global $_coder_upgrade_module_name;
  $case = &$case_node->data;

  // Set values for the new hook function.
  $comment = array(
    'type' => T_DOC_COMMENT,
    'value' => "/**\n * Implements hook$hook.\n */",
  );
  $name = $_coder_upgrade_module_name . $hook;

  // Create the new hook function.
  $function = new PGPClass($name);
  $function->comment = $comment;
  $function->type = T_FUNCTION;
  $function->parameters = new PGPList();

  // Use the editor to set the function parameters.
  $editor = new PGPEditor();
  $editor->setParameters($function, $parameters);

  // Copy the case (or if) block as the body of the function.
  $function->body = $case->body;
  if ($function->body->isEmpty()) {
    // TODO Clone the node??? Use a while loop since there could be more than two cases back to back.
    $case_node2 = &$case_node->next;
    $case2 = &$case_node2->data;
    $body2 = $case2->body;
    if (!$body2->isEmpty()) {
      $function->body = clone $case2->body;
    }
  }

  // Remove the break statement from a case block.
  if (($break = $function->body->find(T_BREAK, 'reverse', TRUE))) {
    coder_upgrade_debug_echo("return statement found in hook\n");
    $function->body->delete($break);
  }
  // Remove any trailing blank lines (after break) that are included in body.
  $last = $function->body->last();
  if (is_array($last->data) && $last->data['type'] == T_WHITESPACE) {
    coder_upgrade_debug_echo("YAHOO: found whitespace statement in hook_nodeapi\n");
    $function->body->delete($last);
  }

  // Get the statement list the function node is part of.
  $container = &$node->container;

  // Insert the new function before the old function.
  $container->insertBefore($node, $function, 'function');
  // Insert a blank line.
  $whitespace = array(
    'type' => T_WHITESPACE,
    'value' => 1,
  );
  $container->insertBefore($node, $whitespace, 'whitespace');
}
