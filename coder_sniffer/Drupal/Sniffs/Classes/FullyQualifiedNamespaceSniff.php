<?php
/**
 * Drupal_Sniffs_Classes_FullyQualifiedNamespaceSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that class references do not use FQN but use stataments.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Classes_FullyQualifiedNamespaceSniff implements PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_NS_SEPARATOR);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The PHP_CodeSniffer file where the
     *                                        token was found.
     * @param int                  $stackPtr  The position in the PHP_CodeSniffer
     *                                        file's token stack where the token
     *                                        was found.
     *
     * @return void|int Optionally returns a stack pointer. The sniff will not be
     *                  called again on the current file until the returned stack
     *                  pointer is reached. Return (count($tokens) + 1) to skip
     *                  the rest of the file.
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // Skip this sniff in *api.php files because they want to have fully
        // qualified names for documentation purposes.
        if (substr($phpcsFile->getFilename(), -8) === '.api.php') {
            return (count($tokens) + 1);
        }

        // We are only interested in a backslash embedded between strings, which
        // means this is a class reference with more than once namespace part.
        if ($tokens[($stackPtr - 1)]['code'] !== T_STRING || $tokens[($stackPtr + 1)]['code'] !== T_STRING) {
            return;
        }

        // Check if this is a use statement and ignore those.
        $before = $phpcsFile->findPrevious([T_STRING, T_NS_SEPARATOR, T_WHITESPACE], $stackPtr, null, true);
        if ($tokens[$before]['code'] === T_USE || $tokens[$before]['code'] === T_NAMESPACE) {
            return $phpcsFile->findNext([T_STRING, T_NS_SEPARATOR], ($stackPtr + 1), null, true);
        }

        $error = 'Namespaced classes/interfaces/traits should be referenced with use statements';
        $phpcsFile->addError($error, $stackPtr, 'UseStatementMissing');

        // Continue after this class reference so that errors for this are not
        // flagged multiple times.
        return $phpcsFile->findNext([T_STRING, T_NS_SEPARATOR], ($stackPtr + 1), null, true);

    }//end process()


}//end class
