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
   * @var Boolean|NULL|FALSE|TRUE|[]
   */
  protected $someVar;

  /**
   * A variable name should not follow the type declaration.
   *
   * @var \Drupal\Core\Session\AccountProxyInterface $user
   */
  protected $user;

  // A doc comment should be used here.
  protected $wrongCommentStyle;

  // Wrong style with var keyword.
  var $varStyle;

  protected $missingComment;

  /**
   * A variable comment is optional for a typed property.
   */
  public ?Bar $bar;

  /**
   * Property fooBar.
   *
   * @var Foo|Bar|false|null
   */
  public Foo|Bar|FALSE|NULL $fooBar;

  /**
   * Readonly property.
   */
  protected readonly string $readOnly;

  /**
   * PHPStan array annotation.
   *
   * @var array<int,array<int,int>>
   */
  protected array $arrayStructure;

  /**
   * PHPStan array shapes.
   *
   * @var array{'foo': int, "bar"?: string}
   */
  protected array $arrayShape;

  /**
   * PHPStan constant wildcard.
   *
   * @var Foo::*
   */
  protected string $constant;

  /**
   * Intersection type support, it is allowed to repeat it in the var type.
   *
   * @var \Drupal\user\UserStorageInterface&\PHPUnit\Framework\MockObject\MockObject
   */
  protected UserStorageInterface&MockObject $userStorageMock;

  /**
   * It is also allowed to leave the var comment out since there is a type.
   */
  protected UserStorageInterface&MockObject $userStorageMock2;

}
