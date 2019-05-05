<?php

namespace Drupal\Sniffs\Files;

use Drupal\Test\CoderSniffUnitTest;

class EndFileNewlineUnitTest extends CoderSniffUnitTest
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
        // All the good files have no error.
        if (strpos($testFile, 'good') !== false) {
            return [];
        } else {
            // All other files have one error on line one (they have all just one
            // code line in them).
            return [1 => 1];
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
    public function getWarningList()
    {
        return [];

    }//end getWarningList()


}//end class
