<?php
/**
 * Unit test class for all good files that must not throw errors/warnings.
 */

namespace DrupalPractice\Test\good;

use Drupal\Test\CoderSniffUnitTest;

/**
 * Unit test class for all good files that must not throw errors/warnings.
 */
class GoodUnitTest extends CoderSniffUnitTest
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
        return [__DIR__.'/good.php'];

    }//end getTestFiles()


    /**
     * False if just the current sniff should be checked, false if all sniffs should be checked.
     *
     * @return bool
     */
    protected function checkAllSniffCodes()
    {
        // We want to test all sniffs defined in the standard.
        return true;

    }//end checkAllSniffCodes()


}//end class
