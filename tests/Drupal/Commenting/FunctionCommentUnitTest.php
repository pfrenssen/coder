<?php

namespace Drupal\Test\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class FunctionCommentUnitTest extends CoderSniffUnitTest
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
        case 'FunctionCommentUnitTest.inc':
            return [
                13  => 1,
                15  => 1,
                34  => 1,
                44  => 1,
                54  => 1,
                63  => 1,
                72  => 1,
                79  => 1,
                88  => 1,
                93  => 1,
                102 => 1,
                114 => 1,
                127 => 2,
                148 => 1,
                149 => 2,
                188 => 1,
                196 => 1,
                206 => 1,
                216 => 1,
                217 => 1,
                226 => 3,
                234 => 1,
                236 => 1,
                238 => 1,
                249 => 1,
                251 => 1,
                253 => 1,
                255 => 1,
                257 => 1,
                299 => 1,
                309 => 1,
                312 => 1,
                322 => 1,
                325 => 1,
                335 => 1,
                346 => 1,
                358 => 1,
                361 => 1,
                372 => 1,
                390 => 2,
                391 => 2,
                402 => 1,
                415 => 1,
                417 => 1,
                427 => 2,
                428 => 2,
                539 => 1,
                541 => 1,
                942 => 1,
                987 => 1,
            ];
        case 'FunctionCommentUnitTest.1.inc':
            return [];

        case 'FunctionCommentUnitTest.2.inc':
            return [8 => 1];
        }//end switch

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
        return [];

    }//end getWarningList()


}//end class
