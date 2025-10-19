<?php
/**
 * Unit test class for all bad files.
 */

namespace DrupalPractice\Test\bad;

use Drupal\Test\CoderSniffUnitTest;

/**
 * Unit test class for all bad files.
 */
class BadUnitTest extends CoderSniffUnitTest
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
        case 'VariableAnalysisUnitTest.inc':
            return [
                4  => 1,
                8  => 1,
                17 => 1,
                61 => 1,
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
        $di        = new \DirectoryIterator(__DIR__);
        $testFiles = [];

        foreach ($di as $file) {
            $path = $file->getPathname();
            if ($path !== __FILE__ && $file->isFile() === true && preg_match('/\.fixed$/', $path) !== 1) {
                $testFiles[] = $path;
            }
        }

        // Get them in order.
        sort($testFiles);
        return $testFiles;

    }//end getTestFiles()


    /**
     * False if just the current sniff should be checked, true if all sniffs should be checked.
     *
     * @return bool
     */
    protected function checkAllSniffCodes()
    {
        // We want to test all sniffs defined in the standard.
        return true;

    }//end checkAllSniffCodes()


}//end class
