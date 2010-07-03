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
 * http://drupal.org/node/224333#db_rewrite_sql (NOT FINISHED)
 */

/**
 * Implements hook_upgrade_call_db_query_alter().
 *
 * @param PGPFunctionCall $item
 */
function coder_upgrade_upgrade_call_db_query_alter(&$item, &$reader) { // NEEDS WORK
  // Disable this routine until it is improved.
//  return;

  // Create helper objects.
//  $editor = PGPEditor::getInstance();

  /*
   * Use cases
   * - query is a string: parse directly
   * - query is a variable; find the variable and see if it is a string (assigned only once or always from strings, no expressions)
   * - query is an expression (leave this alone)
   */

  // Process function call.
  $p0 = $item->getParameter(0);
  if ($p0->count() == 1) {
    $operand0 = $p0->getElement();
    if (is_array($operand0) && $operand0['type'] == T_CONSTANT_ENCAPSED_STRING) {
      // Convert values to strings.
      // Parse the string.
      coder_upgrade_parse_query_string($item, $operand0);
    }
  }
}

/**
 * Implements hook_upgrade_call_db_rewrite_sql_alter().
 *
 * @todo db_rewrite_sql() replaced with hook_query_alter()
 * @see http://drupal.org/node/224333#db_rewrite_sql
 *
 * @param PGPFunctionCall $item
 */
function coder_upgrade_upgrade_call_db_rewrite_sql_alter(&$item, &$reader) {
  // TODO This is not ready as the call to arrayitize() on a SELECT will
  // look at the count of parameters to db_rewrite_sql() which are not the
  // same as count of parameters to db_query().
  return;
  // Create helper objects.
//  $editor = PGPEditor::getInstance();

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
      coder_upgrade_parse_query_string($item, $operand0);
      // Add $query->addTag('node_access' /* TODO Please indicate the appropriate tag */);
    }
  }
}

/**
 * Replaces D6 database API call with D7 equivalent.
 *
 * @todo Fill in this with unhandled items.
 *
 * @param PGPFunctionCall $item
 *   The function call object to be replaced.
 * @param PGPOperand $operand
 *   The query string to be parsed (first parameter to $item).
 */
