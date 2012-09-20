<?php
/**
 * Drupal_Sniffs_Semanitcs_TInHookMenuSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that t() is not used in hook_menu().
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Semantics_TInHookMenuSniff implements PHP_CodeSniffer_Sniff
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
     * @param int                  $stackPtr  The position of the current token in
     *                                        the stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $fileExtension = strtolower(substr($phpcsFile->getFilename(), -6));
        // Only check in *.module files.
        if ($fileExtension !== 'module') {
            return;
        }

        $fileName = substr(basename($phpcsFile->getFilename()), 0, -7);
        $tokens   = $phpcsFile->getTokens();
        if ($tokens[$stackPtr]['content'] !== ($fileName.'_menu')) {
            return;
        }

        // Check if this is a function definition.
        $function = $phpcsFile->findPrevious(
            PHP_CodeSniffer_Tokens::$emptyTokens,
            ($stackPtr - 1),
            null,
            true
        );
        if ($tokens[$function]['code'] !== T_FUNCTION) {
            return;
        }

        // Search in the function body for t() calls.
        $string = $phpcsFile->findNext(
            T_STRING,
            $tokens[$function]['scope_opener'],
            $tokens[$function]['scope_closer']
        );
        while ($string !== false) {
            if ($tokens[$string]['content'] === 't') {
                $opener = $phpcsFile->findNext(
                    PHP_CodeSniffer_Tokens::$emptyTokens,
                    ($string + 1),
                    null,
                    true
                );
                if ($opener !== false
                    && $tokens[$opener]['code'] === T_OPEN_PARENTHESIS
                ) {
                    $error = 'Do not use t() in hook_menu()';
                    $phpcsFile->addError($error, $string, 'TFound');
                }
            }

            $string = $phpcsFile->findNext(
                T_STRING,
                ($string + 1),
                $tokens[$function]['scope_closer']
            );
        }//end while

    }//end process()


}//end class

?>
