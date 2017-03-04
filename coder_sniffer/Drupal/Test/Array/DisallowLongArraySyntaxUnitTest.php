<?php

class Drupal_Sniffs_Array_DisallowLongArraySyntaxUnitTest extends CoderSniffUnitTest
{

    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    public function getErrorList($testFile)
    {
        switch ($testFile) {
            case 'DisallowLongArraySyntaxUnitTest.2.inc':
                return array(12 => 1);

            default:
                return array();
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
    public function getWarningList($testFile)
    {
        return array();

    }//end getWarningList()


    /**
     * Returns a list of test files that should be checked.
     *
     * @return array The list of test files.
     */
    protected function getTestFiles() {
        return array(
                __DIR__ . '/disallow_long_array_d7/DisallowLongArraySyntaxUnitTest.1.inc',
                __DIR__ . '/disallow_long_array_d8/DisallowLongArraySyntaxUnitTest.2.inc',
               );
    }


}//end class
