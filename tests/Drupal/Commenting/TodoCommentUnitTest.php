<?php

namespace Drupal\Test\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class TodoCommentUnitTest extends CoderSniffUnitTest
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
        $warningList = (array_fill_keys(range(16, 34), 1) + array_fill_keys(range(38, 40), 1) + array_fill_keys(range(55, 73), 1) + array_fill_keys(range(75, 77), 1));
        return $warningList;

    }//end getWarningList()


}//end class
