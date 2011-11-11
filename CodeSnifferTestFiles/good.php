<?php
//  $Id: token.inc,v 1.9 2010/03/12 14:33:02 dries Exp $

/**
 * @file
 *   This file contains all the valid notations for the drupal coding standard
 *   Target is to create a style checker that validates all of this constructs
 *   The // $Id: above is a valid CVS tag.
 */

// Singleline comment

/**
 * Multiline comment
 */

// PHP Constants must be written in CAPITAL lettres
TRUE;
FALSE;
NULL;

// Operators - have a space before and after
$i = 0;
$i += 0;
$i -= 0;
$i == 0;
$i != 0;
$i > 0;
$i < 0;
$i >= 0;
$i <= 0;

// Unary operators must not have a space
$i--;
--$i;
$i++;
++$i;

// Casting has a space
(int) $i;

// The last item in an multiline array should be followed by a comma
// But not in a inline array
$a = array();
$a = array('1', '2', '3');
$a = array(
  '1',
  '2',
  '3',
);
$a = array('1', '2', array('3'));
$a = array('1', '2', array(
  '3'),
);

// Item assignment operators must be prefixed and followed by a space
$a = array('one' => '1', 'two' => '2');
foreach ( $a as $key => $value) {
}

// If conditions have a space before and after the condition parenthesis
if (TRUE || TRUE) {
  $i;
}
elseif (TRUE && TRUE) {
  $i;
}
else {
  $i;
}

while (1 == 1) {
  if (TRUE || TRUE) {
    $i;
  }
  else {
    $i;
  }
}

switch ($condition) {
  case 1:
    $i;
    break;

  case 2:
    $i;
    break;

  default:
    $i;
}

do {
  $i;
} while ($condition);

/**
 * Short description
 *
 * We use doxygen style comments.
 * What's sad because eclipse PDT and
 * PEAR CodeSniffer base on phpDoc comment style.
 * Makes working with drupal not easier :|
 *
 * @param $field1
 *  Doxygen style comments
 * @param $field2
 *  Doxygen style comments
 * @param $field3
 *  Doxygen style comments
 * @return
 *  Doxygen style comments
 */
function foo_bar($field1, $field2, $field3 = NULL) {
  $system["description"] = t("This module inserts funny text into posts randomly.");
  return $system[$field];
}

// Function call
$var = foo($i, $i, $i);

// Multiline function call
$var = foo(
  $i,
  $i,
  $i
);

// Multiline function call with array
$var = foo(array(
    $i,
    $i,
    $i,
  ),
  $i,
  $i
);

// Multiline function call with only one array
$var = foo(array(
    $i,
    $i,
    $i,
));

/**
 * Class declaration
 *
 * Classes always have a multiline comment
 */
class Bar {
  // Private properties have no prefix
  private $secret = 1;

  // Public properties also don't a prefix
  protected $foo = 1;

  // Public properties don't have a prefix
  public $bar = 1;

  // Public static variables use camelCase, too.
  public static $basePath = NULL;

  /**
   * Enter description here ...
   */
  public function foo() {

  }

  /**
   * Enter description here ...
   */
  protected function barMethod() {

  }

  /**
   * Enter description here ...
   */
  private function _foobar() {

  }
}

/**
 * Enter description here ...
 */
function _private_foo() {

}

// When calling class constructors with no arguments, always include parentheses
$bar = new Bar();

$bar = new Bar($arg1, $arg2);

$bar = 'Bar';
$foo = new $bar();
$foo = new $bar($i, $i);

// Static class variables use camelCase.
Bar::$basePath = '/foo';

// Concatenation - there has to be a space
$i . "test";
$i . 'test';
$i . $i;
$i . NULL;

// It is allowed to have the closing "}" on the same line if the class is empty.
class MyException extends Exception {}
