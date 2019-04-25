<?php

/**
 * Test class with strict config schema checking disabled.
 */
class TestFalse {

  /**
   * Disable strict config checking during testing.
   */
  protected $strictConfigSchema = FALSE;
}

class TestNull {
  protected $strictConfigSchema = NULL;

  public function strictConfigSchema() {
    // This is ok because it's non-member variable.
    $strictConfigSchema = FALSE;
    return $strictConfigSchema;
  }
}

class TestNoDefault {
  protected $strictConfigSchema;
}

class TestMultipleVars {
  protected $foo, $strictConfigSchema = FALSE, $bar;
}

class NormalClass {

  /**
   * This is OK because it isn't a test class.
   */
  protected $stictConfigSchema = FALSE;

}

trait TestTrait {
  protected $strictConfigSchema;
}
