<?php

namespace DrupalPractice\Sniffs\InfoFiles;

use Drupal\Test\CoderSniffUnitTest;

class NamespacedDependencyUnitTest extends CoderSniffUnitTest
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
     * @param string $testFile The name of the file being tested.
     *
     * @return array(int => int)
     */
    public function getWarningList($testFile=null)
    {
        switch ($testFile) {
        case 'dependencies_test.info.yml':
            return [
                9  => 1,
                11 => 1,
                13 => 1,
            ];
        case 'dependencies_theme.info.yml':
            return [];
        }

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
            __DIR__.'/dependencies_test.info.yml',
            __DIR__.'/dependencies_theme.info.yml',
        ];

    }//end getTestFiles()


}//end class
