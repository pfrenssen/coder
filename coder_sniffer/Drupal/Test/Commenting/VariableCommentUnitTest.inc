<?php

/**
 * @file
 * Test file.
 */

/**
 * Test class with properties.
 */
class Test {

  /**
   * The data type declaration should not end with a dot.
   *
   * @var \Drupal\locale\StringStorageInterface.
   */
  protected $localeStorage;

  /**
   * Multiple type declarations can be declared, separated by pipes.
   *
   * @var \Drupal\Core\Entity\EntityInterface|null
   */
  protected $entity;

  /**
   * Test for some common misspellings.
   *
   * @var Boolean|stdClass|NULL|FALSE|TRUE|[]
   */
  protected $someVar;

}
