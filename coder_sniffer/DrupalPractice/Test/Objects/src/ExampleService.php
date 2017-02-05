<?php

namespace Drupal\testmodule;

use Drupal\node\Entity\Node;

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

  /**
   * Loading nodes should be done from an injected service.
   */
  public function test2() {
    return Node::load(1);
  }

}
