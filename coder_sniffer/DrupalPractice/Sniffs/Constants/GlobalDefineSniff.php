<?php
/**
 * DrupalPractice_Sniffs_Constants_GlobalDefineSniff
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that global define() constants are not used in modules in Drupal 8.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalPractice_Sniffs_Constants_GlobalDefineSniff extends Drupal_Sniffs_Semantics_FunctionCall
{


    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array
     */
    public function registerFunctionNames()
    {
        return array('define');

    }//end registerFunctionNames()


    /**
     * Processes this function call.
     *
     * @param PHP_CodeSniffer_File $phpcsFile
     *   The file being scanned.
     * @param int                  $stackPtr
     *   The position of the function call in the stack.
     * @param int                  $openBracket
     *   The position of the opening parenthesis in the stack.
     * @param int                  $closeBracket
     *   The position of the closing parenthesis in the stack.
     *
     * @return void
     */
    public function processFunctionCall(
        PHP_CodeSniffer_File $phpcsFile,
        $stackPtr,
        $openBracket,
        $closeBracket
    ) {
        $tokens = $phpcsFile->getTokens();

        // Only check constants in the global scope in module files.
        if (empty($tokens[$stackPtr]['conditions']) === false || substr($phpcsFile->getFilename(), -7) !== '.module') {
            return;
        }

        $coreVersion = DrupalPractice_Project::getCoreVersion($phpcsFile);
        if ($coreVersion !== '8.x') {
            return;
        }

        $warning = 'Global constants should not be used, move it to a class or interface';
        $phpcsFile->addWarning($warning, $stackPtr, 'GlobalConstant');

    }//end processFunctionCall()


}//end class
