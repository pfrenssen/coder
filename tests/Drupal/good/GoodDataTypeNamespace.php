<?php

namespace Foo\Bar;

use Some\Namespaced\TestClass;

/**
 * Test.
 */
class GoodDataTypeNamespace {

  /**
   * Param and Return data types can reference use statements.
   *
   * @param TestClass $y
   *   Some description.
   *
   * @return TestClass
   *   Yep.
   *
   * @throws TestClass
   */
  public function test1(TestClass $y) {
    return $y;
  }

  /**
   * Inline var data types are also fine to not be fully namespaced.
   */
  public function test2(array $x) {
    /** @var TestClass $y */
    $y = $x['test'];
    return $y;
  }

}
