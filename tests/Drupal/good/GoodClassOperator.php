<?php

/**
 * Test class.
 */
class GoodClassOperator {

  /**
   * Seen IDs.
   *
   * @var array
   */
  protected static $seenIds;

  /**
   * Test method.
   */
  public function test() {
    $id = $id . '--' . ++static::$seenIds[$id];
    return $id;
  }

}
