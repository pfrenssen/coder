<?php
/**
 * Drupal_Sniffs_Semanitcs_TInHookSchemaSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that t() is not used in hook_schema().
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Semantics_TInHookSchemaSniff implements PHP_CodeSniffer_Sniff
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
        $fileExtension = strtolower(substr($phpcsFile->getFilename(), -7));
        // Only check in *.install files.
        if ($fileExtension !== 'install') {
            return;
        }

        $fileName = substr(basename($phpcsFile->getFilename()), 0, -8);
        $tokens   = $phpcsFile->getTokens();
        if ($tokens[$stackPtr]['content'] !== ($fileName.'_schema')) {
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
                    $error = 'Do not use t() in hook_schema(), this will only generate overhead for translators';
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
