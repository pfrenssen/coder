<?php
// $Id$

/**
 * @file
 * Function (or hook) conversion routine file for the coder_upgrade module.
 * These routines upgrade functions (or hooks) using the grammar parser.
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
 * http://drupal.org/node/224333#schema_html
 * http://drupal.org/node/224333#install-schema
 * http://drupal.org/node/224333#block_deltas_renamed
 *
 *
 * Menus
 * http://drupal.org/node/224333#hook_menu_link_alter
 *
 *
 * Blocks
 * http://drupal.org/node/224333#remove_op (DUP Node API)
 * http://drupal.org/node/224333#block_deltas_renamed (DUP Database)
 *
 *
 * Comments
 * http://drupal.org/node/224333#remove_op (DUP Node API) (THIS IS MISSING FROM THE CHRONO PAGE!!!)
 *
 *
 * Input Sanitization and Input Formats
 * http://drupal.org/node/224333#hook_filter_info
 *
 *
 * Theming
 * http://drupal.org/node/224333#hook_theme_render_changes
 *
 *
 * User API
 * http://drupal.org/node/224333#user_cancel (DONE with remove_op)
 * http://drupal.org/node/224333#remove_op (DUP Node API)
 *
 *
 * Node API
 * http://drupal.org/node/224333#node_links (NOT DONE)
 * http://drupal.org/node/224333#node_type_base (NOT DONE)
 * http://drupal.org/node/224333#remove_op (SPANS MULTIPLE HOOKS)
 * http://drupal.org/node/224333#node_build_rss
 * http://drupal.org/node/224333#build_mode
 * http://drupal.org/node/224333#hook_node_xxx
 *
 *
 * Miscellaneous
 * http://drupal.org/node/224333#trigger_overhaul
 */

/**
 * Implements hook_upgrade_hook_access_alter().
 */
function coder_upgrade_upgrade_hook_access_alter(&$node, &$reader) {
  // Changes: hook_node_access
  coder_upgrade_convert_access($node);
}

/**
 * Implements hook_upgrade_hook_action_info_alter().
 */
function coder_upgrade_upgrade_hook_action_info_alter(&$node, &$reader) {
  // Changes: trigger_overhaul
  coder_upgrade_convert_return($node, 'action_info');
}

/**
 * Implements hook_upgrade_hook_block_alter().
 */
function coder_upgrade_upgrade_hook_block_alter(&$node, &$reader) {
  // Changes: block_deltas_renamed
  // Changes: remove_op
  $callback = 'coder_upgrade_callback_block';
  $op_index = 0;
  coder_upgrade_convert_op($node, $callback, $op_index);
}

/**
 * Implements hook_upgrade_hook_comment_alter().
 */
function coder_upgrade_upgrade_hook_comment_alter(&$node, &$reader) {
  // Changes: remove_op
  $callback = 'coder_upgrade_callback_comment';
  $op_index = 1;
  coder_upgrade_convert_op($node, $callback, $op_index);
}

/**
 * Implements hook_upgrade_hook_filter_alter().
 */
function coder_upgrade_upgrade_hook_filter_alter(&$node, &$reader) {
  // Changes: hook_filter_info
  coder_upgrade_convert_filter($node, $reader);
}

/**
 * Implements hook_upgrade_hook_hook_info_alter().
 */
function coder_upgrade_upgrade_hook_hook_info_alter(&$node, &$reader) {
  // Changes: trigger_overhaul
  $item = &$node->data;

  global $_coder_upgrade_module_name;

  // Rename function.
  $item->name = $_coder_upgrade_module_name . '_trigger_info';
  // Update document comment.
  $item->comment = preg_replace('@hook_hook_info@', "hook_trigger_info", $item->comment);

  // Restructure the triggers array.
  coder_upgrade_convert_return($node, 'hook_info');
}

/**
 * Implements hook_upgrade_hook_install_alter().
 */
function coder_upgrade_upgrade_hook_install_alter(&$node, &$reader) {
  // Changes: install-schema
  coder_upgrade_convert_install($node);
}

/**
 * Implements hook_upgrade_hook_uninstall_alter().
 */
function coder_upgrade_upgrade_hook_uninstall_alter(&$node, &$reader) {
  // Changes: install-schema
  coder_upgrade_convert_install($node);
}

/**
 * Implements hook_upgrade_hook_link_alter().
 */
function coder_upgrade_upgrade_hook_link_alter(&$node, &$reader) {
  // Changes: build_mode and others !!!???
  coder_upgrade_convert_link($node);
}

/**
 * Implements hook_upgrade_hook_menu_link_alter_alter().
 */
function coder_upgrade_upgrade_hook_menu_link_alter_alter(&$node, &$reader) {
  // Changes: hook_menu_link_alter
  coder_upgrade_convert_menu_link_alter($node);
}

/**
 * Implements hook_upgrade_hook_nodeapi_alter().
 */
function coder_upgrade_upgrade_hook_nodeapi_alter(&$node, &$reader) {
  // Changes: build_mode, remove_op and others !!!???
  $callback = 'coder_upgrade_callback_nodeapi';
  $op_index = 1;
  coder_upgrade_convert_op($node, $callback, $op_index);
}

/**
 * Implements hook_upgrade_hook_node_info_alter().
 */
function coder_upgrade_upgrade_hook_node_info_alter(&$node, &$reader) {
  // Changes: node_type_base
  coder_upgrade_convert_return($node, 'node_info');
}

/**
 * Implements hook_upgrade_hook_node_type_alter().
 */
function coder_upgrade_upgrade_hook_node_type_alter(&$node, &$reader) {
  // Changes: remove_op
  $callback = 'coder_upgrade_callback_node_type';
  $op_index = 0;
  coder_upgrade_convert_op($node, $callback, $op_index);
}

/**
 * Implements hook_upgrade_hook_perm_alter().
 */
function coder_upgrade_upgrade_hook_perm_alter(&$node, &$reader) {
  // Changes: hook_permission and descriptions_permissions
  coder_upgrade_convert_perm($node);
}

/**
 * Implements hook_upgrade_hook_schema_alter().
 */
function coder_upgrade_upgrade_hook_schema_alter(&$node, &$reader) {
  // Changes: schema_translation and schema_html
  coder_upgrade_convert_schema($node);
}

/**
 * Implements hook_upgrade_hook_theme_alter().
 */
function coder_upgrade_upgrade_hook_theme_alter(&$node, &$reader) { // (OMIT)
  // Changes: hook_theme_render_changes
  coder_upgrade_convert_return($node, 'theme');
}

/**
 * Implements hook_upgrade_hook_update_N_alter().
 */
function coder_upgrade_upgrade_hook_update_N_alter(&$node, &$reader) {
  // Changes: update_php
  coder_upgrade_convert_update_N($node);
}

/**
 * Implements hook_upgrade_hook_user_alter().
 */
