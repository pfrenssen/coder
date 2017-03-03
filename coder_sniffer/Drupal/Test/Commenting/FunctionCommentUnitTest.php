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
        switch ($testFile) {
            case 'FunctionCommentUnitTest.inc':
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
                        298 => 1,
                        308 => 1,
                        311 => 1,
                        321 => 1,
                        324 => 1,
                        334 => 1,
                        345 => 1,
                        357 => 1,
                        360 => 1,
                        371 => 1,
                        389 => 2,
                        390 => 2,
                        401 => 1,
                        414 => 1,
                        416 => 1,
                        426 => 2,
                        427 => 2,
                       );
            case 'FunctionCommentUnitTest.1.inc':
                return array();

            case 'FunctionCommentUnitTest.2.inc':
                return array(8 => 1);
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


}//end class
