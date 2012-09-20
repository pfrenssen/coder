<?php
/**
 * Drupal_Sniffs_Semantics_InstallTSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that t() and st() are not used in hook_install() and hook_requirements().
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Semantics_InstallTSniff implements PHP_CodeSniffer_Sniff
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
        if ($tokens[$stackPtr]['content'] !== ($fileName.'_install')
            && $tokens[$stackPtr]['content'] !== ($fileName.'_requirements')
        ) {
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
            if ($tokens[$string]['content'] === 't' || $tokens[$string]['content'] === 'st') {
                $opener = $phpcsFile->findNext(
                    PHP_CodeSniffer_Tokens::$emptyTokens,
                    ($string + 1),
                    null,
                    true
                );
                if ($opener !== false
                    && $tokens[$opener]['code'] === T_OPEN_PARENTHESIS
                ) {
                    $error = 'Do not use t() or st() in installation phase hooks, use $t = get_t() to retrieve the appropriate localization function name';
                    $phpcsFile->addError($error, $string, 'TranslationFound');
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
