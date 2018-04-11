<?php

namespace Drupal\Sniffs\Formatting;

use Drupal\Test\CoderSniffUnitTest;

class SpaceUnaryOperatorUnitTest extends CoderSniffUnitTest
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
        return array(
                3 => 1,
                4 => 1,
                5 => 1,
                6 => 1,
                7 => 1,
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
    public function getWarningList($testFile = NULL)
    {
        return array();

    }//end getWarningList()


}//end class
