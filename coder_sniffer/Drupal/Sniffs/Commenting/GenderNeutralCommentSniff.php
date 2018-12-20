<?php
/**
 * PHP_CodeSniffer_Sniffs_Drupal_Commenting_InlineCommentSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * PHP_CodeSniffer_Sniffs_Drupal_Sniffs_Commenting_GenderNeutralCommentSniff.
 *
 * Checks that gender specific language does not appear in comments.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Commenting_GenderNeutralCommentSniff implements PHP_CodeSniffer_Sniff
{

    protected $error = 'Unnecessarily gendered language in a comment';

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
                T_COMMENT,
                T_DOC_COMMENT_OPEN_TAG,
               );

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
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
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token
     *                                        in the stack passed in $tokens.
     * @param string               $comment   The comment to be checked.
     *
     * @return void
     */
    protected function checkComment(PHP_CodeSniffer_File $phpcsFile, $stackPtr, $comment) {
        if (preg_match('/(^|\W)(he|she|him|his|her|hers)($|\W)/i', $comment)) {
            $warning = 'Unnecessarily gendered language in a comment';
            $phpcsFile->addError($warning, $stackPtr, 'GenderNeutral');
        }
    }

}//end class
