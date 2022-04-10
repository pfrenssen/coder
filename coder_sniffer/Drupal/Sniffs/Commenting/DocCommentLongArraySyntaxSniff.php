<?php
/**
 * Ensures @code annotations in doc blocks don't contain long array syntax.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Commenting;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * Ensures @code annotations in doc blocks don't contain long array syntax.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DocCommentLongArraySyntaxSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [T_DOC_COMMENT_OPEN_TAG];

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token
     *                                               in the stack passed in $tokens.
     *
     * @return void
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $tokens       = $phpcsFile->getTokens();
        $commentEnd   = $phpcsFile->findNext(T_DOC_COMMENT_CLOSE_TAG, ($stackPtr + 1));

        // Look for @code annotations.
        $codeEnd = $stackPtr;
        do {
            $codeStart = $phpcsFile->findNext(T_DOC_COMMENT_STRING, ($codeEnd + 1), $commentEnd, TRUE, '@code');
            if ($codeStart) {
                $codeEnd = $phpcsFile->findNext(T_DOC_COMMENT_STRING, ($codeStart + 1), $commentEnd, TRUE, '@endcode');
                if ($codeEnd !== FALSE) {
                    // Check for long array syntax use inside this @code annotation.
                    for ($i = $codeStart + 1; $i < $codeEnd; $i++) {
                        if (preg_match('/\barray\s*\(/', $tokens[$i]['content'])) {
                            $error = 'Long array syntax used in doc comment code annotation';
                            $phpcsFile->addError($error, $i, 'DocLongArray');
                        }
                    }
                }
            }
        } while ($codeStart !== false);

    }//end process()


}//end class
