<?php

/**
 * @file
 * Contains Test.
 */

/**
 * Test class.
 */
class Test extends OtherClass{

}

namespace FirstNamespace{
  /**
   * FirstClass for the test.
   */
  class FirstClass{

  }
}

namespace SecondNamespace{
  /**
   * SecondClass for the test.
   */
  class SecondClass{

  }
}

namespace {
  // Global namespace.
}

/**
 * Testing an empty class body with comment.
 */
class ThirdClass {
  // No-op.
}

/**
 * Class declaration over multiple lines.
 */
class WithManyInterfaces implements
  Interface1,
  Interface2,
  Interface3,
  Interface4 {

}
