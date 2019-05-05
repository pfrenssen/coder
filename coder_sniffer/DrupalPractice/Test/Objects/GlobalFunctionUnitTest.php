<?php

namespace DrupalPractice\Sniffs\Objects;

use Drupal\Test\CoderSniffUnitTest;

class GlobalFunctionUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getErrorList()
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
     * @return array(int => int)
     */
    protected function getWarningList($testFile=null)
    {
        switch ($testFile) {
        case 'GlobalFunctionUnitTest.inc':
            return [6 => 1];
        case 'ExampleService.php':
            return [30 => 1];
        default:
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
            __DIR__.'/GlobalFunctionUnitTest.inc',
            __DIR__.'/src/ExampleService.php',
        ];

    }//end getTestFiles()


}//end class
