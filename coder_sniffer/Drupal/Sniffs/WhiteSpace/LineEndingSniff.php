<?php

class Drupal_Sniffs_WhiteSpace_LineEndingSniff implements PHP_CodeSniffer_Sniff
{

    /**
     * A list of tokenizers this sniff supports.
     *
     * @var array
     */
    public $supportedTokenizers = array(
                                   'PHP',
                                   'JS',
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
                T_CLOSE_TAG,
                T_WHITESPACE,
                T_COMMENT,
                T_DOC_COMMENT,
               );

    }//end register()


    /**
     * Processes this sniff, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token in the
     *                                        stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // Check for end of line whitespace.
        if (strpos($tokens[$stackPtr]['content'], $phpcsFile->eolChar) === false) {
            return;
        }

        $tokenContent = rtrim($tokens[$stackPtr]['content'], $phpcsFile->eolChar);
        if (empty($tokenContent) === false) {
            if (preg_match('|^.*\s+$|', $tokenContent) !== 0) {
                $phpcsFile->addError('Whitespace found at end of line', $stackPtr, 'EndLine');
            }
        }

    }//end process()


}//end class

?>
