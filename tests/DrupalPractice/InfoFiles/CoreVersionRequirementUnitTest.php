<?php

namespace DrupalPractice\Test\InfoFiles;

use Drupal\Test\CoderSniffUnitTest;

class CoreVersionRequirementUnitTest extends CoderSniffUnitTest
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
        switch ($testFile) {
        case 'core_version.info.yml':
            return [1 => 1];
        case 'core_version_test.info.yml':
            return [];
        }

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
        return [
            __DIR__.'/core_version.info.yml',
            __DIR__.'/core_version_test.info.yml',
            __DIR__.'/core_version.config.info.yml',
        ];

    }//end getTestFiles()


}//end class
