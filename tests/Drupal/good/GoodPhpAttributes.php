<?php

/**
 * Test PHP attributes.
 */
class GoodPhpAttributes {

  /**
   * Bar property.
   */
  #[NotBlank]
  private bool $bar;

  /**
   * Tests method with PHP attribute and docblock.
   */
  #[\ReturnTypeWillChange]
  public function attributes(): void {
  }

}
