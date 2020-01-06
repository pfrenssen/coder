<?php
/**
 * Unit test class for all good files that must not throw errors/warnings.
 */

namespace Drupal\Test\good;

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
    protected function getErrorList(string $testFile)
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
    protected function getWarningList(string $testFile)
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
        $di = new \DirectoryIterator(__DIR__);

        foreach ($di as $file) {
            $path = $file->getPathname();
            if ($path !== __FILE__ && $file->isFile() === true) {
                $testFiles[] = $path;
            }
        }

        $testFiles[] = __DIR__.'/drupal8/LongNamespace.php';

        // Get them in order.
        sort($testFiles);
        return $testFiles;

    }//end getTestFiles()


    /**
     * Returns a list of sniff codes that should be checked in this test.
     *
     * @return array The list of sniff codes.
     */
    protected function allSniffCodes()
    {
        // We want to test all sniffs defined in the standard.
        return true;

    }//end allSniffCodes()


}//end class
