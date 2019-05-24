<?php

namespace Drupal\testmodule;

use Drupal\node\Entity\Node;

/**
 * A class which is not a service.
 */
class ExampleClass {

  /**
   * Using \Drupal here is allowed since this class cannot access the container.
   */
  public function test() {
    return \Drupal::configFactory();
  }

  /**
   * Loading nodes directly is allowed since we cannot access the container.
   */
  public function test2() {
    return Node::load(1);
  }

  /**
   * Global function is allowed since we cannot access the container.
   */
  public function test3() {
    return format_date(time());
  }

  /**
   * t() should not be used, instead we should use the StringTranslationTrait.
   */
  public function test4() {
    return t('Test');
  }

}
