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

$getMapOnlyLatestRevision = function ($tableName, $baseField, $idField, $preValidIds = []) use ($db): ?array {
  if (empty($tableName) || empty($baseField) || empty($idField)) {
    return NULL;
  }
  $ids = [];
  $map = [];
  if (empty($preValidIds)) {
    $ids = $db->query("SELECT MAX($idField) FROM $tableName GROUP BY $baseField")->fetchCol();
  }
  else {
    $ids = $db->query("SELECT MAX($idField) FROM $tableName WHERE $idField IN (:ids[]) GROUP BY $baseField", [
      ':ids[]' => $preValidIds,
    ])->fetchCol();
  }
  if (empty($ids)) {
    return NULL;
  }
  $map = $db->query("SELECT * FROM $tableName WHERE $idField IN (:ids[])", [
    ':ids[]' => $ids,
  ])->fetchAllAssoc($idField);
  return $map;
};
