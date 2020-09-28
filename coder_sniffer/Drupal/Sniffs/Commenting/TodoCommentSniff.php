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
            T_DOC_COMMENT_STRING,
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
        $tokens = $phpcsFile->getTokens();
        // Standard comments and multi-line comments where the "@" is missing so
        // it does not register as a T_DOC_COMMENT_TAG.
        if ($tokens[$stackPtr]['code'] === T_COMMENT || $tokens[$stackPtr]['code'] === T_DOC_COMMENT_STRING) {
            $comment = $tokens[$stackPtr]['content'];
            $this->checkTodoFormat($phpcsFile, $stackPtr, $comment);
        } else if ($tokens[$stackPtr]['code'] === T_DOC_COMMENT_TAG) {
            // Document comment tag (i.e. comments that begin with "@").
            // Determine if this is related at all and build the full comment line
            // from the various segments that the line is parsed into.
            $expression = '/^@to/i';
            if ((bool) preg_match($expression, $tokens[$stackPtr]['content']) === true) {
                $index   = $stackPtr;
                $comment = '';
                while ($tokens[$index]['line'] === $tokens[$stackPtr]['line']) {
                    $comment .= $tokens[$index]['content'];
                    $index++;
                }

                $this->checkTodoFormat($phpcsFile, $stackPtr, $comment);
            }//end if
        }

    }//end process()


    /**
     * Checks a comment string for the correct syntax.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token
     *                                               in the stack passed in $tokens.
     * @param string                      $comment   The comment text.
     *
     * @return void
     */
    private function checkTodoFormat(File $phpcsFile, $stackPtr, string $comment)
    {
        $expression = '/^(\/\/|\*)*\s*(?i)(?=(@*to(-|\s|)+do))(?-i)(?!@todo\s(?!-|:)\S)/m';
        if ((bool) preg_match($expression, $comment) === true) {
            $comment = trim($comment, " /\r\n");
            $phpcsFile->addError("'%s' should match the format '@todo Some task'", $stackPtr, 'TodoFormat', [$comment]);
        }

    }//end checkTodoFormat()


}//end class
