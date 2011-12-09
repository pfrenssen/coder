<?php
/**
 * DrupalCodingStandard_Sniffs_WhiteSpace_FileEndSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @author   Klaus Purer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * DrupalCodingStandard_Sniffs_WhiteSpace_FileEndSniff.
 *
 * Checks that a file ends in exactly one single new line character.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @author   Klaus Purer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_WhiteSpace_FileEndSniff implements PHP_CodeSniffer_Sniff
{

    /**
     * A list of tokenizers this sniff supports.
     *
     * @var array
     */
    public $supportedTokenizers = array(
                                   'PHP',
                                   // We cannot use JS files right now because the tokenizer is strange.
                                   //'JS',
                                   'CSS',
                                  );


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(
                T_OPEN_TAG,
                T_INLINE_HTML,
               );

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
        // We want to run this method only once per file, so we remember if this has
        // been called already.
        static $called = array();
        if (isset($called[$phpcsFile->getFilename()]) === false) {
            $called[$phpcsFile->getFilename()] = true;
            $tokens = $phpcsFile->getTokens();
            // Temporary fix for CSS files, that have artificial tokens at the end.
            $fileExtension = $fileExtension = strtolower(substr($phpcsFile->getFilename(), -3));
            if ($fileExtension === 'css') {
                array_pop($tokens);
                array_pop($tokens);
            }
            $lastToken = $tokens[count($tokens) - 1];
            $error = false;
            // There must be a \n character at the end of the last token.
            if (substr($lastToken['content'], -1) !== $phpcsFile->eolChar) {
                $error = true;
            }
            // There must be only one \n character at the end of the file.
            else if ($lastToken['content'] === $phpcsFile->eolChar
                && isset($tokens[count($tokens) - 2]) === true
                && substr($tokens[count($tokens) - 2]['content'], -1) === $phpcsFile->eolChar
            ) {
                $error = true;
            }

            if ($error === true) {
                $error = 'Files must end in a single new line character';
                $phpcsFile->addError($error, count($tokens) - 1, 'FileEnd');
            }
        }//end if

    }//end process()


}//end class

?>