function coder_upgrade_upgrade_hook_user_alter(&$node, &$reader) {
  // Changes: remove_op, user_cancel and others !!!???
  $callback = 'coder_upgrade_callback_user';
  $op_index = 0;
  coder_upgrade_convert_op($node, $callback, $op_index);
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
  cdp("inside " . __FUNCTION__);
  $item = &$node->data;
  cdp($item->print_r());

  global $_coder_upgrade_module_name;

  // Rename function.
  $item->name = $_coder_upgrade_module_name . '_node_access';
  // Update document comment.
  $item->comment['value'] = preg_replace('@hook_access([^i])@', "hook_node_access$1", $item->comment['value']);

  // Switch places of the first two parameters.
  //  cdp("Printing parameters");
  cdp($item->parameters->print_r());

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
 * Updates hook_filter().
 *
 * hook_filter() and hook_filter_tips() replaced by hook_filter_info().
 *
 * @todo Integrate hook_filter_tips() code with this function.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_filter(&$node, &$reader) { // DONE
  cdp("inside " . __FUNCTION__);
  $item = &$node->data;

  global $_coder_upgrade_module_name;

  $tips = array();
  $editor = PGPEditor::getInstance();
  // Find the hook_filter_tips function object.
  $function_node = $editor->findFunction($reader->getFunctions(), $_coder_upgrade_module_name . '_filter_tips', 'node');
  if (!is_null($function_node)) {
    $tips = coder_upgrade_convert_filter_tips($function_node);
  }

  // Rename function.
  $item->name .= '_XXX'; // $item->name = $_coder_upgrade_module_name . '_filter_info';
  // Update document comment.
  //  $item->comment = preg_replace('@hook_filter@', "hook_filter_info", $item->comment);

  // Restructure the triggers array.
  $body = &$item->body;

  if (!($switch1 = $body->find(T_SWITCH))) {
    clp("ERROR: switch statement not found in hook_filter");
    return;
  }
  //  $value = &$switch1['value'];

  /*
   * Compare first parameter to first switch condition (s/b $op)
   * Compare second parameter to second switch condition (s/b $delta)
   *
   * Read $op case block: case operand gives the key for array
   * Read $delta case block: case operand gives the key for array
   * Return value gives the array value for above keys
   * Build array internally, then write it out.
   *
   * On the callback items, we only want the callback function name, not the
   * parameters. If there is not a function name (e.g. the filter module had
   * case 'process': case 4: return trim(check_plain($text));),
   * then make a new function with the return value as its body.
   * For this example, core created _filter_html_escape($text).
   *
   * Look for a hook_filter_tips($delta, $format, $long = false).
   * If present:
   * - if multiple $delta bodies, then create new callback functions using
   * $delta as part of the function name
   * - else if one, then create new callback function
   * - remove the $delta parameter
   * - add tips_callback parameters to filter_info array items
   */

  $operand1 = $switch1->conditions->toString();
  if ($operand1 != $item->printParameter()) {
    clp("ERROR: switch statement operand does not match first function parameter in hook_filter");
    return;
  }

  $filters = array();
  // Get list of case statements.
  $cases1 = $switch1->body;
  $current1 = $cases1->first();
  while ($current1->next != NULL) {
    $case1 = $current1->data;
    if ($case1->type == T_DEFAULT) {
      $current1 = $current1->next;
      continue;
    }
    $key1 = trim($case1->case->toString(), "'\"");
    cdp("key1 = $key1");
    // TODO Convert key2 to new key: Ex. 'process' becomes 'process callback'
    $key1 = coder_upgrade_callback_filter($key1);
    cdp("new key1 = $key1");
    $body1 = $case1->body->getElement();
    if (get_class($body1) == 'PGPAssignment' /*&& $body1->type == T_RETURN*/) {
      // Use case 1: returns an array.
      cdp("body1 is an array");
      if ($key1 != 'name') {
        clp("ERROR: key is not 'list' in hook_filter");
        return;
      }
      if ($body1->type != T_RETURN) {
        clp("ERROR: switch statement body for 'list' key does not return an array in hook_filter");
        return;
      }
      $value1 = $body1->value->getElement();
      if (get_class($value1) == 'PGPArray') {
        //        eval("\$temp = " . $body1['value']->toString() . ";");
        //        cdp(print_r($temp, 1));
        //        foreach ($temp as $delta => $value) {
        //          $filters[$delta][$key1] = $value;
        //        }

        $node_x = $value1->values->first();
        while ($node_x->next != NULL) {
          $data = $node_x->data;
          if ($node_x->type == 'key') {
            $key2 = $data->toString(); // Should be an integer = $delta
          }
          elseif ($node_x->type == 'value') {
            $filters[$key2][$key1] = $data->toString();
          }
          $node_x = $node_x->next;
        }

        //        $filters[$key1] = $value1->values->toString();
        //        cdp($value1->values->toString());
      }
    }
    elseif (get_class($body1) == 'PGPConditional' && $body1->type == T_SWITCH) {
      // Use case 2: switch statement on $delta.
      $switch2 = &$body1;
      $operand2 = $switch2->conditions->toString();
      if ($operand2 != $item->getParameter(1)->getElement()->toString()) {
        clp("ERROR: switch statement operand does not match second function parameter in hook_filter");
        return;
      }
      // Get list of case statements.
      $cases2 = $switch2->body;
      $current2 = $cases2->first();
      while ($current2->next != NULL) {
        $case2 = $current2->data;
        if ($case2->type == T_DEFAULT) {
          $current2 = $current2->next;
          continue;
        }
        $key2 = trim($case2->case->toString(), "'\"");
        cdp("key2 = $key2");
        $body2 = $case2->body->getElement();
        if (get_class($body2) == 'PGPAssignment') {
          // Use case 2: returns an array.
          cdp("body2 is an array");
          if ($body2->type != T_RETURN) {
            clp("ERROR: switch statement operand does not match first function parameter in hook_filter");
            return;
          }
          $value2 = $body2->value; // ->getElement();
          if (get_class($value2) == 'PGPExpression') {
            // TODO On the callback items only want the callback function name, not the parameters!!!
            $filters[$key2][$key1] = $body2->value->toString(); // $value2->values->toString();
            cdp($body2->value->toString()); // $value2->values->toString() . "\n";
          }
        }
        $current2 = $current2->next;
      }
    }
    $current1 = $current1->next;
  }
  cdp(print_r($filters, 1));

  $hook = '_filter_info';
  coder_upgrade_new_filter_hook($node, $hook, $filters);

  // Delete the function body.
  $item->body->clear();
}

// TODO Rename this function.
function coder_upgrade_callback_filter($key2) { // DONE
  switch ($key2) {
    case 'list':
      return 'name';
    case 'process':
      return 'process callback';
    case 'settings':
      return 'settings callback';
    case 'tips':
      return 'tips callback';
    default:
      return $key2;
  }
}

function coder_upgrade_new_filter_hook($node, $hook, $filters = array()) { // DONE
  global $_coder_upgrade_module_name;

  // Set values for the new hook function.
  $comment = array(
    'type' => T_DOC_COMMENT,
    'value' => "/**\n * Implements hook$hook().\n */",
  );
  $name = $_coder_upgrade_module_name . $hook; // '_filter_info';

  // Create the new hook function.
  $function = new PGPClass($name);
  $function->comment = $comment;
  $function->type = T_FUNCTION;
  $function->parameters = new PGPList();

  // Use the editor to set the function parameters.
  $editor = PGPEditor::getInstance();
  //  $editor->setParameters($function, $parameters);

  // Create body statements.
  $string = '';
  foreach ($filters as $key => $filter) {
    $string .= "\$filters[$key] = array(\n";
    foreach ($filter as $key2 => $value) {
      $string .= "\t'$key2' => $value,\n";
    }
    $string .= ");\n";
  }
  $string .= "return \$filters;\n";
  cdp(print_r($string, 1));

  // Copy the case (or if) block as the body of the function.
  $function->body = $editor->textToStatements($string);
  cdp(get_class($function->body));
  cdp($editor->statementsToText($function->body));
  if ($function->body->isEmpty()) {
  }
  //  return ;

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

/**
 * Updates hook_filter_tips().
 *
 * hook_filter() and hook_filter_tips() replaced by hook_filter_info().
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 * @return array
 *   Array of tip callback functions indexed by $delta.
 */
function coder_upgrade_convert_filter_tips(&$node) { // DONE
  cdp("inside " . __FUNCTION__);
  $item = &$node->data;

  global $_coder_upgrade_module_name;

  // Rename function.
  $item->name .= '_XXX';
  // Update document comment.
  //  $item->comment .= 'This function is obsolete.'; // TODO Make a comment manipulation routine.

  // Restructure the triggers array.
  $body = &$item->body;

  if (!($switch1 = $body->find(T_SWITCH))) {
    clp("ERROR: switch statement not found in hook_filter_tips");
    return array();
  }

  /*
   * Compare first parameter to first switch condition (s/b $op)
   *
   * Read $delta case block: case operand gives the key for array
   * Return value gives the array value for above keys
   * Build array internally, then write it out.
   *
   * Look for a hook_filter_tips($delta, $format, $long = false).
   * If present:
   * - if multiple $delta bodies, then create new callback functions using
   *   $delta as part of the function name
   * - else if one, then create new callback function
   * - remove the $delta parameter
   * - add tips_callback parameters to filter_info array items
   *
   * - TODO Case 2: could be an if block
   */

  $operand1 = $switch1->conditions->toString();
  if ($operand1 != $item->printParameter()) {
    clp("ERROR: switch statement operand does not match first function parameter in hook_filter_tips");
    return array();
  }

  // Remove first parameter for new callback function.
  // TODO Do we need to clone this object for each new function???
  $parameters1 = $item->parameters;
  $parameters1->deleteElement();

  $tips = array();
  // Get list of case statements.
  $cases1 = $switch1->body;
  $current1 = $cases1->first();
  while ($current1->next != NULL) {
    $case1 = $current1->data;
    if ($case1->type == T_DEFAULT) {
      $current1 = $current1->next;
      continue;
    }
    // Get the $delta value.
    $key1 = trim($case1->case->toString(), "'\"");
    cdp("key1 = $key1");
    $body1 = $case1->body->getElement();

    // Make a new callback function.
    $hook = '_filter_tips_' . $key1;
    coder_upgrade_new_filter_tips_hook($node, $hook, $parameters1, $body1);
    // Store callback name in return array.
    $tips[$key1] = $_coder_upgrade_module_name . $hook;

    $current1 = $current1->next;
  }

  // Delete the function body.
  $item->body->clear();

  return $tips;
}

function coder_upgrade_new_filter_tips_hook($node, $hook, $parameters, $body) { // DONE
  global $_coder_upgrade_module_name;

  // Set values for the new hook function.
  $delta = substr($hook, strrpos($hook, '_') + 1);
  $comment = array(
    'type' => T_DOC_COMMENT,
    'value' => "/**\n * Filter tips callback function for \$filters[$delta] in hook_filter_info().\n */",
  );
  $name = $_coder_upgrade_module_name . $hook;

  // Create the new hook function.
  $function = new PGPClass($name);
  $function->comment = $comment;
  $function->type = T_FUNCTION;
  $function->parameters = $parameters; // new PGPList();
  $function->body = new PGPBody();
  $function->body->insertLast($body);

  //  cdp($function->print_r());

  // TODO REFACTOR: The following statements are repeated in other create_hook routines.

  // Get the statement list the function node is part of.
  $container = &$node->container;

  // Insert the new function before the old function.
  $new_node = $container->insertBefore($node, $function, 'function');
  //  $editor = PGPEditor::getInstance();
  //  cdp($editor->statementsToText($new_node));
  // Insert a blank line.
  $whitespace = array(
    'type' => T_WHITESPACE,
    'value' => 1,
  );
  $container->insertBefore($node, $whitespace, 'whitespace');
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
  cdp("inside " . __FUNCTION__);
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
    cdp($statement);
    if (is_a($statement, 'PGPAssignment')) {
      $text = $statement->values->toString();
      cdp($text);
      if (preg_match('(drupal_install_schema|drupal_uninstall_schema)', $text)) {
        $editor = PGPEditor::getInstance();
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
 * Updates hook_link().
 *
 * Move node, taxonomy, and comment links into $node->content;
 * Deprecate hook_link() and hook_link_alter().
 *
 * @todo THIS IS NOT DONE.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_link(&$node) { // NOT DONE
  return;
  cdp("inside " . __FUNCTION__);
  cdp("$callback");

  /*
   * Find code related to delete operation (in switch or if)
   * Move block to new function hook_user_cancel
   * Rename $user to $account (toString, preg_replace, convert to expression)
   * Add $method parameter
   * Add switch on $method; add core case values?
   * Place existing code under case 'user_cancel_reassign'
   * Add TODO note to check placement of code
   * DBTNG changes can be done in another routine
   */

  // Get the function object.
  $item = &$node->data;
  // Rename the function in case any code is left over.
  $item->name .= '_OLD'; // $item->name = $item->name . '_OLD'; // $item->name['value'] = $item->name['value'] . '_OLD';
  // Get the first function parameter, usually called $op.
  $count = $item->parameters->count();
  // TODO This gets the entire parameter including any default value. Hook_block has $op = 'list'.
  $op = $item->printParameter($op_index);
  //  cdp("op = '$op'");

  // Get the function body statements.
  $body = &$item->body;

  /*
   * Two likely cases: switch statement or series of if blocks.
   * Do the if blocks later.
   * Compare the second parameter to the function with the switch operand.
   */

  // TODO The following code is common to the remove_op upgrades. Refactor.

  // Loop on the body statements looking for the $op parameter in an IF or
  // SWITCH condition.
  $current = $body->first();
  while ($current->next != NULL) {
    $statement = &$current->data;
    if (is_object($statement)) {
      cdp($statement->print_r());
    }
    if (is_a($statement, 'PGPConditional')) {
      //      cdp("inside PGPConditional check");
      //      cdp("statement->type = " . $statement->type);
      if ($statement->type == T_SWITCH) {
        //        cdp("inside T_SWITCH check");
        // Get the list of conditions.
        $conditions = $statement->conditions;
        // Get the first condition. (With a switch there should only be one condition.)
        $condition = $conditions->getElement();
        $operand = $condition->toString();
        // If the condition variable matches the $op variable, then go to work.
        if ($op == $operand) {
          $cases = $statement->body;
          $node->traverse($cases, $callback /*'coder_upgrade_callback_user'*/);
          //          // Get the statement list the switch statement node is part of.
          //          $container = &$current->container;
          //          $container->delete($current);
          //          $statement->body->clear();
        }
      }
      elseif (in_array($statement->type, array(T_IF, T_ELSEIF/*, T_ELSE*/))) {
        // Do something similar.
        //        cdp("inside T_IF check");
        // Get the list of conditions.
        //        $conditions = $statement->conditions;
        // Get the text of the conditions.
        //        $conditions = $statement->conditionsToArray();
        //        print_r($conditions);
        $operations = coder_upgrade_extract_operations($statement->conditions, $op); // list($operations, $others) = coder_upgrade_separate_operators($conditions, $op);
        /*
         * Extract the conditions referencing the $op variable and loop on them.
         * These are conditions of the form $op == 'operation'.
         * Replace them with condition of TRUE to not disrupt the logic.
         * Retain any other conditions as part of the body in the new hook
         * function.
         * Do we need a helper function to find the operand with $op???
         * Determine the $op comparison value (i.e. $op == 'what') so we can
         * use a case block in the upgrade routine.
         * Change a T_ELSEIF to a T_IF in the new hook function if we have
         * extra conditions.
         */
        foreach ($operations as $operand) {
 // TODO Rename $operand to $operation here and in called functions
          $statement->type = T_IF; // If it isn't already.
          $block = new stdClass();
          $block->body = new PGPBody();
          $block->body->insertLast($statement);
          $case_node = new PGPNode($block, $current->container); // TODO What is the correct container???
          $callback($node, $case_node, $operand); // coder_upgrade_callback_user($node, $case_node, $operand);
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
 * Updates hook_menu_link_alter().
 *
 * Changed hook_menu_link_alter() (removed the $menu parameter).
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_menu_link_alter(&$node) {
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
  cdp("inside " . __FUNCTION__);
  $item = &$node->data;
  cdp($item->print_r());

  // Rename function.
  $item->name .= 'ission';
  // Update document comment.
  $item->comment = preg_replace('@hook_perm([^i])@', "hook_permission$1", $item->comment);

  // Restructure the permissions array.
  $body = &$item->body;

  if (!($return = $body->find(T_RETURN, 'reverse'))) {
    clp("ERROR: return statement not found in hook_perm");
    return;
  }
  //  cdp("Printing return item");
  $value = &$return->value;
  cdp($value->print_r());
  cdp($value->toString());
  //  cdp("Printing return item DONE");

  $array = $value->getElement();
  if (!is_a($array, 'PGPArray')) {
    clp("ERROR: return statement does not include an array of values in hook_perm");
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
      // TODO Fix next line when condition is TRUE
      $type = is_object($expression) ? '$expression->type' : $expression['type']; // Operand object does not have a type parameter // $type = is_object($expression) ? $expression->type : $expression['type'];
      $add_quotes = $type == T_CONSTANT_ENCAPSED_STRING;

      $permission = &$current->data->toString();
      //      cdp("permission = $permission");
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
  //  cdp($string);
  $editor = PGPEditor::getInstance();
  $expression = $editor->expressionToStatement($string);
  //  cdp("PRINT NEW expression");
  //  cdp($expression->print_r(3));
  //  cdp("PRINT NEW expression DONE");

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
  cdp("inside " . __FUNCTION__);
  $item = &$node->data;
  $body = &$item->body;

  if (!($return = $body->find(T_RETURN, 'reverse'))) {
    clp("ERROR: return statement not found in hook_schema");
    return;
  }
  //  cdp("Printing return item");
  cdp($item->print_r(0, $return));
  //  cdp("Printing return item DONE");
  $variable = $return->value->first();

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
  $body->searchCallback('coder_upgrade_convert_schema_callback', 'PGPFunctionCall', 'name', 'value', 't');
}

function coder_upgrade_convert_schema_callback(&$item /*&$node*/) {
  cdp("inside " . __FUNCTION__);
  //  cdp($item->print_r());

  if (get_class($item) != 'PGPFunctionCall') {
    return;
  }

  // Fetch the first parameter of the t() call.
  $parameter = $item->getParameter();
  $operand = $parameter->getElement();
  if (is_array($operand)) {
    // schema_html: schema descriptions are now plain text instead of HTML.
    $operand['value'] = html_entity_decode($operand['value']);
  }
  //  cdp("operand");
  //  cdp(print_r($operand, 1));

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
 * @todo These hooks do not need to carry over from one version to the next.
 *   So we could simply delete the hook or its body.
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_update_N(&$node) {
  cdp("inside " . __FUNCTION__);
  $item = &$node->data;
  //  cdp($item->print_r());

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
  cdp("inside " . __FUNCTION__);
  cdp("$callback");

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
      cdp($statement->print_r());
    }
    if (is_a($statement, 'PGPConditional')) {
      //      cdp("inside PGPConditional check");
      //      cdp("statement->type = " . $statement->type);
      if ($statement->type == T_SWITCH) {
        //        cdp("inside T_SWITCH check");
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
        //        cdp("inside T_IF check");
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
          $block->body = new PGPBody();
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
  cdp("inside " . __FUNCTION__);
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
          $current2 = /*&*/ $current2->next;
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
      $current = /*&*/ $current->next;
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
  cdp("inside " . __FUNCTION__);

  if (!$operation) {
    $case = &$case_node->data;
    if (!is_a($case, 'PGPCase')) {
      cdp("Houston, we've got an unexpected statement");
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
      cdp("ERROR: Invalid case value");
      return;
  }

  // http://drupal.org/node/224333#block_deltas_renamed
  // Change numeric block keys to strings
  $editor = new PGPEditor();
  $body = &$case_node->data->body;
  $text = $body->toString();
  $search = array(
    '/\$blocks\[(\d+)\]/si',
    '/\$delta ?([=!])= ?(\d+)/si',
    '/case (\d+)/si',
  );
  $replace = array(
    '$blocks[\'delta-\1\']',
    '$delta \1= \'delta-\2\'',
    'case \'delta-\1\'',
  );
  $count = 0;
  $text = preg_replace($search, $replace, $text, -1, $count);
  if ($count) {
    $body = $editor->textToStatements($text);
    $body->insertFirst($editor->commentToStatement('// TODO Rename block deltas (e.g. delta-0) to readable strings.', 'comment'));
  }

  // Create the new hook function.
  coder_upgrade_op_to_hook($node, $case_node, $hook, $parameters);
}

/**
 * Updates hook_comment().
 *
 * ADD THIS TO:
 * hook_nodeapi, hook_node_type, hook_user, and hook_block removed and replaced
 * with families of related functions
 */
function coder_upgrade_callback_comment($node, $case_node, $operation = '') {
  cdp("inside " . __FUNCTION__);

  if (!$operation) {
    $case = &$case_node->data;
    if (!is_a($case, 'PGPCase')) {
      cdp("Houston, we've got an unexpected statement");
      return;
    }
    $operation = $case->case->toString();
    $operation = trim($operation, "'\"");
  }

  $hook = '_comment_' . str_replace(' ', '_', $operation);
  $parameters = array('$comment');

  switch ($operation) {
    case 'delete':
      // This block becomes example_comment_delete
      break;
    case 'insert':
      // This block becomes example_comment_insert
      break;
    case 'publish':
      // This block becomes example_comment_publish
      break;
    case 'unpublish':
      // This block becomes example_comment_unpublish
      break;
    case 'update':
      // This block becomes example_comment_update
      break;
    case 'validate':
      // This block becomes example_comment_validate
      break;
    case 'view':
      // This block becomes example_comment_view
      break;
    default:
      cdp("ERROR: Invalid case value");
      return;
  }

  // Create the new hook function.
  coder_upgrade_op_to_hook($node, $case_node, $hook, $parameters);
}

/**
 * Updates hook_nodeapi().
 *
 * hook_nodeapi, hook_node_type, hook_user, and hook_block removed and replaced
 * with families of related functions
 */
function coder_upgrade_callback_nodeapi($node, $case_node, $operation = '') {
  cdp("inside " . __FUNCTION__);

  if (!$operation) {
    $case = &$case_node->data;
    if (!is_a($case, 'PGPCase')) {
      cdp("Houston, we've got an unexpected statement");
      return;
    }
    $operation = $case->case->toString();
    $operation = trim($operation, "'\"");
  }

  $hook = '_node_' . str_replace(' ', '_', $operation);
  $parameters = array('$node');

  switch ($operation) {
    case 'alter':
      // This block becomes example_node_build_alter
      $hook = '_node_build_alter';
      $parameters = array('$build');
      break;
    case 'delete':
      // This block becomes example_node_delete
      break;
    case 'delete revision':
      // This block becomes example_node_revision_delete
      $hook = '_node_revision_delete';
      break;
    case 'insert':
      // This block becomes example_node_insert
      break;
    case 'load':
      // This block becomes example_node_load
      $parameters = array('$node', '$types');
      break;
    case 'prepare':
      // This block becomes example_node_prepare
      break;
    case 'prepare translation':
      // This block becomes example_node_prepare_translation
      break;
    case 'print':
      // This block becomes example_node_view with $view_mode = 'print'
      $hook = '_node_view';
      $parameters = array('$node', '$view_mode = \'print\'');
      break;
    case 'rss item':
      // This block becomes example_node_view with $view_mode = 'rss'
      $hook = '_node_view';
      $parameters = array('$node', '$view_mode = \'rss\'');
      break;
    case 'search result':
      // This block becomes example_node_search_result
      break;
    case 'presave':
      // This block becomes example_node_presave
      break;
    case 'update':
      // This block becomes example_node_update
      break;
    case 'update index':
      // This block becomes example_node_update_index
      break;
    case 'validate':
      // This block becomes example_node_validate
      $parameters = array('$node', '$form');
      break;
    case 'view':
      // This block becomes example_node_view with $view_mode = 'full' by default
      $parameters = array('$node', '$view_mode = \'full\'');
      break;
    default:
      cdp("ERROR: Invalid case value");
      return;
  }

  // Create the new hook function.
  coder_upgrade_op_to_hook($node, $case_node, $hook, $parameters);
}

/**
 * Updates hook_node_type().
 *
 * hook_nodeapi, hook_node_type, hook_user, and hook_block removed and replaced
 * with families of related functions
 */
function coder_upgrade_callback_node_type($node, $case_node, $operation = '') {
  cdp("inside " . __FUNCTION__);

  if (!$operation) {
    $case = &$case_node->data;
    if (!is_a($case, 'PGPCase')) {
      cdp("Houston, we've got an unexpected statement");
      return;
    }
    $operation = $case->case->toString();
    $operation = trim($operation, "'\"");
  }

  $hook = '_node_type_' . str_replace(' ', '_', $operation);
  $parameters = array('$info');

  switch ($operation) {
    case 'delete':
      // This block becomes example_node_type_delete
      break;
    case 'insert':
      // This block becomes example_node_type_insert
      break;
    case 'update':
      // This block becomes example_node_type_update
      break;
    default:
      cdp("ERROR: Invalid case value");
      return;
  }

  // Create the new hook function.
  coder_upgrade_op_to_hook($node, $case_node, $hook, $parameters);
}

/**
 * Updates hook_user().
 *
 * hook_nodeapi, hook_node_type, hook_user, and hook_block removed and replaced
 * with families of related functions
 * Renamed user_delete() to user_cancel();
 * likewise renamed hook_user_delete() to hook_user_cancel(). (Did this exist?)
 */
function coder_upgrade_callback_user($node, $case_node, $operation = '') {
  cdp("inside " . __FUNCTION__);

  if (!$operation) {
    $case = &$case_node->data;
    if (!is_a($case, 'PGPCase')) {
      cdp("Houston, we've got an unexpected statement");
      return;
    }
    $operation = $case->case->toString();
    $operation = trim($operation, "'\"");
  }

  $hook = '_user_' . str_replace(' ', '_', $operation);
  $parameters = array('$edit', '$account');

  // TODO We can end up with multiple copies of same hook if this mapping is accurate???
  switch ($operation) {
    case 'after_update':
      // The user object has been updated and changed. Use this if (probably along with 'insert') if you want to reuse some information from the user object.
      // This block becomes example_user_update
      $hook = '_user_update';
      $parameters = array('&$edit', '$account', '$category');
      break;
    case 'categories':
      // A set of user information categories is requested.
      // This block becomes example_user_categories
      $parameters = array();
      break;
    case 'delete':
      // The user account is being deleted. The module should remove its custom additions to the user object from the database.
      // This block becomes example_user_cancel
      $hook = '_user_cancel';
      $parameters = array('$edit', '$account', '$method');
      break;
    case 'form':
      // The user account edit form is about to be displayed. The module should present the form elements it wishes to inject into the form.
      // This block becomes example_user_???
      $hook = '_user_XXX';
      break;
    case 'insert':
      // The user account is being added. The module should save its custom additions to the user object into the database and set the saved fields to NULL in $edit.
      // This block becomes example_user_insert
      $parameters = array('&$edit', '$account', '$category');
      break;
    case 'load':
      // The user account is being loaded. The module may respond to this and insert additional information into the user object.
      // This block becomes example_user_load
      $parameters = array('$users');
      break;
    case 'login':
      // The user just logged in.
      // This block becomes example_user_login
      $parameters = array('&$edit', '$account');
      break;
    case 'logout':
      // The user just logged out.
      // This block becomes example_user_logout
      $parameters = array('$account');
      break;
    case 'register':
      // The user account registration form is about to be displayed. The module should present the form elements it wishes to inject into the form.
      // This block becomes example_user_???
      $hook = '_user_XXX';
      break;
    case 'submit':
      // Modify the account before it gets saved.
      // This block becomes example_user_???
      $hook = '_user_presave';
      $parameters = array('&$edit', '$account', '$category');
      break;
    case 'update':
      // The user account is being changed. The module should save its custom additions to the user object into the database and set the saved fields to NULL in $edit.
      // This block becomes example_user_presave
      $hook = '_user_presave';
      $parameters = array('&$edit', '$account', '$category');
      break;
    case 'validate':
      // The user account is about to be modified. The module should validate its custom additions to the user object, registering errors as necessary.
      // This block becomes example_user_presave
      $hook = '_user_presave';
      $parameters = array('&$edit', '$account', '$category');
      break;
    case 'view':
      // The user's account information is being displayed. The module should format its custom additions for display, and add them to the $account->content array.
      // This block becomes example_user_view
      $parameters = array('$account', '$view_mode');
      break;
    default:
      cdp("ERROR: Invalid case value");
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
  $editor = PGPEditor::getInstance();
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
    cdp("return statement found in hook");
    $function->body->delete($break);
  }
  // Remove any trailing blank lines (after break) that are included in body.
  $last = $function->body->last();
  if (is_array($last->data) && $last->data['type'] == T_WHITESPACE) {
    cdp("YAHOO: found whitespace statement in hook_nodeapi");
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

/**
 * Initiates the transformation of array assignments in a hook.
 *
 * Applies to: hook_action_info(), hook_hook_info(), hook_node_info(), hook_theme().
 *
 * NOTE
 * In general, there are 3 typical cases (or code styles):
 * - return array('key1' => array('key2' => ...);
 * - $var = array('key1' => array('key2' => ...); return $var;
 * - $var = array(); $var['key1'] = array('key2' => ...); return $var;
 *
 * The inner array to modify is 3 levels deep in the first 2 cases, but only
 * 2 levels deep in the third. In the first 2 cases, we can loop on the key1
 * arrays. In the third, the loop is on assignment statements.
 *
 */
function coder_upgrade_convert_return(&$node, $hook, $callback = '') { // (OMIT)
  cdp("inside " . __FUNCTION__);
  $editor = PGPEditor::getInstance();
  $msg = '// TODO The array elements in this hook function need to be changed.';

  $item = &$node->data;

  // Get the function body.
  $body = &$item->body;

  if (!($return = $body->find(T_RETURN, 'reverse'))) {
    clp("ERROR: return statement not found in hook_$hook");
    $body->insertFirst($editor->commentToStatement($msg, 'comment'));
    return;
  }

  $value = &$return->getParameter();

  // Examine the type of the operand in the return statement.
  $operand = $value->getElement();
  if (get_class($operand) == 'PGPArray') {
    // Use case 1 - returns array directly.
    // The keys of this array are the node type items.
    $array1 = $value->getElement();
    coder_upgrade_callback_return_case1($array1, $hook, $callback);
  }
  elseif (get_class($operand) == 'PGPOperand') {

    /*
     * Loop on body statements until we find an assignment to the return variable.
     * The assignment could be to an array element like $info['node_type_name'] = array(...).
     * Or directly to the variable like $info = array('node_type_name' = array(...)).
     */

    $return_variable = $operand->toString();
    $count = 0;

    $current = $body->first();
    while ($current->next != NULL) {
      if (is_object($current->data) && $current->data->type == T_ASSIGNMENT) { // if ($current->data->isType(T_ASSIGNMENT)) {
        $assignment = $current->data;
        $assign_variable = $assignment->values->getElement()->getElement()->stripComments()->toString();
        if ($return_variable == $assign_variable) {
          // Use case 2: makes one assignment to array variable; returns variable.
          cdp("Assignment variable matches return variable");
          /*
           * TODO This equality check would fail if comments are embedded between
           * variable and operator.
           * Example: $theme /**\/ = $array + array(..);
           */
          $value1 = $assignment->values->getElement();
          /*
           * TODO Find the operand of class PGPArray.
           * Example: $theme /**\/ = $array + array(..);
           */
          $array1 = $value1->getElement($value1->count() - 1);
          if (!is_object($current->data) || get_class($array1) != 'PGPArray') {
            continue;
          }
          coder_upgrade_callback_return_case1($array1, $hook, $callback);
          $count++;
        }
        elseif (strpos($assign_variable, $return_variable) !== FALSE) {
          // Use case 3: makes multiple assignments to array variable; returns variable.
          cdp("Assignment variable matches return variable");
          /*
           * TODO This substring check would fail if comments are embedded between
           * variable and operator.
           * Example: $theme /**\/ = $array + array(..);
           */
          $value1 = $assignment->values->getElement();
          $array1 = $value1->getElement($value1->count() - 1);
          coder_upgrade_callback_return_case3($array1, $hook, $callback);
          $count++;
        }
      }
      $current = $current->next;
    }
    if (!$count) {
      clp("ERROR: assignment statement to return variable not found in hook_$hook");
      $body->insertFirst($editor->commentToStatement($msg, 'comment'));
      return;
    }
  }
  else {
    clp("ERROR: operand of return statement is not an array or variable in hook_$hook");
    $body->insertFirst($editor->commentToStatement($msg, 'comment'));
  }
}

/**
 * Loops on elements of first-level array.
 *
 * @todo Abstract to the PGPArray object: traverse with callback.
 * This is setting up as a nice recursive function.
 *
 * @param PGPArray $array1
 */
function coder_upgrade_callback_return_case1(&$array1, $hook, $callback = '') { // (OMIT)
  cdp("inside " . __FUNCTION__);

  // The keys of this array are the node types.
  if (!is_a($array1, 'PGPArray')) {
    clp("ERROR: array1 is not a PGPArray object in hook_$hook");
    return;
  }

  // Grab the PGPList of values.
  $values1 = $array1->values;

  $current1 = $values1->first();
  while ($current1->next != NULL) {
    if ($current1->type == 'key') {
      $key1 = $current1->data;
      // Do something if appropriate.
      // This is not the one needed; we need the next level down.
    }
    elseif ($current1->type == 'value') {
      // This is the value expression for a node type key.
      $value1 = $current1->data;
      if (get_class($value1) != 'PGPExpression') {
        clp("ERROR: value1 is not a PGPExpression object in hook_$hook");
        return;
      }
      // This is the array of node type items.
      $array2 = $value1->getElement();
      coder_upgrade_callback_return_case3($array2, $hook, $callback);
    }
    $current1 = $current1->next;
  }
}

/**
 * Loops on elements of second-level array.
 *
 * @todo Abstract to the PGPArray object: traverse with callback.
 *
 * @param PGPArray $array2
 */
function coder_upgrade_callback_return_case3(&$array2, $hook, $callback = '') { // (OMIT)
  cdp("inside " . __FUNCTION__);
  $callback = $callback == '' ? "coder_upgrade_callback_$hook" : $callback;

  // The keys of this array are the node type items.
  if (!is_a($array2, 'PGPArray')) {
    clp("ERROR: array2 is not a PGPArray object in hook_$hook");
    return;
  }

  // Grab the PGPList of values.
  $values2 = $array2->values;

  $key2 = '';
  $current2 = $values2->first();
  while ($current2->next != NULL) {
    if ($current2->type == 'key') {
      $key2 = trim($current2->data->toString(), "'\"");
      $callback($array2, $current2, $hook, $current2->type, $key2, NULL);
    }
    elseif ($current2->type == 'value') {
      $value2 = trim($current2->data->toString(), "'\"");
      $callback($array2, $current2, $hook, $current2->type, $key2, $value2);
      $key2 = ''; // Clear key in case not all elements have keys.
    }
    $current2 = $current2->next;
  }
}

/**
 * Updates hook_action_info() arrays.
 *
 * The arrays returned by hook_action_info(), hook_action_info_alter(),
 * actions_list(), actions_get_all_actions(), actions_actions_map() were
 * changed.
 *
 * @todo The other functions listed above.
 * @todo Database table {actions} - 'description' field is now called 'label'.
 *
 * @param PGPArray $array2
 *   The array object containing the array element ($current2).
 * @param PGPNode $current2
 *   The node object of the array element.
 * @param string $hook
 *   The hook name.
 * @param string $type
 *   The type (key or value) of the array element.
 * @param string $key
 *   The key of the array element (or the most recent key).
 * @param string $value
 *   The value of the array element (or NULL if element is a key).
 */
function coder_upgrade_callback_action_info(&$array2, &$current2, $hook, $type, $key, $value) { // DONE (NEEDS WORK on other functions listed above)
  cdp("inside " . __FUNCTION__);

  if (!is_a($current2, 'PGPNode')) {
    clp("ERROR: current2 is not a PGPNode object in hook_$hook");
    return;
  }

  $editor = PGPEditor::getInstance();

  // The keys of this array are the action function items.
  if ($type == 'key' && $key == 'description') {
    cdp("Found the description key");
    if (!$current2->data->isType(T_CONSTANT_ENCAPSED_STRING)) {
      clp("ERROR: key expression is not a string in hook_$hook");
      return;
    }
    // Change key from 'description' to 'label.'
    $current2->data = $editor->expressionToStatement("'label'");
  }
  elseif ($type == 'key' && $key == 'hooks') {
    cdp("Found the hooks key");
    if (!$current2->data->isType(T_CONSTANT_ENCAPSED_STRING)) {
      clp("ERROR: key expression is not a string in hook_$hook");
      return;
    }
    // Change key from 'hooks' to 'triggers.'
    $current2->data = $editor->expressionToStatement("'triggers'");

    // Find the value element for this key.
    if (!$array2->findNextValue($current2)) {
      clp("ERROR: did not find a value expression for the triggers key in hook_$hook");
      return;
    }

    if (!$current2->data->isType(T_ARRAY)) {
      clp("ERROR: value expression is not a PGPArray object in hook_$hook");
      return;
    }

    /*
     * The triggers (formerly hooks) element is an associative array keyed by
     * module name with its value being an array of actions. The new format
     * combines the module name and action, eliminating one nested array.
     * Example:
     * 'hooks' => array(
     *   'comment' => array('insert', 'update'),
     *   "taxonomy" => array("insert", "update", "delete", "view"),
     * )
     *
     * 'triggers' => array(
     *   'comment_insert', 'comment_update',
     *   'taxonomy_insert', 'taxonomy_update', 'taxonomy_delete', 'taxonomy_view'
     * )
     */
    $triggers = &$current2->data->getElement();

    $ops = array();

    $trigger = $triggers->values->first();
    while ($trigger->next != NULL) {
      if ($trigger->type == 'key') {
        // This is the module (or content type) the action pertains to.
        $prefix = trim($trigger->data->toString(), "'\"");
      }
      elseif ($trigger->type == 'value') {
        if (!$trigger->data->isType(T_ARRAY)) {
          clp("ERROR: trigger value expression is not a PGPArray object in hook_$hook");
          return;
        }

        // This is the array of action operation items.
        $operation_items = $trigger->data->getElement();
        $string = $operation_items->values->toString();
        $items = explode(', ', $string);
        foreach ($items as $op) {
          $ops[] = "'{$prefix}_" . trim($op, "'\"") . "'";
        }
      }
      $trigger = &$trigger->next;
    }
    $string = 'array(' . implode(', ', $ops) . ')';
    $editor = PGPEditor::getInstance();
    $expression = $editor->expressionToStatement($string);
    // TODO Use multiline format? Depending on number of items?
    $expression->getElement()->multiline = 1;
    $expression->getElement()->preserve = 0;
    // Replace the triggers nested array with new array.
    $triggers = $expression;
  }
}

/**
 * Updates hook_hook_info() arrays.
 *
 * hook_hook_info() is now called hook_trigger_info(), and its return value has
 * been changed and simplified.
 *
 * @todo Database table {trigger_assignments} - 'op' field was removed, and the
 * 'hook' field now contains the full function name.
 *
 * @param PGPArray $array2
 *   The array object containing the array element ($current2).
 * @param PGPNode $current2
 *   The node object of the array element.
 * @param string $hook
 *   The hook name.
 * @param string $type
 *   The type (key or value) of the array element.
 * @param string $key
 *   The key of the array element (or the most recent key).
 * @param string $value
 *   The value of the array element (or NULL if element is a key).
 */
function coder_upgrade_callback_hook_info(&$array2, &$current2, $hook, $type, $key, $value) { // DONE
  cdp("inside " . __FUNCTION__);

  if (!is_a($current2, 'PGPNode')) {
    clp("ERROR: current2 is not a PGPNode object in hook_$hook");
    return;
  }

  $editor = PGPEditor::getInstance();

  /*
   * Array levels
   * - L0 return array(
   * - L1   'node' => array(
   * - L2     'nodeapi' => array(
   * - L3       'presave' => array(
   * - L4         'runs when' => t('When either saving a new post or updating an existing post'),
   */

  // The keys of this array are the module (or content type) the hook pertains to.
  if ($type == 'key') {
    cdp("Found the module (or content type) key");
    if (!$current2->data->isType(T_CONSTANT_ENCAPSED_STRING)) {
      clp("ERROR: key expression is not a string in hook_$hook");
      return;
    }

    // This is the module (or content type) the hook pertains to.
    // TODO Other prefixes may need to be modified.
    switch ($key) {
      case 'nodeapi':
        $prefix = 'node';
        break;
      case 'taxonomy':
        $prefix = 'taxonomy_term';
        break;
      default:
        $prefix = $key;
        break;
    }

    // Find the value element for this key.
    if (!$array2->findNextValue($current2)) {
      clp("ERROR: did not find a value expression for the triggers key in hook_$hook");
      return;
    }

    if (!$current2->data->isType(T_ARRAY)) {
      clp("ERROR: value expression is not a PGPArray object in hook_$hook");
      return;
    }

    /*
     * The triggers information element is an associative array doubly keyed by
     * module name with its value being an array of action information. The new
     * format combines the module name and action, eliminating one nested array.
     * Example:
     * 'node' => array(
     *   'nodeapi' => array(
     *     'presave' => array(
     *       'runs when' => t('When either saving a new post or updating an existing post'),
     *     ),
     *   ),
     * ),
     *
     * 'node' => array(
     *   'node_presave' => array(
     *     'label' => t('When either saving a new post or updating an existing post'),
     *   ),
     * ),
     */
    $triggers = &$current2->data->getElement();

    $trigger = $triggers->values->first();
    while ($trigger->next != NULL) {
      if ($trigger->type == 'key') {
        if (!$trigger->data->isType(T_CONSTANT_ENCAPSED_STRING)) {
          clp("ERROR: trigger key expression is not a string in hook_$hook");
          return;
        }
        // This is the module (or content type) the hook pertains to.
        // Concatenate prefix with key.
        $suffix = trim($trigger->data->toString(), "'\"");
        $new_key = "'{$prefix}_$suffix'";
        $trigger->data = $editor->expressionToStatement($new_key);
      }
      elseif ($trigger->type == 'value') {
        if (!$trigger->data->isType(T_ARRAY)) {
          clp("ERROR: trigger value expression is not a PGPArray object in hook_$hook");
          return;
        }

        // This is the array of trigger information items.
        // Currently, the only key is 'label' (formerly 'runs when').
        $information_items = &$trigger->data->getElement();
        $information_items->multiline = 1;
        $information_items->preserve = 0;

        if (!$information_items->changeKey('runs when', "'label'")) {
          clp("ERROR: could not change 'runs when' key expression in hook_$hook");
          clp($information_items->toString());
          return;
        }
      }
      $trigger = &$trigger->next;
    }
    // Move up level 3 (triggers info) to level 2 (redundant module name) of the array.
    $array2->values = $triggers->values; // $array2->values = $array3->values;
    /*
     * Set array properties so the rewritten array:
     * - has a comma after all values
     * - is not indented one level too many
     */
    $array2->count = $triggers->count;
    $array2->commaCount = $triggers->count;
    $array2->multiline = 1;
    $array2->preserve = 0;
  }
}

/**
 * Updates hook_node_info() arrays.
 *
 * In hook_node_info() change 'module' back to 'base' and change 'node' to
 * 'node_content'.
 *
 * @todo A similar change applies to the array passed as the parameter to
 * node_type_set_defaults($example_node_type).
 *
 * @param PGPArray $array2
 *   The array object containing the array element ($current2).
 * @param PGPNode $current2
 *   The node object of the array element.
 * @param string $hook
 *   The hook name.
 * @param string $type
 *   The type (key or value) of the array element.
 * @param string $key
 *   The key of the array element (or the most recent key).
 * @param string $value
 *   The value of the array element (or NULL if element is a key).
 */
function coder_upgrade_callback_node_info(&$array2, &$current2, $hook, $type, $key, $value) { // DONE
  cdp("inside " . __FUNCTION__);

  if (!is_a($current2, 'PGPNode')) {
    clp("ERROR: current2 is not a PGPNode object in hook_$hook");
    return;
  }

  $editor = PGPEditor::getInstance();

  // The keys of this array are the node type items.
  if ($type == 'key' && $key == 'module') {
    cdp("Found the module key");
    if (!$current2->data->isType(T_CONSTANT_ENCAPSED_STRING)) {
      clp("ERROR: key expression is not a string in hook_$hook");
      return;
    }
    // Change key from 'module' to 'base.'
    $current2->data = $editor->expressionToStatement("'base'");

    // Find the value element for this key.
    if (!$array2->findNextValue($current2)) {
      clp("ERROR: did not find a value expression for the base key in hook_$hook");
      return;
    }

    if (!$current2->data->isType(T_CONSTANT_ENCAPSED_STRING)) {
      clp("ERROR: value expression is not a string in hook_$hook");
      return;
    }
    if (trim($current2->data->toString(), "'\"") == 'node') {
      // Change value from 'node' to 'node_content.'
      $current2->data = $editor->expressionToStatement("'node_content'");
    }
  }
}

/**
 * Updates hook_theme() arrays.
 *
 * Each theme function must register how it integrates with drupal_render().
 *
 * @param PGPArray $array2
 *   The array object containing the array element ($current2).
 * @param PGPNode $current2
 *   The node object of the array element.
 * @param string $hook
 *   The hook name.
 * @param string $type
 *   The type (key or value) of the array element.
 * @param string $key
 *   The key of the array element (or the most recent key).
 * @param string $value
 *   The value of the array element (or NULL if element is a key).
 */
function coder_upgrade_callback_theme(&$array2, &$current2, $hook, $type, $key, $value) { // DONE
  cdp("inside " . __FUNCTION__);

  if (!is_a($current2, 'PGPNode')) {
    clp("ERROR: current2 is not a PGPNode object in hook_$hook");
    return;
  }

  $editor = PGPEditor::getInstance();

  // The keys of this array are the theme items.
  if ($type == 'key' && $key == 'arguments') {
    cdp("Found the arguments key");
    if (!$current2->data->isType(T_CONSTANT_ENCAPSED_STRING)) {
      clp("ERROR: key expression is not a string in hook_$hook");
      return;
    }
    // Save the key expression for possible modification.
    $key_expression = &$current2->data;

    // Find the value element for this key.
    if (!$array2->findNextValue($current2)) {
      clp("ERROR: did not find a value expression for the arguments key in hook_$hook");
      return;
    }

    if (!$current2->data->isType(T_ARRAY)) {
      clp("ERROR: value expression is not a PGPArray object in hook_$hook");
      return;
    }

    $array = $current2->data->getElement();
    $key0 = trim($array->getKey()->toString(), "'\"");
    // At a minimum, an array with key-value pairs will have 4 keys after one
    // pair: lparens, key, assign, and value.
    // What other likely values are there for a render element?
    // Does $form ever appear with other variables? If so, how is this to be handled?
    // TODO This is not a deterministic way of setting the key.
    if ($array->count == 1 && in_array($key0, array('form', 'element', 'elements'))) { // if ($array->values->count() == 4 && in_array($key0, array('form', 'element'))) {
      // Change key from 'arguments' to 'render element.'
      $key_expression = $editor->expressionToStatement("'render element'");
    }
    else {
      // Change key from 'arguments' to 'variables.'
      $key_expression = $editor->expressionToStatement("'variables'");
    }
  }
}
