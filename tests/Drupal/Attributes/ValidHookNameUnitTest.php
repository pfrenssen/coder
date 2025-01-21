<?php

namespace Drupal\Test\Attributes;

use Drupal\Test\CoderSniffUnitTest;

class ValidHookNameUnitTest extends CoderSniffUnitTest
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
        return [];

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
        return [
            19  => 1,
            27  => 1,
            36  => 1,
            44  => 1,
            52  => 1,
            62  => 1,
            101 => 1,
            116 => 1,
        ];

    }//end getWarningList()


}//end class
