<?php

class Test {

  /**
   * Private constructor in case class can be instantiated.
   */
  private function __construct() {

  }

  /**
   * Method is not called from anywhere in the class.
   */
  private function bong() {

  }

  /**
   * Method is called from somewhere in the class.
   */
  private function isUsed() {

  }

  /**
   * Call method with different upper/lower case.
   */
  public function doCalls() {
    $this->IsUsEd();
  }

}
