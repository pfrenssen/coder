<?php
TEST: Comment movement

--INPUT--
// Change:
$foo = foo(); // Move up.
if ($foo) {
  bar($foo); // Move up.
}

// Don't change:
$string = '//';
$foo    = 'http://google.com';
$string = array(
  'foo', '//',
);

// Comment // comment.

/**
 * This does stuff with FOO//BOO.
 */
function foo() {
  return 'boo';
}

--EXPECT--
// Change:
// Move up.
$foo = foo();
if ($foo) {
  // Move up.
  bar($foo);
}

// Don't change:
$string = '//';
$foo    = 'http://google.com';
$string = array(
  'foo', '//',
);

// Comment // comment.

/**
 * This does stuff with FOO//BOO.
 */
function foo() {
  return 'boo';
}
