<?php

function test() {
  list($version) = some_function();
}

function test2($text, $data = array(), array $options = array()) {
  $processed_strings =& drupal_static(__FUNCTION__, NULL);
  // Short-circuit the degenerate case, just like token_replace() does.
  $text_tokens = \Drupal::token()->replace($text);
  if (!empty($text_tokens)) {
    return $text_tokens;
  }
}

function test3() {
  global $test;
  $x = 5;
  if ($x == 5) {
    $x = 123;
  }
  return $x;
}

function test4() {
  return new Foo(function () {
    $metatags = [];
    foreach ([1, 2] as $u) {
      $metatags[] = $u;
    }
    return $metatags;
  });
}
