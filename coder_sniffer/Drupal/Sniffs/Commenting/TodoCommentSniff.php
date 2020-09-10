<?php
/**
 * Parses and verifies comment language.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Commenting;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * Parses and verifies that comments use the correct @todo format.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class TodoCommentSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [
            T_COMMENT,
            T_DOC_COMMENT_TAG,
        ];

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
        $tokens  = $phpcsFile->getTokens();
        // Use the comment text by default.
        $comment = $tokens[$stackPtr]['content'];
        // Use the next line for multi-line comments.
        if ($tokens[$stackPtr]['code'] === T_DOC_COMMENT_TAG) {
            $comment = $phpcsFile->findNext(T_DOC_COMMENT_STRING, ($stackPtr + 1));
        }

        $expression = '/(?i)(?=(@to(-|\s|)+do))(?-i)(?!@todo\s\w)/m';
        if ((bool) preg_match($expression, $comment) === true) {
            $phpcsFile->addError('@todo comments should be in the "@todo Some task." format.', $stackPtr, 'TodoFormat');
        }

    }//end process()


}//end class
