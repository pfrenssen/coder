<?php

namespace Drupal\Test\Arrays;

use Drupal\Test\CoderSniffUnitTest;

class ArrayUnitTest extends CoderSniffUnitTest
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
        switch ($testFile) {
        case 'ArrayUnitTest.inc':
            return [
                13 => 1,
                33 => 1,
                83 => 1,
                88 => 1,
                92 => 1,
            ];
        case 'ArrayUnitTest.1.inc':
            return [
                14 => 1,
                17 => 1,
            ];
        }

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
        switch ($testFile) {
        case 'ArrayUnitTest.inc':
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
        }

        return [];

    }//end getWarningList()


}//end class
