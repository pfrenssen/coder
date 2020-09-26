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
        $errorList = array_fill_keys(range(133, 151), 1);
        $errorList[27] = 1;
        $errorList[32] = 1;
        $errorList[37] = 1;
        $errorList[42] = 1;
        $errorList[47] = 1;
        $errorList[52] = 1;
        $errorList[57] = 1;
        $errorList[62] = 1;
        $errorList[67] = 1;
        $errorList[72] = 1;
        $errorList[77] = 1;
        $errorList[82] = 1;
        $errorList[87] = 1;
        $errorList[92] = 1;
        $errorList[97] = 1;
        $errorList[102] = 1;
        $errorList[107] = 1;
        $errorList[112] = 1;
        $errorList[117] = 1;
        return $errorList;

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
