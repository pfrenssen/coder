<?php

namespace Drupal\testmodule;

/**
 * Some service.
 */
class ExampleService {

  /**
   * Using \Drupal here but it should be injected instead.
   */
  public function test() {
    return \Drupal::configFactory();
  }

}
