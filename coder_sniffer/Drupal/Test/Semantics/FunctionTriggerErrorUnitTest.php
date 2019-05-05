<?php

namespace Drupal\Sniffs\Semantics;

use Drupal\Test\CoderSniffUnitTest;

class FunctionTriggerErrorUnitTest extends CoderSniffUnitTest
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
            30 => 1,
            32 => 1,
            34 => 1,
            36 => 1,
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
            38 => 1,
            40 => 1,
            42 => 1,
            44 => 1,
            46 => 1,
            48 => 1,
            50 => 1,
            52 => 1,
            54 => 1,
            56 => 1,
            58 => 1,
            60 => 1,
            62 => 1,
            64 => 1,
            66 => 1,
        ];

    }//end getWarningList()


}//end class
