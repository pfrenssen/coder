<?php

namespace DrupalPractice\Test\Variables;

use Drupal\Test\CoderSniffUnitTest;

class GetRequestDataUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getErrorList(string $testFile)
    {
        return [
            3  => 1,
            4  => 1,
            5  => 1,
            6  => 1,
            7  => 1,
            9  => 1,
            10 => 1,
            11 => 1,
            12 => 1,
            13 => 1,
        ];

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getWarningList(string $testFile)
    {
        return [];

    }//end getWarningList()


}//end class
