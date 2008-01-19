<?php
TEST: Assignments by reference

--INPUT--
// No change:
function &foo() {
  echo 'asdf';
}


--EXPECT--
// No change:
function &foo() {
  echo 'asdf';
}

