<?php

namespace DrupalPractice\Test\CodeAnalysis;

use Drupal\Test\CoderSniffUnitTest;

/**
 * Unit test class for the VariableAnalysis sniff.
 *
 * A sniff unit test checks a .inc file for expected violations of a single
 * coding standard. Expected errors and warnings are stored in this class.
 */
class VariableAnalysisUnitTest extends CoderSniffUnitTest
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
            4  => 1,
            8  => 1,
            17 => 1,
        ];

    }//end getWarningList()

    /**
     * False if just the current sniff should be checked, true if all sniffs should be checked.
     *
     * @return bool
     */
    protected function checkAllSniffCodes()
    {
        // We want to test all sniffs defined in the standard as we're looking to include
        // vendor/sirbrillig/phpcs-variable-analysis.
        return true;

    }//end checkAllSniffCodes()

}//end class
