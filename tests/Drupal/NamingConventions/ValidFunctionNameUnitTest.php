<?php

namespace Drupal\Test\NamingConventions;

use Drupal\Test\CoderSniffUnitTest;

class ValidFunctionNameUnitTest extends CoderSniffUnitTest
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
        case 'ValidFunctionNameUnitTest.inc':
            return [
                3 => 1,
                4 => 1,
                8 => 1,
            ];
        case 'valid_function_name_test.module':
            return [24 => 1];
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
        return [];

    }//end getWarningList()


    /**
     * Returns a list of test files that should be checked.
     *
     * @param string $testFileBase The base path that the unit tests files will have.
     *
     * @return array<string>
     */
    protected function getTestFiles($testFileBase): array
    {
        $testFiles[] = __DIR__.'/valid_function_name_test.module';
        $testFiles[] = __DIR__.'/ValidFunctionNameUnitTest.inc';

        return $testFiles;

    }//end getTestFiles()


}//end class
