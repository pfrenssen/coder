<?php

namespace Drupal\Sniffs\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class DocCommentUnitTest extends CoderSniffUnitTest
{

    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    public function getErrorList($testFile = NULL)
    {
        switch ($testFile) {
            case 'DocCommentUnitTest.inc':
                return array(
                    8 => 1,
                    12 => 1,
                    14 => 1,
                    16 => 1,
                    29 => 1,
                    36 => 2,
                    45 => 1,
                    66 => 1,
                    100 => 4,
                    101 => 1,
                );
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
    public function getWarningList($testFile = NULL)
    {
        return array();

    }//end getWarningList()


}//end class
