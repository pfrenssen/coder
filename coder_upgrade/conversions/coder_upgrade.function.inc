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
 *
 *
 * Menus
 * http://drupal.org/node/224333#hook_menu_link_alter
 *
 *
 * Blocks
 * http://drupal.org/node/224333#remove_op (DUP Node API)
 *
 *
 * Comments
 * http://drupal.org/node/224333#remove_op (DUP Node API) (THIS IS MISSING FROM THE CHRONO PAGE!!!)
 *
 *
 * User API
 * http://drupal.org/node/224333#user_cancel (DONE with remove_op)
 * http://drupal.org/node/224333#remove_op (DUP Node API)
 *
 *
 * Node API
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
  coder_upgrade_convert_action_info($node);
}

/**
 * Implements hook_upgrade_hook_block_alter().
 */
function coder_upgrade_upgrade_hook_block_alter(&$node, &$reader) {
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
 * Implements hook_upgrade_hook_install_alter().
 */
function coder_upgrade_upgrade_hook_install_alter(&$node, &$reader) { }

/**
 * Implements hook_upgrade_hook_uninstall_alter().
 */
function coder_upgrade_upgrade_hook_uninstall_alter(&$node, &$reader) {
  // Changes: install-schema
  coder_upgrade_convert_install($node);
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
  $item->comment = preg_replace('@hook_access([^i])@', "hook_node_access$1", $item->comment);

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
 * Updates hook_action_info().
 *
 * The arrays returned by hook_action_info(), hook_action_info_alter(),
 * actions_list(), actions_get_all_actions(), actions_actions_map() were
 * changed.
 *
 * @todo Database table {actions} - 'description' field is now called 'label'.
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 */
function coder_upgrade_convert_action_info(&$node) { // DONE
  cdp("inside " . __FUNCTION__);
  $item = &$node->data;
  //  cdp($item->print_r());

  // Restructure the actions array.
  $body = &$item->body;

  if (!($return = $body->find(T_RETURN, 'reverse'))) {
    clp("ERROR: return statement not found in hook_action_info");
    return;
  }
  //  cdp("Printing return item");
  $value = &$return->value;
  cdp($value->print_r());
  cdp($value->toString());
  //  cdp("Printing return item DONE");

  // Examine the type of the operand in the return statement.
  $operand = $return->value->getElement();
  if (get_class($operand) == 'PGPArray') {
    // Use case 1 - returns array directly.
    // The keys of this array are the hook function names.
    $array1 = $value->getElement();
    if (!is_a($array1, 'PGPArray')) {
      clp("ERROR: return statement does not include an array of values in hook_action_info");
      return;
    }
    coder_upgrade_callback_action_info($array1);
  }
  elseif (get_class($operand) == 'PGPOperand') {
    // Use case 2 - assigns array to variable, then returns variable.
    // TODO Do callback on body statements until find the assignment to the return variable.
    $variable = $operand->toString();
    if (!($assign = $body->find(T_ASSIGNMENT))) {
      clp("ERROR: assignment statement not found in hook_action_info");
      return;
    }
    $variable2 = $assign->values->getElement()->getElement()->toString();
    if ($variable == $variable2) {
      cdp("Assignment variable matches return variable");
      //      $array1 = $assign->values->getElement()->last();
      $value1 = $assign->values->getElement();
      $array1 = $value1->getElement($value1->count() - 1);
      if (!is_a($array1, 'PGPArray')) {
        clp("ERROR: assignment statement does not include an array of values in hook_action_info");
        return;
      }
      coder_upgrade_callback_action_info($array1);
    }
  }
}

function coder_upgrade_callback_action_info(&$array) { // DONE
  // The keys of this array are the action function names.
  //  $array = $value->getElement();
  if (!is_a($array, 'PGPArray')) {
    clp("ERROR: return statement does not include an array of values in hook_action_info");
    return;
  }

  // This check duplicates above - go with one of them.
  if (get_class($array) == 'PGPArray') {
    // Grab the PGPList of values.
    $actions = $array->values;

    $current = $actions->first();
    while ($current->next != NULL) {
      //      cdp('$current->type = '. $current->type);
      if ($current->type == 'key') {
        //        $key = &$current->data; // This is not the one needed; we need the next level down.
      }
      elseif ($current->type == 'value') {
        // This is the value expression for an action key.
        $value = $current->data;
        //        cdp('class = ' . get_class($value));
        if (get_class($value) != 'PGPExpression') {
          $current = &$current->next;
          continue;
        }
        // This is the array of action items.
        $action_items = $value->getElement();
        //        cdp('class = ' . get_class($action_items));
        if (get_class($action_items) != 'PGPArray') {
          $current = &$current->next;
          continue;
        }
        if (($key = $action_items->findKey("'description'")) !== FALSE) {
          //          cdp("Found the description key");
          // Change key from 'description' to 'label.'
          $element = &$key->getElement(); // $key->setElement();
          $element['value'] = "'label'";
        }
        if (($key = $action_items->findKey("'hooks'")) !== FALSE) {
          //          cdp("Found the hooks key");
          // Change key from 'hooks' to 'triggers.'
          $element = &$key->getElement(); // $key->setElement();
          $element['value'] = "'triggers'";

          $editor = new PGPEditor();
          $list = new PGPList();

          // This should be a PGPArray object.
          $triggers = &$action_items->findValue("'triggers'")->getElement();
          $trigger = $triggers->values->first();
          while ($trigger->next != NULL) {
            if ($trigger->type == 'key') {
              // This is the module (or content type) the action pertains to.
              $prefix = trim($trigger->data->toString(), "'\"");
            }
            elseif ($trigger->type == 'value') {
              // This is the value expression for an action key.
              $value2 = $trigger->data;
              // This is the array of action operation items.
              $operation_items = $value2->getElement();
              $string = $operation_items->values->toString();
              //              print_r($string . "\n");
              $ops = explode(', ', $string);
              foreach ($ops as $id => $op) {
                $ops[$id] = "'{$prefix}_" . trim($op, "'\"") . "'";
              }
              $string = 'array(' . implode(', ', $ops) . ')';
              //              print_r($string . "\n");
              $list->insertLast($editor->expressionToStatement($string), 'value');
              //              cdp($list->toString());
            }
            $trigger = &$trigger->next;
          }
          $triggers = $list;
        }
      }
      $current = &$current->next;
    }
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
    cdp($statement->print_r());
    if (is_a($statement, 'PGPAssignment')) {
      $text = $statement->values->toString();
      cdp($text);
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
  $editor = new PGPEditor();
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
      // This block becomes example_node_view with $build_mode = 'print'
      $hook = '_node_view';
      $parameters = array('$node', '$build_mode = \'print\'');
      break;
    case 'rss item':
      // This block becomes example_node_view with $build_mode = 'rss'
      $hook = '_node_view';
      $parameters = array('$node', '$build_mode = \'rss\'');
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
      // This block becomes example_node_view with $build_mode = 'full' by default
      $parameters = array('$node', '$build_mode = \'full\'');
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
      $parameters = array('$account', '$build_mode');
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
