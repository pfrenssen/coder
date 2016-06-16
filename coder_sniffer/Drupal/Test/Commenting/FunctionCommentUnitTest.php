<?php

class Drupal_Sniffs_Commenting_FunctionCommentUnitTest extends CoderSniffUnitTest
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
                12 => 1,
                14 => 1,
                33 => 1,
                43 => 1,
                53 => 1,
                62 => 1,
                71 => 1,
                78 => 1,
                87 => 1,
                92 => 1,
                101 => 1,
                113 => 1,
                126 => 2,
                147 => 1,
                148 => 2,
                180 => 1,
                187 => 1,
                195 => 1,
                205 => 1,
                215 => 1,
                216 => 1,
                225 => 3,
                233 => 1,
                235 => 1,
                237 => 1,
                248 => 1,
                250 => 1,
                252 => 1,
                254 => 1,
                256 => 1,
                285 => 1,
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
