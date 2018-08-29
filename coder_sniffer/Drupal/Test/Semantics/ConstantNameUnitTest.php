<?php

namespace Drupal\Sniffs\Semantics;

use Drupal\Test\CoderSniffUnitTest;

class ConstantNameUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns a list of test files that should be checked.
     *
     * @return array The list of test files.
     */
  protected function getTestFiles($testFileBase)
    {
        $dir = dirname(__FILE__);
        return array($dir.'/constant_test.install', $dir.'/constant_test.module');

    }//end getTestFiles()


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    public function getErrorList($testFile = NULL)
    {
        return array();

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @return array(int => int)
     */
    public function getWarningList($testFile = NULL)
    {
        return array(3 => 1);

    }//end getWarningList()


}//end class
