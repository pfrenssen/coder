<?php

class DrupalPractice_Sniffs_Objects_GlobalClassUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getErrorList($testFile)
    {
        return array();

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getWarningList($testFile)
    {
        switch ($testFile) {
            case 'GlobalClassUnitTest.inc':
                return array(8 => 1);
            case 'ExampleService.php':
                return array(23 => 1);
        }

    }//end getWarningList()

    /**
     * Returns a list of test files that should be checked.
     *
     * @return array The list of test files.
     */
    protected function getTestFiles() {
        return [__DIR__.'/GlobalClassUnitTest.inc', __DIR__.'/src/ExampleService.php'];
    }


}//end class
