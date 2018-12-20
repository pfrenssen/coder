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
 * Parses and verifies that comments use gender neutral language.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class GenderNeutralCommentSniff implements Sniff
{

    /**
     * The error message.
     *
     * @var string
     */
    protected $error = 'Unnecessarily gendered language in a comment';

    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(
                T_COMMENT,
                T_DOC_COMMENT_OPEN_TAG,
               );

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

        if ($tokens[$stackPtr]['code'] === T_COMMENT) {
            $this->checkComment($phpcsFile, $stackPtr, $tokens[$stackPtr]['content']);
        }
        elseif ($tokens[$stackPtr]['code'] === T_DOC_COMMENT_OPEN_TAG) {
            $commentEnd = $tokens[$stackPtr]['comment_closer'];
            for ($i = $stackPtr; $i < $commentEnd; $i++) {
                if ($tokens[$i]['code'] === T_DOC_COMMENT_STRING) {
                    $this->checkComment($phpcsFile, $i, $tokens[$i]['content']);
                }
            }
        }


    }//end process()

    /**
     * Checks the indentation level of the comment contents.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token
     *                                               in the stack passed in $tokens.
     * @param string                      $comment   The comment to be checked.
     *
     * @return void
     */
    protected function checkComment(File $phpcsFile, $stackPtr, $comment) {
        if (preg_match('/(^|\W)(he|she|him|his|her|hers)($|\W)/i', $comment)) {
            $warning = 'Unnecessarily gendered language in a comment';
            $phpcsFile->addError($warning, $stackPtr, 'GenderNeutral');
        }
    }//end checkComment()

}//end class
