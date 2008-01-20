<?php
TEST: Variables

--INPUT--
function db_status_report($phase) {
  $foo = bar();
  $t   = get_t();
  $baz = bay();

  $version = db_version();
}

--INPUT--
class CoderTestFile extends SimpleExpectation {
  private $expected;

  /* Filename of test */
  var $filename;

  protected function describeException($exception) {
    return get_class($exception) .": ". $exception->getMessage();
  }
}

