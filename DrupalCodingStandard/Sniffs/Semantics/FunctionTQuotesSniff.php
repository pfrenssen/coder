<?php
/**
 * DrupalCodingStandard_Sniffs_Semantics_FunctionTQuotesSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Check the usage of the t() function to not escape translateable string with back
 * slashes.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_Semantics_FunctionTQuotesSniff extends DrupalCodingStandard_Sniffs_Semantics_FunctionCall
{


    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array
     */
    public function registerFunctionNames()
    {
        return array('t');

    }//end registerFunctionNames()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token
     *                                        in the stack passed in $tokens.
     *
     * @return void
     */
    public function processFunctionCall(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens   = $phpcsFile->getTokens();
        $argument = $this->getArgument(1);

        if ($tokens[$argument['start']]['code'] !== T_CONSTANT_ENCAPSED_STRING) {
            // Not a translatable string literal.
            return;
        }

        $string = $tokens[$argument['start']]['content'];
        // Check if there is a backslash escaped single quote in the string and
        // if the string makes use of double quotes.
        if ($string{0} === "'" && strpos($string, "\'") !== false
            && strpos($string, '"') === false
        ) {
            $warn = 'Avoid backslash escaping in translatable strings when possible, use "" quotes instead';
            $phpcsFile->addWarning($warn, $argument['start'], 'BackslashSingleQuote');
            return;
        }

        if ($string{0} === '"' && strpos($string, '\"') !== false
            && strpos($string, "'") === false
        ) {
            $warn = "Avoid backslash escaping in translatable strings when possible, use '' quotes instead";
            $phpcsFile->addWarning($warn, $argument['start'], 'BackslashDoubleQuote');
        }

    }//end processFunctionCall()


}//end class

?>
