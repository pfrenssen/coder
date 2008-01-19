<?php
TEST: Miscellaneous (split me!)

--INPUT--
// No change:
if ($foo) {
  if ($bar) {
    // Trall!
  }
}

foo(
  $bar
);

--EXPECT--
// No change:
if ($foo) {
  if ($bar) {
    // Trall!
  }
}

foo(
  $bar
);
