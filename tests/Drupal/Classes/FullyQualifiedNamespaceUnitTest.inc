<?php

/**
 * @file
 * Example.
 */

use Test\Bar;
use Test\NotUsed;
use Test\Alias as TestAlias;
use Test\MultiLine as MultiLineAlias,
    Test\MultiLineSecond;

/**
 * Example.
 */
class Example {

  /**
   * Description.
   */
  public function test1(TestAlias $alias) {

  }

  /**
   * Description.
   */
  public function test2(Test\NotUsed $notUsed) {

  }

  /**
   * Description.
   */
  public function test3(Test\Alias $alias) {

  }

  /**
   * Description.
   */
  public function test4(Test\Foo $foo) {

  }

  /**
   * Description.
   */
  public function test5(Bar $bar) {

  }

  /**
   * Description.
   */
  public function test6(TestDiff\Bar $bar) {

  }

  /**
   * Description.
   */
  public function test7(TestDiff\TestAlias $anotherAlias) {

  }

  /**
   * Description.
   */
  public function test8(Test\MultiLine $multiLine, Test\MultiLineSecond $multiLineSecond) {

  }

}
