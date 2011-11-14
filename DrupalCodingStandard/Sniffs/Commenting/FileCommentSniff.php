<?php
/**
 * Parses and verifies the doc comments for files.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

if (class_exists('PHP_CodeSniffer_CommentParser_ClassCommentParser', true) === false) {
    throw new PHP_CodeSniffer_Exception('Class PHP_CodeSniffer_CommentParser_ClassCommentParser not found');
}

/**
 * Parses and verifies the doc comments for files.
 *
 * Verifies that :
 * <ul>
 *  <li>A doc comment exists.</li>
 *  <li>There is a blank newline after the @file statement.</li>
 * </ul>
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

class DrupalCodingStandard_Sniffs_Commenting_FileCommentSniff implements PHP_CodeSniffer_Sniff
{
    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_OPEN_TAG);

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
        // We are only interested if this is the first open tag.
        if ($stackPtr !== 0) {
            if ($phpcsFile->findPrevious(T_OPEN_TAG, ($stackPtr - 1)) !== false) {
                return;
            }
        }

        $tokens = $phpcsFile->getTokens();

        // Find the next non whitespace token.
        $commentStart
            = $phpcsFile->findNext(T_WHITESPACE, ($stackPtr + 1), null, true);

        $errorToken = ($stackPtr + 1);
        if (isset($tokens[$errorToken]) === false) {
            $errorToken--;
        }

        if ($tokens[$commentStart]['code'] === T_CLOSE_TAG) {
            // We are only interested if this is the first open tag.
            return;
        } else if ($tokens[$commentStart]['code'] === T_COMMENT) {
            $error = 'You must use "/**" style comments for a file comment';
            $phpcsFile->addError($error, $errorToken, 'WrongStyle');
            return;
        } else if ($commentStart === false
            || $tokens[$commentStart]['code'] !== T_DOC_COMMENT
        ) {
            $phpcsFile->addError('Missing file doc comment', $errorToken, 'Missing');
            return;
        } else {

            // Extract the header comment docblock.
            $commentEnd = $phpcsFile->findNext(
                T_DOC_COMMENT,
                ($commentStart + 1),
                null,
                true
            );

            $commentEnd--;

            if ($tokens[$commentStart]['content'] !== ('/**' . $phpcsFile->eolChar)) {
                $phpcsFile->addError('The first line in the file doc comment must only be "/**"', $commentStart);
                return;
            }
            $fileLine = $phpcsFile->findNext(T_DOC_COMMENT, $commentStart + 1);
            if ($tokens[$fileLine]['content'] !== (' * @file' . $phpcsFile->eolChar)) {
                $phpcsFile->addError('The second line in the file doc comment must be " * @file"', $fileLine);
                return;
            }
            $line = $fileLine + 1;
            while ($line = $phpcsFile->findNext(T_DOC_COMMENT, $line, $commentEnd)) {
                $found = preg_match('/^ \* [^ ]/', $tokens[$line]['content']);
                if (!$found && $tokens[$line]['content'] !== (' *' . $phpcsFile->eolChar)) {
                    $phpcsFile->addError('File doc description lines must start with " * " and no additional indentation', $line);
                }
                $line++;
            }
            if ($tokens[$commentEnd]['content'] !== (' */')) {
              $phpcsFile->addError('The last line in the file doc comment must be " */"', $commentEnd);
            }
        }
    }//end process()

}//end class

?>
