<?php

class Drupal_Sniffs_Formatting_MultipleStatementAlignmentUnitTest extends CoderSniffUnitTest
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
        return array(
                8 => 1,
                10 => 1,
                11 => 1,
                13 => 1,
                14 => 1,
                16 => 1,
                17 => 1,
                19 => 1,
                20 => 1,
                30 => 1,
               );

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


}//end class
