<?php

namespace DrupalPractice\Test\General;

use Drupal\Test\CoderSniffUnitTest;

/**
 * Unit test class for the OptionsT sniff.
 *
 * A sniff unit test checks a .inc file for expected violations of a single
 * coding standard. Expected errors and warnings are stored in this class.
 */
class OptionsTUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getErrorList(string $testFile)
    {
        return [];

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getWarningList(string $testFile)
    {
        return [14 => 1];

    }//end getWarningList()


}//end class
