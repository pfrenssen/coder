<?php

namespace DrupalPractice\Sniffs\Yaml;

use Drupal\Test\CoderSniffUnitTest;

class RoutingAccessUnitTest extends CoderSniffUnitTest
{

    /**
     * Returns a list of test files that should be checked.
     *
     * @return array The list of test files.
     */
    protected function getTestFiles($testFileBase) {
        return [__DIR__.'/routing_access_test.routing.yml'];
    }


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getErrorList($testFile = NULL)
    {
        return array();

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getWarningList($testFile = NULL)
    {
        return array(
                7 => 1,
                28 => 1,
               );

    }//end getWarningList()


}//end class
