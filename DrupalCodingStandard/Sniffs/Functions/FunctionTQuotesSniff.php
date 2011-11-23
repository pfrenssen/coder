<?php
/**
 * DrupalCodingStandard_Sniffs_Functions_FunctionTQuotesSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @author   Klaus Purer <klaus.purer@example.com>
 * @license  http://www.gnu.org/licenses/gpl-2.0.html GPLv2
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * DrupalCodingStandard_Sniffs_Functions_FunctionTQuotesSniff.
 *
 * Check the usage of the t() function to not escape translateable string with back
 * slashes.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @author   Klaus Purer <klaus.purer@example.com>
 * @license  http://www.gnu.org/licenses/gpl-2.0.html GPLv2
 * @version  Release: 1.2.0RC3
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_Functions_FunctionTQuotesSniff implements PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_STRING);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token
     *                                        in the stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();
        if ($tokens[$stackPtr]['content'] === 't') {
            // Find the next non-empty token.
            $openBracket = $phpcsFile->findNext(PHP_CodeSniffer_Tokens::$emptyTokens, ($stackPtr + 1), null, true);

            if ($tokens[$openBracket]['code'] !== T_OPEN_PARENTHESIS) {
                // Not a function call.
                return;
            }

            // Find the next non-empty token, the first argument to t().
            $stringArg = $phpcsFile->findNext(PHP_CodeSniffer_Tokens::$emptyTokens, ($openBracket + 1), null, true);
            if ($tokens[$stringArg]['code'] !== T_CONSTANT_ENCAPSED_STRING) {
                // Not a translatable string literal.
                return;
            }

            if ($tokens[$stringArg]['content']{0} === "'" && strpos($tokens[$stringArg]['content'], "\'") !== false) {
                $warn = 'Avoid backslash escaping in translatable strings when possible, use "" quotes instead';
                $phpcsFile->addWarning($warn, $stringArg);
                return;
            }

            if ($tokens[$stringArg]['content']{0} === '"' && strpos($tokens[$stringArg]['content'], '\"') !== false) {
                $warn = "Avoid backslash escaping in translatable strings when possible, use '' quotes instead";
                $phpcsFile->addWarning($warn, $stringArg);
            }
        }//end if

    }//end process()


}//end class

?>
