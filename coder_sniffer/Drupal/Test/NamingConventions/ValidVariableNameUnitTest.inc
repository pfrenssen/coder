<?php

$UppperCamelName = 0;
$lowerCamelName = 1;

class TestAnnotationProperties extends Plugin {

  /**
   * The entity_type.
   *
   * @var string
   */
  public $entity_type;

}

class TestAnnotationProperties2 extends ViewsPluginAnnotationBase {

  /**
   * The entity_type.
   *
   * @var string
   */
  public $entity_type;

}

class TestAnnotationProperties3 implements AnnotationInterface {

  /**
   * The entity_type.
   *
   * @var string
   */
  public $entity_type;

}
