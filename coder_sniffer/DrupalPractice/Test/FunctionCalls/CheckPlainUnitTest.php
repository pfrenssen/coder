<?php
/**
 * Unit test class for the CheckPlain sniff.
 */

namespace DrupalPractice\Sniffs\FunctionCalls;

use Drupal\Test\CoderSniffUnitTest;

/**
 * Unit test class for the CheckPlain sniff.
 *
 * A sniff unit test checks a .inc file for expected violations of a single
 * coding standard. Expected errors and warnings are stored in this class.
 */
class CheckPlainUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getErrorList()
    {
        return [];

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getWarningList()
    {
        return [3 => 1];

    }//end getWarningList()


}//end class
