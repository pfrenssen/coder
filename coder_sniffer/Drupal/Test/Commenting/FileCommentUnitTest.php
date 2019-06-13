<?php

namespace Drupal\Sniffs\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class FileCommentUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array(int => int)
     */
    public function getErrorList($testFile=null)
    {
        switch ($testFile) {
        case 'FileCommentUnitTest.inc':
            return [1 => 1];
        case 'FileCommentUnitTest.1.inc':
            return [3 => 1];
        case 'FileCommentUnitTest.2.inc':
            return [4 => 1];
        case 'FileCommentUnitTest.3.inc':
            return [4 => 1];
        case 'FileCommentUnitTest.4.inc':
            return [3 => 1];
        case 'FileCommentUnitTest.5.inc':
            return [1 => 1];
        case 'FileCommentUnitTest.6.inc':
            return [3 => 1];
        case 'FileCommentUnitTest.7.inc':
            return [];
        case 'FileCommentUnitTest.8.inc':
            return [3 => 1];
        case 'FileCommentUnitTest.9.inc':
            return [3 => 1];
        case 'FileCommentUnitTest.10.inc':
            return [4 => 1];
        case 'FileCommentUnitTest.11.inc':
            return [4 => 1];
        case 'FileCommentUnitTest.12.inc':
            return [2 => 1];
        }//end switch

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


}//end class
