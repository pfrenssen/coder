<?php

/**
 * Test.
 */
function missing_trailing_comma(
  $a,
  $b
) {

}

$foo = 1;
$bar = 2;
$x = function(
  $a,
  $b
) use (
  $foo,
  $bar
) {

};
