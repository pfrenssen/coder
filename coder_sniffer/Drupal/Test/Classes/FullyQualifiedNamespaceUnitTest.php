<?php

namespace Drupal\Sniffs\Classes;

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
     * @return array(int => int)
     */
    public function getErrorList($testFile=null)
    {
        switch ($testFile) {
        case 'FullyQualifiedNamespaceUnitTest.inc':
            return [3 => 1];
        case 'FullyQualifiedNamespaceUnitTest.api.php':
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
            __DIR__.'/FullyQualifiedNamespaceUnitTest.inc',
            __DIR__.'/FullyQualifiedNamespaceUnitTest.api.php',
        ];

    }//end getTestFiles()


}//end class
