<?php
/**
 * DrupalCodingStandard_Sniffs_WhiteSpace_EmptyLinesSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @author   Klaus Purer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * DrupalCodingStandard_Sniffs_WhiteSpace_EmptyLinesSniff.
 *
 * Checks that there are not more than 2 empty lines following each other. Checks
 * that a file ends in exactly one single new line character.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @author   Klaus Purer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_WhiteSpace_EmptyLinesSniff implements PHP_CodeSniffer_Sniff
{

    /**
     * A list of tokenizers this sniff supports.
     *
     * @var array
     */
    public $supportedTokenizers = array(
                                   'PHP',
                                   'JS',
                                  );


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(
                T_WHITESPACE,
                T_CLOSE_TAG,
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
        $tokens = $phpcsFile->getTokens();

        if ($tokens[$stackPtr]['content'] === $phpcsFile->eolChar
            && isset($tokens[$stackPtr + 1]) === true
            && $tokens[$stackPtr + 1]['content'] === $phpcsFile->eolChar
            && isset($tokens[$stackPtr + 2]) === true
            && $tokens[$stackPtr + 2]['content'] === $phpcsFile->eolChar
            && isset($tokens[$stackPtr + 3]) === true
            && $tokens[$stackPtr + 3]['content'] === $phpcsFile->eolChar
        ) {
            $error = 'More than 2 empty lines are not allowed';
            $phpcsFile->addError($error, $stackPtr + 3, 'EmptyLines');
        }

        $error     = false;
        $token     = $tokens[$stackPtr];
        $lastToken = count($tokens) === ($stackPtr + 1);
        switch ($token['code']) {
            case T_WHITESPACE:
                $nextWhiteSpace = $phpcsFile->findNext(T_WHITESPACE, $stackPtr + 1);
                $nextCloseTag   = $phpcsFile->findNext(T_CLOSE_TAG, $stackPtr + 1);
                // Check if this is the last white space in the file.
                if ($nextWhiteSpace === false && $nextCloseTag === false) {
                    // There has to be a white space at the end.
                    if ($lastToken === false) {
                        $error = true;
                        break;
                    }
                    // The only white space allowed is the \n character.
                    if ($token['content'] !== $phpcsFile->eolChar) {
                        $error = true;
                        break;
                    }
                    // Only one new line character is allowed a t the end.
                    if (isset($tokens[$stackPtr - 1]) === true && $tokens[$stackPtr - 1]['content'] === $phpcsFile->eolChar) {
                        $error = true;
                        break;
                    }
                }
                break;
            case T_CLOSE_TAG:
                // The close tag must end in a \n if it is the last token in the file.
                if ($lastToken === true && substr($token['content'], -1) !== $phpcsFile->eolChar) {
                    $error = true;
                }
                break;
            case T_INLINE_HTML:
                if ($lastToken === true) {
                    // The last line has to end with a \n.
                    if (substr($token['content'], -1) !== $phpcsFile->eolChar) {
                        $error = true;
                        break;
                    }
                    // Blank lines at the end are not allowed.
                    if ($token['content'] === $phpcsFile->eolChar) {
                        $error = true;
                        break;
                    }
                }
                break;
        }//end switch

        if ($error === true) {
            $error = 'Files must end in a single new line character';
            $phpcsFile->addError($error, $stackPtr, 'EmptyLinesFileEnd');
        }

    }//end process()


}//end class

?>
