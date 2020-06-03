<?php

/**
 * @file
 * Example.
 */

use Test;
use Namespaced\TestClass;
use Namespaced\TestClassSecond as NamespacedAlias;
use Test1 as Alias;
use MultiLine as MultiLineAlias,
    MultiLineSecond;
use Namespaced\MultiLine2 as MultiLineAlias2,
    MultiLineSecond2;

/**
 * Example.
 */
class Example extends Test {

  use Test;

  /**
   * Description.
   */
  public function test1(Alias $alias, TestClass $class) {

  }

  /**
   * Description.
   */
  public function test2(NamespacedAlias $alias): Test {

  }

  /**
   * Description.
   */
  public function test3() {
    try {
    }
    catch (Test $exception) {
    }
  }

  /**
   * Description.
   */
  public function test4() {
    MultiLineAlias::create('test');
  }

  /**
   * Description.
   */
  public function test5() {
    $test = new MultiLineSecond();
  }

}
