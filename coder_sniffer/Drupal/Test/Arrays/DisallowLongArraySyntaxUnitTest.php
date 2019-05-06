<?php

namespace Drupal\Sniffs\Arrays;

use Drupal\Test\CoderSniffUnitTest;

class DisallowLongArraySyntaxUnitTest extends CoderSniffUnitTest
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
        case 'DisallowLongArraySyntaxUnitTest.2.inc':
            return [12 => 1];

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
     * @return array(int => int)
     */
    public function getWarningList()
    {
        return [];

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
            __DIR__.'/disallow_long_array_d7/DisallowLongArraySyntaxUnitTest.1.inc',
            __DIR__.'/disallow_long_array_d8/DisallowLongArraySyntaxUnitTest.2.inc',
        ];

    }//end getTestFiles()


}//end class
