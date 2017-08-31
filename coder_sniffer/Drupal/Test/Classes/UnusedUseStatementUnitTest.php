<?php

namespace Drupal\Sniffs\Classes;

use Drupal\Test\CoderSniffUnitTest;

class UnusedUseStatementUnitTest extends CoderSniffUnitTest
{


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
        return array(
                5 => 1,
                6 => 1,
                7 => 1,
                10 => 1,
                11 => 1,
                12 => 1,
                14 => 1,
                16 => 1,
                17 => 1,
                19 => 1,
                20 => 1,
               );

    }//end getWarningList()


}//end class
