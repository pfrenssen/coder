<?php

/**
 * Class declaration.
 *
 * Classes can have a multiline comment.
 */
class GoodClass {

  /**
   * Private properties have no prefix.
   *
   * @var int
   */
  private $secret = 1;

  /**
   * Protected properties also don't have a prefix.
   *
   * @var int
   */
  protected $foo = 1;

  /**
   * Longer properties use camelCase naming.
   *
   * @var int
   */
  public $barProperty = 1;

  /**
   * Using property types is allowed.
   *
   * @var \Foo\Bar
   */
  public ?Bar $bar;

  /**
   * A typed property may omit @var.
   */
  public Bar $baz;

  /**
   * Public static variables use camelCase, too.
   *
   * @var string
   */
  public static $basePath = NULL;

  /**
   * {@inheritdoc}
   */
  protected $modules = ['node', 'user'];

  /**
   * {@inheritDoc}
   */
  protected $allowedModules = ['node', 'user'];

  /**
   * Enter description here ...
   */
  public function foo() {

  }

  /**
   * Enter description here ...
   */
  protected function barMethod() {

  }

  /**
   * Test the ++ and -- operator.
   */
  public function incDecTest() {
    $this->foo++;
    $this->foo--;
    --$this->foo;
    ++$this->foo;
  }

  /**
   * It is allowed to leave out param docs on methods.
   */
  public function noParamDocs($a, $b) {

  }

  /**
   * Param comments with references are found correctly.
   *
   * @param string $a
   *   Parameter one.
   * @param array $b
   *   Parameter two.
   */
  public function test($a, array &$b) {

  }

}
