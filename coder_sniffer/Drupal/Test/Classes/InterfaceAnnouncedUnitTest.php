<?php

namespace Drupal\Test\Classes;

use Drupal\Test\CoderSniffUnitTest;

class InterfaceAnnouncedUnitTest extends CoderSniffUnitTest
{


    /**
     * Sets up this unit test.
     *
     * @return void
     */
    protected function setUp()
    {
        parent::setUp();
        include_once __DIR__.'/InterfaceAnnouncedUnitTest.inc';

    }//end setUp()


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
        if ($testFile === 'InterfaceAnnouncedUnitTest.inc') {
            return [
                36 => 1,
                43 => 2,
                50 => 1,
            ];
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
            __DIR__.'/InterfaceAnnouncedUnitTest.inc',
        ];

    }//end getTestFiles()


}//end class
