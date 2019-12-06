<?php

namespace Drupal\Sniffs\Arrays;

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
     * @return array(int => int)
     */
    public function getErrorList($testFile=null)
    {
        switch ($testFile) {
        case 'ArrayUnitTest.1.inc':
            return [
                13 => 1,
                33 => 1,
                83 => 1,
                88 => 1,
                92 => 1,
            ];
        case 'ArrayUnitTest.2.inc':
            return [
                14 => 1,
                17 => 1,
            ];
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
     * @return array(int => int)
     */
    public function getWarningList($testFile=null)
    {
        switch ($testFile) {
        case 'ArrayUnitTest.1.inc':
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
        case 'ArrayUnitTest.2.inc':
            return [];
        }

    }//end getWarningList()


    /**
     * Returns a list of test files that should be checked.
     *
     * @param string $testFileBase The base path that the unit tests files will have.
     *
     * @return array The list of test files.
     */
    protected function getTestFiles($testFileBase)
    {
        return [
            __DIR__.'/ArrayUnitTest.1.inc',
            __DIR__.'/ArrayUnitTest.2.inc',
        ];

    }//end getTestFiles()


}//end class
