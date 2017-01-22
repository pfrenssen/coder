<?php

namespace MyNamespace\Depth;

use Foo\Bar;
use Bar\Fail;
use Test\Bar\Thing;
use Thing\Fail\ActuallyUsed;
use Test\TraitTest;
use Thing\NotUsed;
use Test\AnotherTrait;
use Thing\SomeName as OtherName;
use Thing\DifferentName as UsedOtherName;
use Another\UnusedUse;
use Example\MyUrlHelper;
use MyNamespace\Depth\UnusedSameNamespace;
use /* I like weird comment placements */ MyNamespace\Depth\AnotherUnusedSameNamespace /* Oh yes I do */;
use MyNamespace\Depth\SomeClass as CoreSomeClass;
use Some\Data\VarName;
use Some\Data\VarName2 as AliasVarName2;

/**
 * Bla.
 */
class Pum {
  use TraitTest;
  use Test\AnotherTrait;

  /**
   * This data type should be fixed to be fully qualified.
   *
   * @var VarName
   */
  protected $x;

  /**
   * Aliased type that is otherwise unused.
   *
   * @var AliasVarName2
   */
  protected $y;

  /**
   * Description.
   */
  protected function test(ActuallyUsed $x, UsedOtherName $y) {

  }

  /**
   * Description.
   */
  protected function test2(\Thing\NotUsed $x) {

  }

  /**
   * PHP is not case sensitive.
   */
  protected function test3(MyURLHelper $x) {

  }

  /**
   * Don't need to use classes in the same namespace.
   */
  protected function test4(UnusedSameNamespace $x, AnotherUnusedSameNamespace $y) {

  }

  /**
   * Renamed class from same namespace.
   */
  protected function test5(CoreSomeClass $x) {

  }

  /**
   * Inline var declarations should also get fixed to the full namespace.
   */
  protected function test6($x) {
    /** @var VarName $y */
    $y = $x['test'];
    /** @var AliasVarName2 $z */
    $z = $x['test2'];
    return $y;
  }

  /**
   * Return declarations should also get fixed to the full namespace.
   *
   * @param array $x
   *   Some array.
   *
   * @return VarName
   *   The variable.
   */
  protected function test7($x) {
    return $x['test'];
  }

}
