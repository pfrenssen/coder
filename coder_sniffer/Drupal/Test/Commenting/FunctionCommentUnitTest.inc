<?php

/**
 * @file
 * Some function comment tests.
 */

/**
 * Test.
 *
 * @param string $x
 *   Comment does not end with full stop
 * @param string $y
 *   comment does not start with a capital letter.
 */
function foo($x, $y) {

}

/**
 * Missing exception description in the throws tag, which is OK.
 *
 * @throws \Drupal\locale\StringStorageException
 */
function test1() {

}

/**
 * Wrong indentation of description for the throws tag.
 *
 * @throws \Drupal\locale\StringStorageException
 *  Description here.
 */
function test2() {

}

/**
 * Description for the throws tag does not start with capital letter.
 *
 * @throws \Drupal\locale\StringStorageException
 *   description here.
 */
function test3() {

}

/**
 * Description for the throws tag does not end with a dot.
 *
 * @throws \Drupal\locale\StringStorageException
 *   Description here
 */
function test4() {

}

/**
 * Description for the throws tag must be on the next line.
 *
 * @throws \Drupal\locale\StringStorageException Description here.
 */
function test5() {

}

/**
 * Some description.
 *
 * @param string $foo
 *   Some parameter that does not exist.
 */
function test6() {

}

function test7() {
  // Missing function doc here.
}

/**
 * Test methods.
 */
class Test {

  public function undocumented() {

  }

  // This should be a doc comment.
  public function wrong() {

  }

}

/**
 * Some comment.
 *
 * @param int $x Description should be on the next line.
 */
function test8($x) {

}

/**
 * Default implementation of a menu item (without a page or form controller).
 *
 * UML:
 * @link http://drupal7demo.webel.com.au/node/1017 DefaultMenuItem @endlink.
 *
 * @param integer $foo
 *   Test.
 */
function test9($foo) {

}

/**
 * Default implementation of a menu item (without a page or form controller).
 *
 * UML:
 * @link http://drupal7demo.webel.com.au/node/1017 DefaultMenuItem @endlink.
 *
 * @param integer $foo Test.
 */
function test10($foo) {

}

/**
 * A function may throw multiple exceptions.
 *
 * @throws \Exception
 *   Thrown when something bad happens.
 * @throws \InvalidArgumentException
 *   Thrown when an argument is invalid.
 */
function test11() {

}

/**
 * Update or set users expiration time
 *
 * @param object $user
 * @param int $timespan (seconds)
 */
function test12($user, $timespan) {

}

/**
 * Update or set users expiration time.
 *
 * @param object $user
 *   Object type hint in comment without real type hint is allowed.
 */
function test13($user) {

}

/**
 * Update or set users expiration time.
 *
 * @param object $user
 *   Object stdClass type hint is allowed for object.
 */
function test14(stdClass $user) {

}

/**
 * Array parameter type mismatch.
 *
 * @param array $foo
 *   Comment here.
 */
function test15(Test $foo) {

}

/*
 * This should be converted into a doc comment.
 */
function test16() {

}

/**
 * Return comment indentation should be three spaces.
 *
 * @return string
 * This is not indented correctly.
 */
function test17() {
  return '1';
}

/**
 * Return comment indentation should be three spaces.
 *
 * @return string
 *This is not indented correctly.
 */
function test18() {
  return '1';
}

/**
 * Return comment indentation should be three spaces.
 *
 * @return string
 * This is not indented correctly. Continues on the next line because this is a
 * really long sentence.
 */
function test19() {
  return '1';
}

/**
 * Multiple errors in @param line.
 *
 * @param   string    $arg1 This should be in next line and needs period
 */
function test20($arg1) {
}

/**
 * Testing fix of incorrect param types.
 *
 * @param Array $arg1
 *   The parameter type is not valid.
 * @param integer|bool $arg2
 *   One of the parameter types listed is not valid.
 * @param integer|boolean|Array $arg3
 *   All of the parameter types listed are not valid.
 */
function test21($arg1, $arg2, $arg3) {
}

/**
 * Test for allowed param var types.
 *
 * @param array|bool|float|int|mixed|object|string|resource|callable $arg1
 *   All of the above types are valid.
 * @param Array|boolean|integer|str|stdClass|NULL $arg2
 *   All of the above types are invalid.
 * @param array()|Boolean|number|String $arg3
 *   All of the above types are invalid.
 * @param type $arg4
 *   All of the above types are invalid.
 * @param FALSE|TRUE|Int $arg5
 *   All of the above types are invalid.
 * @param Bool|Integer $arg6
 *   All of the above types are invalid.
 */
function test22($arg1, $arg2, $arg3, $arg4, $arg5, $arg6) {
}

/**
 * Void returns allowed when null is given in @return.
 *
 * When null is a potential return value it should be allowed to have return
 * statements with void return as well.
 *
 * @return bool|null
 *   This implies that the return value can be NULL, a boolean, or empty.
 */
function test23() {
  if (1 == 2) {
    return;
  }
  return TRUE;
}

/**
 * Even when null is given in @return there must be at least 1 valid return.
 *
 * When null is a potential return value it should be allowed to have return
 * statements with void return. This does not mean that all returns can be void.
 * There must be at least one non-void return.
 *
 * @return bool|null
 *   This implies that the return value can be NULL, a boolean, or empty.
 */
function test24() {
  if (1 == 2) {
    return;
  }
  return;
}
