<?php
TEST: Blank lines with whitespace

--INPUT--
// Change:
if ($foo) {
  bar();
  
  baz();
}

// No change:
if ($foo) {
  bar();

  baz();
}

--EXPECT--
// Change:
if ($foo) {
  bar();

  baz();
}

// No change:
if ($foo) {
  bar();

  baz();
}
