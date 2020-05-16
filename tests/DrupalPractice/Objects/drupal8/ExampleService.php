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

  /**
   * Global function should not be used, we should use an injected service.
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
