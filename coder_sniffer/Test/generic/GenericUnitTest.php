<?php
/**
 * Unit test class for sniffs inherited from the Generic standard.
 */

/**
 * Unit test class for sniffs inherited from the Generic standard.
 */
class Drupal_GenericUnitTest extends CoderSniffUnitTest
{

    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    public function getErrorList($testFile)
    {
        return array(
                1 => 1,
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
    public function getWarningList($testFile)
    {
        return array();

    }//end getWarningList()

    /**
     * Returns a list of test files that should be checked.
     *
     * @return array The list of test files.
     */
    protected function getTestFiles() {
        return array(dirname(__FILE__) . '/GenericUnitTest.inc');
    }

    /**
     * Returns a list of sniff codes that should be checked in this test.
     *
     * @return array The list of sniff codes.
     */
    protected function getSniffCodes() {
        $ruleset = simplexml_load_file(dirname(__FILE__) . '/../../Drupal/ruleset.xml');
        $codes = array();

        // We only want referenced rules from the "Generic" standard.
        foreach($ruleset->rule as $rule) {
            $name = (string) $rule['ref'];
            if (strpos($name, 'Generic') === 0) {
                $codes[] = $name;
            }
        }
        return $codes;
    }


}//end class

?>
