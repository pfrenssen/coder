<?php
TEST: Arithmetic operators

--INPUT--
// No change:
function foo() {
  $foo = $bar ? -1 : 0;
  $foo = (-1 + 1);
  return ($a_weight < $b_weight) ? -1 : 1;
}

function file_download() {
  if (in_array(-1, $headers)) {
    return drupal_access_denied();
  }
}

--EXPECT--
// No change:
function foo() {
  $foo = $bar ? -1 : 0;
  $foo = (-1 + 1);
  return ($a_weight < $b_weight) ? -1 : 1;
}

function file_download() {
  if (in_array(-1, $headers)) {
    return drupal_access_denied();
  }
}

