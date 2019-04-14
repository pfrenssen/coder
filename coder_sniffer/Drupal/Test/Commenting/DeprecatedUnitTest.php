<?php

namespace Drupal\Sniffs\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class DeprecatedUnitTest extends CoderSniffUnitTest
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
                24 => 2,  // basic layout is wrong.
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
        return array(
                37 => 2,  // version x 2.
                39 => 1,  // see url.
                47 => 2,  // version x 2.
                49 => 1,  // see url.
               );

    }//end getWarningList()


}//end class
