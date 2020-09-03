<?php

namespace Drupal\Test\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class InlineVariableCommentUnitTest extends CoderSniffUnitTest
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
            11 => 1,
            12 => 1,
            13 => 1,
            15 => 1,
            16 => 1,
            17 => 1,
            18 => 1,
        ];

    }//end getWarningList()


}//end class
