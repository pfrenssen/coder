<?php

namespace Drupal\Sniffs\Semantics;

use Drupal\Test\CoderSniffUnitTest;

class RemoteAddressUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @param string $testFile The filename of the test file.
     *
     * @return array(int => int)
     */
    public function getErrorList($testFile=null)
    {
        return [
            12 => 1,
            19 => 1,
        ];

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @param string $testFile The filename of the test file.
     *
     * @return array(int => int)
     */
    public function getWarningList($testFile=null)
    {
        return [];

    }//end getWarningList()


}//end class
