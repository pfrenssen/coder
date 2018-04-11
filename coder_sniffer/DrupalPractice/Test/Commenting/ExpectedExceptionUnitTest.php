<?php

namespace DrupalPractice\Sniffs\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class ExpectedExceptionUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getErrorList($testFile = NULL)
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
    protected function getWarningList($testFile = NULL)
    {
        return array(
                8 => 1,
                9 => 1,
                10 => 1,
                11 => 1,0
               );

    }//end getWarningList()


}//end class
