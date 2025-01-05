<?php

namespace Drupal\Test\NamingConventions;

use Drupal\Test\CoderSniffUnitTest;

class ValidClassNameUnitTest extends CoderSniffUnitTest
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
    protected function getErrorList(string $testFile): array
    {
        return [
            5  => 2,
            6  => 1,
            7  => 1,
            11 => 2,
            12 => 1,
            13 => 1,
            17 => 2,
            18 => 1,
            19 => 1,
            23 => 2,
            24 => 1,
            25 => 1,
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
    protected function getWarningList(string $testFile): array
    {
        return [];

    }//end getWarningList()


}//end class
