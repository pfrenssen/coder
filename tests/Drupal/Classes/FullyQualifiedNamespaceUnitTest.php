<?php

namespace Drupal\Test\Classes;

use Drupal\Test\CoderSniffUnitTest;

class FullyQualifiedNamespaceUnitTest extends CoderSniffUnitTest
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
        case 'FullyQualifiedNamespaceUnitTest.inc':
            return [
                29 => 1,
                36 => 1,
                43 => 1,
                57 => 1,
                64 => 1,
                71 => 2,
                78 => 1,
            ];
        case 'FullyQualifiedNamespaceUnitTest.1.inc':
            return [16 => 1];
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
            __DIR__.'/FullyQualifiedNamespaceUnitTest.inc',
            __DIR__.'/FullyQualifiedNamespaceUnitTest.1.inc',
            __DIR__.'/FullyQualifiedNamespaceUnitTest.api.php',
        ];

    }//end getTestFiles()


}//end class
