<?php

namespace Drupal\Test\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class DocCommentStarUnitTest extends CoderSniffUnitTest
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
         switch ($testFile) {
        case 'DocCommentStarUnitTest.inc':
            return [
                9  => 1,
                16 => 1,
            ];
        default:
            return [];
         }

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
