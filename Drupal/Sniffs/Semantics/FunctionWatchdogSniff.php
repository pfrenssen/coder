<?php
/**
 * Drupal_Sniffs_Semanitcs_FunctionWatchdogSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that the second argument to watchdog() is not enclosed with t().
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Semantics_FunctionWatchdogSniff extends Drupal_Sniffs_Semantics_FunctionCall
{


    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array
     */
    public function registerFunctionNames()
    {
        return array('watchdog');

    }//end registerFunctionNames()


    /**
     * Processes this test, when one of its function names is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token in
     *                                        the stack passed in $tokens.
     *
     * @return void
     */
    public function processFunctionCall(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();
        // Get the second argument passed to watchdog().
        $argument = $this->getArgument(2);
        if ($argument === false) {
            $error = 'The second argument to watchdog() is missing';
            $phpcsFile->addError($error, $stackPtr, 'WatchdogArgument');
            return;
        }

        if ($tokens[$argument['start']]['code'] === T_STRING
            && $tokens[$argument['start']]['content'] === 't'
        ) {
            $error = 'The second argument to watchdog() should not be enclosed with t()';
            $phpcsFile->addError($error, $argument['start'], 'WatchdogT');
        }

    }//end processFunctionCall()


}//end class

?>
