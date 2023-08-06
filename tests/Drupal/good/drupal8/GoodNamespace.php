<?php

namespace Drupal\mymodule;

/**
 * Some fluffy comment about the class.
 */
class GoodNamespace {

  /**
   * Using \Drupal without a use statement is fine.
   */
  public static function version(): string {
    return \Drupal::VERSION . phpversion() . \phpversion();
  }

}
