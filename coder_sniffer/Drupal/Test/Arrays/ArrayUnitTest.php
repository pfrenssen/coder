<?php

namespace Drupal\Sniffs\Arrays;

use Drupal\Test\CoderSniffUnitTest;

class ArrayUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    public function getErrorList()
    {
        return [
            13 => 1,
            33 => 1,
            83 => 1,
            88 => 1,
            92 => 1,
        ];

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @return array(int => int)
     */
    public function getWarningList()
    {
        return [
            17 => 1,
            22 => 1,
            23 => 1,
            24 => 1,
            37 => 1,
            42 => 1,
            44 => 1,
            59 => 1,
            76 => 1,
        ];

    }//end getWarningList()


}//end class