function coder_upgrade_parse_query_string(&$item, &$operand) {
  // Create helper objects.
  $editor = PGPEditor::getInstance();

  // TODO Parse the values - the other parameters
  // TODO The parameter could be an array variable or expression
  // See http://drupalcode.org/viewvc/drupal/drupal/modules/simpletest/tests/database_test.test?revision=1.80&view=markup
  $values = array();
  for ($i = 1; $i < $item->parameterCount(); $i++) {
    $item->setParameter($i, $item->getParameter($i)->stripComments());
    $values[] = $item->printParameter($i);
  }
  cdp($values, '$values');

  /*
   * TODO Run the table names through a conversion routine.
   * Ex: taxonomy, block, etc.
   */
  $new = array();
  // Trim won't work here, since it'll trim off the last ' if the string ends in '%s'
  $sql = substr($operand['value'], 1, -1);
  if (strpos($sql, 'SELECT') === 0) {
    $find = '@^(SELECT.*?WHERE\s+)(.*?)$@ms';
    preg_match($find, $sql, $matches);
    cdp($matches, '$matches');
    // Convert conditions from field = '%s' to field = :field
    // TODO handle more complex conditions
    if (isset($matches[2]) && $matches[2]) {
      preg_match_all('/
        (\S*)\s*([!=<>]+)\s*[\'"]?%[sdfb][\'"]? # Match field = %d
        |
        (\S*)\s*([!=<>]+)\s*([\'"].*?[\'"]|\S*) # Match field literals
        |
        (\S*)\s+IN\s?\([\'"]%s[\'"]?\) # Match field IN(%s)
      /x', $matches[2], $conditions);
      cdp($conditions, '$conditions');
      $condition_keys = array();
      foreach ($conditions[0] as $key => $condition) {
        if ($conditions[1][$key]) {
          // field = %d condition
          $condition_keys[] = ':'. $conditions[1][$key];
          $operand['value'] = str_replace($condition, $conditions[1][$key] . ' ' . $conditions[2][$key] . ' :' . $conditions[1][$key], $operand['value']);
        }
        else if ($conditions[3][$key]) {
          // field = literal condition
          $condition_keys[] = ':'. $conditions[3][$key];
          $operand['value'] = str_replace($condition, $conditions[3][$key] . ' ' . $conditions[4][$key] . ' :' . $conditions[3][$key], $operand['value']);
          $item->insertParameter($key + 1, $editor->expressionToStatement($conditions[5][$key]));
        }
        else if ($conditions[6][$key]) {
          // filed IN (%s) condition
          $condition_keys[] = ':'. $conditions[6][$key];
          $operand['value'] = str_replace($condition, $conditions[6][$key] . ' IN (:' . $conditions[6][$key] . ')', $operand['value']);
          // D6 convention was to use implode(",", $array).  Now we can just pass in the array.
          $param = $item->getParameter($key + 1)->getElement()->getParameter(1);
          $item->setParameter($key + 1, $param);
        }
      }
      $editor->setParameter($item, 0, $operand['value']);
      $string = $editor->arrayitize($item, 1, $condition_keys, array_fill(0, count($condition_keys), "'XXX_YYY'"));
      cdp($string, '$string');
      $temp = $editor->expressionToStatement($string);
      $temp->getElement(0)->multiline = 0;
      $item->setParameter(1, $temp);
    }
    return;
  }
  elseif (strpos($sql, 'INSERT') === 0) {
    // INSERT INTO {mytable_longer_name} (intvar, stringvar, floatvar) VALUES (%d, '%s', %f)
    $find = '@INSERT INTO\s+{(\w+)}\s+\((.*?)\)\s+VALUES\s+\((.*?)\)@';
    preg_match($find, $sql, $matches);
    cdp($matches);
    $fields = array_combine(explode(', ', $matches[2]) , explode(', ', $matches[3]));

    cdp($fields);
    // TODO Values could be literals not placeholders.
    $new[] = "\$id = db_insert('{$matches[1]}')";
    $new[] = "\t->fields(array(";
    foreach ($fields as $field => $value) {

      // Value is a placeholder
      if (preg_match('/%[sdbf]/', $value)) {
        $value = array_shift($values);
      }
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
    $new[] = "db_update('{$matches[1]}')";
    $new[] = "\t->fields(array(";
    foreach ($fields as $field) {
      list($field,$value) = explode("=", $field);
      // Value is a placeholder
      if (preg_match('/%[sdbf]/', $value)) {
        $value = array_shift($values);
      }
      // Trim the field for whitespace.
      $field = trim($field);
      $new[] = "\t\t'$field' => $value,";
    }
    $new[] = "\t))";
    if ($matches[3]) {
      $new[] = coder_upgrade_parse_sql_conditions($matches[3], $values);
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
    $new[] = coder_upgrade_parse_sql_conditions($matches[2], $values);
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
  $container->insertAfter($parent, $statement, 'function_call'); // TODO Have not used 'function_call' as type
}

/**
 * Parses sql conditions into conditional object strings.
 *
 * @param string $conditions
 * @param array $values
 *
 * @return string
 *   String of conditions in DBTNG syntax.
 */
function coder_upgrade_parse_sql_conditions($conditions, $values) {
  // Check for inner conditionals or function calls.  For now, we'll just use
  // where() instead of conditionals, given the complexity of inner conditionals.
  if (preg_match("/\((.*)\)/", $conditions, $matches)) {
    return "\t\t->where('$conditions', " . var_dump($values) . ")";
  }
  // TODO handle other conjunctions besides AND.
  $split_conditions = preg_split('/AND|OR|XOR/', $conditions);
  if (!is_array($split_conditions)) {
    $split_conditions = array($conditions);
  }

  foreach ($split_conditions as $condition) {
    list($condition, $operator, $value) = preg_split('/([!=><]+|LIKE)/', $condition, -1, PREG_SPLIT_DELIM_CAPTURE);
    if (preg_match('/%[sbdf]/', trim($value))) {
      $value = array_shift($values);
    }
    // Trim the condition for whitespace.
    $condition = trim($condition);
    $value = trim($value);
    // TODO handle IN and BETWEEN conditions
    switch ($operator) {
      case '=':
        $parsed_conditions[] = "\t\t->condition('$condition', $value)";
        break;
      default:
        $parsed_conditions[] = "\t\t->condition('$condition', $value, '$operator')";
        break;
    }
  }
  return implode("\n", $parsed_conditions);
}
