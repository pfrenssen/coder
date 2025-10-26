<?php

/**
 * All classes need to have a docblock.
 */
class GoodDocBlock implements FooInterface {

  /**
   * {@inheritdoc}
   */
  public function test() {
    /** @var \Drupal\node\NodeInterface $node */
    $node = $this->entity;
    /** @var \Drupal\node\NodeInterface[] $nodes */
    $nodes = foo();
    /** @var \Drupal\node\NodeInterface|\PHPUnit_Framework_MockObject_MockObject $node_mock */
    $node_mock = mock_node();
    /** @var \Drupal\SomeInterface4You $thing */
    $thing = thing();
    /** @var \Drupal\SomeInterface4You $test2 */
    $test2 = test2();
    return $node;
  }

  /**
   * {@inheritdoc}
   *
   * Some additional documentation here.
   */
  public function test2() {}

  /**
   * Return docs are allowed to use $this.
   *
   * @return $this
   *   This object for chaining method calls.
   */
  public function test3() {
    return $this;
  }

  /**
   * Returns the string representatuion of this object.
   */
  public function __toString() {
    return 'foo';
  }

  /**
   * Omitting the comment when returning $this is allowed.
   *
   * @return $this
   */
  public function test4() {
    return $this;
  }

  /**
   * Omitting the comment when returning static is allowed.
   *
   * @return static
   */
  public function test41() {
    return new static();
  }

  /**
   * Loads multiple string objects.
   *
   * @param array $conditions
   *   Any of the conditions used by dbStringSelect().
   * @param array $options
   *   Any of the options used by dbStringSelect().
   * @param string $class
   *   Class name to use for fetching returned objects.
   *
   * @return \Drupal\locale\StringInterface[]
   *   Array of objects of the class requested.
   */
  protected function dbStringLoad(array $conditions, array $options, $class) {
    $strings = [];
    $result = $this->dbStringSelect($conditions, $options)->execute();
    foreach ($result as $item) {
      /** @var \Drupal\locale\StringInterface $string */
      $string = new $class($item);
      $string->setStorage($this);
      $strings[] = $string;
    }
    return $strings;
  }

  /**
   * Short array syntax is allowed.
   */
  public function getConfiguration() {
    return [
      'id' => $this->getPluginId(),
    ] + $this->configuration;
  }

  /**
   * Not documenting a "throws" tag is allowed.
   *
   * This should not fail for errors with underscores in names as well.
   * The second version of this test with error name with underscores
   * is added below.
   *
   * @throws Exception
   */
  public function test6() {
    throw new Exception();
  }

  /**
   * Repeat of above test with error name with underscores.
   *
   * @throws \Twig_Error_Syntax
   */
  public function test7() {
    throw new Exception();
  }

  /**
   * {@inheritDoc}
   */
  public function test8() {}

  /**
   * Orders the result set by a given field.
   *
   * If called multiple times, the query will order by each specified field in
   * the order this method is called.
   *
   * If the query uses DISTINCT or GROUP BY conditions, fields or expressions
   * that are used for the order must be selected to be compatible with some
   * databases like PostgreSQL. The PostgreSQL driver can handle simple cases
   * automatically but it is suggested to explicitly specify them. Additionally,
   * when ordering on an alias, the alias must be added before orderBy() is
   * called.
   *
   * @param string $field
   *   The field on which to order. The field is escaped for security so only
   *   valid field and alias names are possible. To order by an expression, add
   *   the expression with addExpression() first and then use the alias to order
   *   on.
   *
   *   Example:
   *   @code
   *   $query->addExpression('SUBSTRING(thread, 1, (LENGTH(thread) - 1))', 'order_field');
   *   $query->orderBy('order_field', 'ASC');
   *   @endcode
   * @param string $direction
   *   The direction to sort. Legal values are "ASC" and "DESC". Any other value
   *   will be converted to "ASC".
   *
   * @return \Drupal\Core\Database\Query\SelectInterface
   *   The called object.
   */
  public function orderBy($field, $direction = 'ASC') {
    return $this;
  }

  /**
   * Example with multiple code blocks in param docs.
   *
   * @param string $param1
   *   Just some Example param.
   * @param ...
   *   Any additional arguments are passed on to the functions called by
   *   self::submitForm(), including the unique form constructor function.
   *   For example, the node_edit form requires that a node object be passed
   *   in here when it is called. Arguments that need to be passed by reference
   *   should not be included here, but rather placed directly in the
   *   $form_state build info array so that the reference can be preserved. For
   *   example, a form builder function with the following signature:
   *   @code
   *   function mymodule_form($form, FormStateInterface &$form_state, &$object) {
   *   }
   *   @endcode
   *   would be called via self::submitForm() as follows:
   *   @code
   *   $form_state->setValues($my_form_values);
   *   $form_state->addBuildInfo('args', [&$object]);
   *   \Drupal::formBuilder()->submitForm('mymodule_form', $form_state);
   *   @endcode
   */
  public function test9($param1) {}

  /**
   * This is an example of a doc block that is good.
   *
   * We want to show some example code:
   * @code
   *   if ($something) {
   *     $x = $y;
   *   }
   * @endcode
   * Some more example code:
   * @code
   *   if ($something) {
   *     $x = $y;
   *   }
   * @endcode
   * And one more piece of example code:
   * @code
   *   if ($something) {
   *     $x = $y;
   *   }
   * @endcode
   * Followed by some summary text.
   */
  public function test10() {}

  /**
   * This is good.
   *
   * @return string
   *   Here is a comment, let's explain the return value with an example:
   *   @code
   *     if ($something) {
   *       $x = $y;
   *     }
   *   @endcode
   *   And then the comment goes on here. You want more code? Here you go:
   *   @code
   *     if ($something) {
   *       $x = $y;
   *     }
   *   @endcode
   *   And this is the end.
   */
  public function test11() {
    return 'foo';
  }

}
