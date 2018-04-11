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
     * @return array(int => int)
     */
    public function getErrorList($testFile = NULL)
    {
        switch ($testFile) {
            case 'FileCommentUnitTest.inc':
                return array(
                        1 => 1,
                       );
            case 'FileCommentUnitTest.1.inc':
                return array(
                        3 => 1,
                       );
            case 'FileCommentUnitTest.2.inc':
                return array(
                        4 => 1,
                       );
            case 'FileCommentUnitTest.3.inc':
                return array(
                        4 => 1,
                       );
            case 'FileCommentUnitTest.4.inc':
                return array(
                        3 => 1,
                       );
            case 'FileCommentUnitTest.5.inc':
                return array(
                        1 => 1,
                       );
            case 'FileCommentUnitTest.6.inc':
                return array(
                    3 => 1,
                );
            case 'FileCommentUnitTest.7.inc':
                return array();
            case 'FileCommentUnitTest.8.inc':
                return array(
                    3 => 1,
                );
            case 'FileCommentUnitTest.9.inc':
                return array(
                    3 => 1,
                );
            case 'FileCommentUnitTest.10.inc':
                return array(
                    4 => 1,
                );
            case 'FileCommentUnitTest.11.inc':
                return array(
                    4 => 1,
                );
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
