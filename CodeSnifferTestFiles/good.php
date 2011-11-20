<?php

/**
 * @file
 * This file contains all the valid notations for the drupal coding standard.
 *
 * The goal is to create a style checker that validates all of this
 * constructs.
 *
 * Theme files often have lists in their file block:
 * - item 1
 * - item 2
 * - sublist:
 *   - sub item 1
 *   - sub item2
 */

// Singleline comment before a code line.
$foo = 'bar';

/*
 * Multiline comment
 *
 * @see my_function()
 */

// PHP Constants must be written in CAPITAL letters.
TRUE;
FALSE;
NULL;

// Has whitespace at the end of the line.
$whitespaces = 'Yes, Please';

// Operators - have a space before and after.
$i = 0;
$i += 0;
$i -= 0;
$i == 0;
$i != 0;
$i > 0;
$i < 0;
$i >= 0;
$i <= 0;

// Unary operators must not have a space.
$i--;
--$i;
$i++;
++$i;
$i = -1;
$i = +1;
array('i' => -1);
array('i' => +1);
$i = (1 == -1);
$i = (1 === -1);
$i = (1 == +1);
$i = (1 === +1);
range(-50, -45);
$i[0] + 1;
$x->{$i} + 1;

// Casting has a space.
(int) $i;

// The last item in an multiline array should be followed by a comma.
// But not in a inline array.
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

// Pretty array layout.
$a = array(
  'title'    => 1,
  'weight'   => 2,
  'callback' => 3,
);

// Item assignment operators must be prefixed and followed by a space.
$a = array('one' => '1', 'two' => '2');
foreach ($a as $key => $value) {
}

// If conditions have a space before and after the condition parenthesis.
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

// Function call.
$var = foo($i, $i, $i);

// Multiline function call.
$var = foo(
  $i,
  $i,
  $i
);

// Multiline function call with array.
$var = foo(array(
    $i,
    $i,
    $i,
  ),
  $i,
  $i
);

// Multiline function call with only one array.
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
  // Private properties have no prefix.
  private $secret = 1;

  // Public properties also don't a prefix.
  protected $foo = 1;

  // Longer properties use camelCase naming.
  public $barProperty = 1;

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
}

/**
 * Enter description here ...
 */
function _private_foo() {

}

// When calling class constructors with no arguments, always include
// parentheses.
$bar = new Bar();

$bar = new Bar($arg1, $arg2);

$bar = 'Bar';
$foo = new $bar();
$foo = new $bar($i, $i);

// Static class variables use camelCase.
Bar::$basePath = '/foo';

// Concatenation - there has to be a space.
$i . "test";
$i . 'test';
$i . $i;
$i . NULL;

// It is allowed to have the closing "}" on the same line if the class is empty.
class MyException extends Exception {}

// Nice alignment is allowed for assignments.
class MyExampleLog {
  const INFO      = 0;
  const WARNING   = 1;
  const ERROR     = 2;
  const EMERGENCY = 3;

  /**
   * Empty method implementation is allowed.
   */
  public function empty_method() {}

  /**
   * Protected functions are allowed.
   */
  protected function protectedTest() {

  }
}

/**
 * Nice allignment in functions.
 */
function test_test2() {
  $a   = 5;
  $aa  = 6;
  $aaa = 7;
}

// Uses the Rules API as example.
$rule = rule();
$rule->condition('rules_test_condition_true')
     ->condition('rules_test_condition_true')
     ->condition(rules_or()
       // Inline test comment that continues on a
       // second line.
       ->condition(rules_condition('rules_test_condition_true')->negate())
       ->condition('rules_test_condition_false')
       ->condition(rules_and()
         ->condition('rules_test_condition_false')
         ->condition('rules_test_condition_true')
         ->negate()
       )
     );

// Test usages of t().
t('special character: \"');
t("special character: \'");

// Test inline comment style.
// Comment one.
t('x');
// Comment two
// @todo this is valid!
t('x');
// Goes on?
t('x');
/* Longer comment
 * bla bla
 * end!
 */
t('x');
// @see http://example.com
t('x');
// @see my_function()
t('x');
// t() refers to a function name and should not be capitalized.
t('x');
// rules_admin is a fancy machine name word with underscores and should not be
// capitalized and ignored.
t('x');
// {} this comment start should not be flagged.
t('x');

// Template test. Alternative control structure style is allowed.
?>
<div>
<?php if (TRUE): ?>
  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo">
    <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
  </a>
<?php else: ?>
  <i>some text</i>
<?php endif; ?>
</div>
