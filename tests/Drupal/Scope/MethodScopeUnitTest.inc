<?php

trait TestTrait {

  /**
   * Function visibility is missing here.
   */
  function foo() {

  }

  /**
   * Visibility is missing here and should be auto-fixed correctly.
   */
  static function bar() {

  }

}
