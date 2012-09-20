<?php
/**
 * Drupal_Sniffs_Semantics_InstallHooksSniff
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that hook_disable(), hook_enable(), hook_install(), hook_uninstall(),
 * hook_requirements() and hook_schema() are not defined in the module file.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Semantics_InstallHooksSniff implements PHP_CodeSniffer_Sniff
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

        $tokens = $phpcsFile->getTokens();
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

        $fileName = substr(basename($phpcsFile->getFilename()), 0, -7);
        if ($tokens[$stackPtr]['content'] === ($fileName.'_install')
            || $tokens[$stackPtr]['content'] === ($fileName.'_uninstall')
            || $tokens[$stackPtr]['content'] === ($fileName.'_requirements')
            || $tokens[$stackPtr]['content'] === ($fileName.'_schema')
            || $tokens[$stackPtr]['content'] === ($fileName.'_enable')
            || $tokens[$stackPtr]['content'] === ($fileName.'_disable')
        ) {
            $error = '%s() is an installation hook and must be declared in an install file';
            $data  = array($tokens[$stackPtr]['content']);
            $phpcsFile->addError($error, $stackPtr, 'InstallHook', $data);
        }

    }//end process()


}//end class

?>
