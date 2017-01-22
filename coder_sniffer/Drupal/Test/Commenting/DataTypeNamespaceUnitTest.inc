<?php

namespace Foo\Bar;

use Some\Namespaced\TestClass;

/**
 * Test.
 */
class Test {

  /**
   * Param and Return data types should be fully namespaced.
   *
   * @param TestClass $y
   *   Some description.
   *
   * @return TestClass
   *   Yep.
   */
  public function test1(TestClass $y) {
    return $y;
  }

  /**
   * Inline var data types should be converted.
   */
  public function test2(array $x) {
    /** @var TestClass $y */
    $y = $x['test'];
    return $y;
  }

}
