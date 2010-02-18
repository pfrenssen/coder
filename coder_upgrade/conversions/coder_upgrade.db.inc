<?php
// $Id$

/**
 * @file
 * Database function call conversion routine file for the coder_upgrade module.
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
 * Database
 * http://drupal.org/node/224333#dbtng (NOT FINISHED)
 */

/**
 * Implements hook_upgrade_call_db_query_alter().
 *
 * @param PGPFunctionCall $item
 */
function coder_upgrade_upgrade_call_db_query_alter(&$item, &$reader) {
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  /*
   * Use cases
   * - query is a string: parse directly
   * - query is a variable; find the variable and see if it is a string (assigned only once or always from strings, no expressions)
   * - query is an expression (leave this alone)
   */

  // Process function call.
  $name = &$item->name; // DONE (NEEDS WORK)
  $p0 = $item->getParameter(0);
  if ($p0->count() == 1) {
    $operand0 = $p0->getElement();
    if (is_array($operand0) && $operand0['type'] == T_CONSTANT_ENCAPSED_STRING) {
      // Convert values to strings.
      // Parse the string.
      coder_upgrade_parse_query_string($item, $operand0, $reader);
    }
  }
}

/**
 * Implements hook_upgrade_call_db_query_alter().
 *
 * @param PGPFunctionCall $item
 */
function coder_upgrade_parse_query_string(&$item, &$operand, &$reader) {
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // TODO Parse the values - the other parameters
  // TODO The parameter could be an array variable or expression
  // See http://drupalcode.org/viewvc/drupal/drupal/modules/simpletest/tests/database_test.test?revision=1.80&view=markup
  $values = array();
  for ($i = 1; $i < $item->parameters->count(); $i++) {
    $values[] = $item->printParameter($i);
  }
  cdp($values);

  /*
   * TODO Run the table names through a conversion routine.
   * Ex: taxonomy, block, etc.
   */
  $new = array();
  $sql = trim($operand['value'], "'\"");
  if (strpos($sql, 'INSERT') === 0) {
    // INSERT INTO {mytable_longer_name} (intvar, stringvar, floatvar) VALUES (%d, '%s', %f)
    $find = '@INSERT INTO\s+{(\w+)}\s+\((.*?)\)\s+VALUES\s+\((.*?)\)@';
    preg_match($find, $sql, $matches);
    cdp($matches);
    $fields = explode(', ', $matches[2]);
    cdp($fields);
    // TODO Values could be literals not placeholders.
    $new[] = "\$id = db_insert('{$matches[1]}')";
    $new[] = "\t->fields(array(";
    foreach ($fields as $field) {
      $value = array_shift($values);
      $new[] = "\t\t'$field' => $value,";
    }
    $new[] = "\t))";
    $new[] = "\t->execute();";
  }
  elseif (strpos($sql, 'UPDATE') === 0) {
    // UPDATE {node} SET title='%s', status=%d WHERE uid=%d
    $find = '@^UPDATE\s+{(\w+)}\s+SET\s+(.*?)\s+WHERE\s+(.*?)$@m';
    // TODO Check for WHERE and use a different regex.
    preg_match($find, $sql, $matches);
    cdp($matches);
    $fields = explode(', ', $matches[2]);
    cdp($fields);
    // TODO Values could be literals not placeholders.
    $new[] = "db_update('{$matches[1]}')";
    $new[] = "\t->fields(array(";
    foreach ($fields as $field) {
      list($field,) = explode("=", $field);
      // Trim the field for whitespace.
      $field = trim($field);
      $value = array_shift($values);
      $new[] = "\t\t'$field' => $value,";
    }
    $new[] = "\t))";
    if ($matches[3]) {
      $conditions = $matches[3]; // Explode around AND, OR
      // TODO Parse the WHERE clause which may have more complex expressions.
      list($condition,) = explode("=", $conditions);
      // Trim the condition for whitespace.
      $condition = trim($condition);
      $value = array_shift($values);
      $new[] = "\t->condition('$condition', $value)";
    }
    $new[] = "\t->execute();";
  }
  elseif (strpos($sql, 'DELETE') === 0) {
    // DELETE FROM {node} WHERE uid=%d AND created < %d
    $find = '@^DELETE\s+FROM\s+{(\w+)}\s+WHERE\s+(.*?)$@m';
    // TODO Check for WHERE and use a different regex.
    preg_match($find, $sql, $matches);
    cdp($matches);
    $new[] = "db_delete('{$matches[1]}')";
    // TODO Values could be literals not placeholders.
    $conditions = explode(' AND ', $matches[2]); // Explode around AND, OR
    cdp($conditions);
    foreach ($conditions as $condition) {
      // TODO Parse the WHERE clause which may have more complex expressions.
      list($condition,) = explode("=", $condition);
      // Trim the condition for whitespace.
      $condition = trim($condition);
      $value = array_shift($values);
      // TODO The comparison operator is the third parameter.
      $new[] = "\t\t->condition('$condition', $value)";
    }
    $new[] = "\t->execute();";
  }

  cdp($new);
  $new = implode("\n", $new);
  $new = str_replace("\t", '  ', $new);
  cdp($new);

  // Get the parent = statement (i.e. node) this function call is part of.
  $parent = &$item->parent;
  // Get the statement list the parent is part of.
  $container = &$parent->container;
  // Insert a statement.
  $statement = $editor->textToStatements($new)->getElement(0);
  $container->insertAfter($parent, $statement, 'function_call');
}

/**
 * Implements hook_upgrade_call_db_rewrite_sql_alter().
 *
 * @param PGPFunctionCall $item
 */
function coder_upgrade_upgrade_call_db_rewrite_sql_alter(&$item, &$reader) {
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  /*
   * Use cases
   * - query is a string: parse directly
   * - query is a variable; find the variable and see if it is a string
   * - query is an expression
   */

  // Process function call.
  $name = &$item->name; // NOT DONE - Copied from db_query above
  $p0 = $item->getParameter(0);
  if ($p0->count() == 1) {
    $operand0 = $p0->getElement();
    if (is_array($operand0) && $operand0['type'] == T_CONSTANT_ENCAPSED_STRING) {
      // Convert values to strings.
      // Parse the string.
      coder_upgrade_parse_query_string($item, $operand0, $reader);
      // Add $query->addTag('node_access' /* TODO Please indicate the appropriate tag */);
    }
  }
}
