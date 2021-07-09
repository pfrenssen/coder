<?php
/**
 * Helper class to be compatible with different PHPUnit major versions.
 *
 * We want to support PHP 7.0 which does not have the "void" return type hint.
 * For PHP 8 we need a higher PHPUnit version that enforces the type hint. Our
 * solution is to define a compatibility class depending on PHP version. It
 * routes around the setup method with a compatibleSetUp() method that child
 * classes use.
 */

namespace Drupal\Test;

use PHPUnit\Framework\TestCase;

if (version_compare(PHP_VERSION, '7.1.0') >= 0) {
    class PhpunitCompatibilityTestCase extends TestCase
    {


        /**
         * {@inheritdoc}
         *
         * @return void
         */
        protected function setUp(): void
        {
            $this->compatibleSetUp();

        }//end setUp()


        /**
         * Helper method that is used in place of ::setUp().
         *
         * @return void
         */
        protected function compatibleSetUp()
        {

        }//end compatibleSetUp()


    }//end class

} else {
    class PhpunitCompatibilityTestCase extends TestCase
    {


        /**
         * {@inheritdoc}
         *
         * @return void
         */
        protected function setUp()
        {
            $this->compatibleSetUp();

        }//end setUp()


        /**
         * Helper method that is used in place of ::setUp().
         *
         * @return void
         */
        protected function compatibleSetUp()
        {

        }//end compatibleSetUp()


    }//end class

}//end if
