<?php
/**
 * DrupalPractice_Sniffs_General_GlobalConstantSniff
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that globally defined constants are not used in Drupal 8 and higher.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalPractice_Sniffs_General_GlobalConstantSniff implements PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_CONST);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The current file being processed.
     * @param int                  $stackPtr  The position of the current token
     *                                        in the stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // Only check constants in the global scope.
        if (empty($tokens[$stackPtr]['conditions']) === false) {
            return;
        }

        $coreVersion = DrupalPractice_Project::getCoreVersion($phpcsFile);
        if ($coreVersion !== '8.x') {
            return;
        }

        $warning = 'Global constants should not be used, move it to a class or interface';
        $phpcsFile->addWarning($warning, $stackPtr, 'GlobalConstant');

    }//end process()


}//end class
