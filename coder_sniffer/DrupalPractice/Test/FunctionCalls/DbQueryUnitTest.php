<?php
/**
 * Unit test class for the DbQuery sniff.
 */

namespace DrupalPractice\Test\FunctionCalls;

use Drupal\Test\CoderSniffUnitTest;

/**
 * Unit test class for the DbQuery sniff.
 *
 * A sniff unit test checks a .inc file for expected violations of a single
 * coding standard. Expected errors and warnings are stored in this class.
 */
class DbQueryUnitTest extends CoderSniffUnitTest
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
        return [
            3  => 1,
            4  => 1,
            5  => 1,
            12 => 1,
            18 => 1,
        ];

    }//end getWarningList()


}//end class
