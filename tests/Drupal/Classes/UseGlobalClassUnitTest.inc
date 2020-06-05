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
use function count;

/**
 * Example.
 */
class Example extends Test implements Test, Alias, MultiLineAlias {

  use Test;

  /**
   * Description.
   */
  public function test1(Alias $alias, TestClass $class) {
    count([]);
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

  /**
   * Description.
   */
  public function test6() {
    $test = new MultiLineAlias2();
  }

  /**
   * Description.
   */
  public function test7(): ?Test {

  }

  /**
   * Description.
   */
  abstract public function test8(): Test;

  /**
   * Description.
   */
  abstract public function test9(): ?Test;

}
