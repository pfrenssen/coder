<?php
/**
 * Unit test class for all good files that must not throw errors/warnings.
 */

namespace Drupal\good;

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
     * @return array(int => int)
     */
    public function getErrorList()
    {
        return [];

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
        $dir = dirname(__FILE__);
        $di  = new \DirectoryIterator($dir);

        foreach ($di as $file) {
            $path = $file->getPathname();
            if ($path !== __FILE__ && $file->isFile() === true) {
                $testFiles[] = $path;
            }
        }

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
