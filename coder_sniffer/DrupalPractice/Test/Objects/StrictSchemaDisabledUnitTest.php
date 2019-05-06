<?php

namespace DrupalPractice\Sniffs\Objects;

use Drupal\Test\CoderSniffUnitTest;

class StrictSchemaDisabledUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getErrorList()
    {
        return [
            11 => 1,
            15 => 1,
            25 => 1,
            29 => 1,
            42 => 1,
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
    protected function getWarningList()
    {
        return [];

    }//end getWarningList()


}//end class
