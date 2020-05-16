<?php

namespace Drupal\Test\Classes;

use Drupal\Test\CoderSniffUnitTest;

class ClassFileNameUnitTest extends CoderSniffUnitTest
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
        case 'ClassFileNameUnitTest.php':
            return [3 => 1];
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
        return [
            __DIR__.'/drupal8/ClassFileNameUnitTest.php',
            __DIR__.'/drupal8/drupal8.behat.inc',
            __DIR__.'/drupal7/class_fle_name_test.module',
            __DIR__.'/drupal8/markdownFile.md',
        ];

    }//end getTestFiles()


}//end class
