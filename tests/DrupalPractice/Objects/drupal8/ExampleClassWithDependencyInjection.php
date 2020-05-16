<?php

namespace Drupal\testmodule;

use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Drupal\node\Entity\Node;

/**
 * A class which is not a service but has access to the container.
 */
class ExampleClassWithDependencyInjection implements ContainerInjectionInterface {

  /**
   * Using \Drupal here is not allowed, it should be injected instead.
   */
  public function test() {
    return \Drupal::configFactory();
  }

  /**
   * Loading nodes directly is not allowed, we should use an injected service.
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
