<?php

namespace Foo\Bar;

use Some\Namespaced\TestClass;
use Some\Namespaced\AnotherTestClass;

/**
 * @coversDefaultClass Some\Namespaced\TestClass
 */
class Test {

  /**
   * Var data types should be fully namespaced.
   *
   * @var Some\Namespaced\TestClass
   */
  protected $test1;

  /**
   * Var data types should be fully namespaced.
   *
   * @var Some\Namespaced\TestClass|string
   */
  protected $test2;

    /**
   * Param, Return and Throws data types should be fully namespaced with a
   * leading backslash, even when the full namespace is given without the
   * leading backslash.
   *
   * @param Some\Namespaced\TestClass $y
   *   Some description.
   * @param Some\Namespaced\AnotherTestClass|null $z
   *   Yet another description.
   *
   * @return Some\Namespaced\TestClass|null
   *   Yep.
   *
   * @throws Some\Namespaced\TestClass
   *
   * @see Some\Namespaced\TestClass
   * @see Some\Namespaced\TestClass::foo()
   */
  public function test3(TestClass $y, AnotherTestClass $z = NULL) {
    if ($z === NULL) {
      return $z;
    }

    return $y;
  }

}
