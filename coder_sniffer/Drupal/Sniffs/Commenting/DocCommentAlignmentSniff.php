<?php
/**
 * Drupal_Sniffs_Commenting_EmptyCatchCommentSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Drupal_Sniffs_Commenting_DocCommentAlignmentSniff.
 *
 * Tests that the stars in a doc comment align correctly. Largely copied from
 * Squiz_Sniffs_Commenting_DocCommentAlignmentSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Commenting_DocCommentAlignmentSniff implements
PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_DOC_COMMENT);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token
     *                                         in the stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // We only want to get the first comment in a block. If there is
        // a comment on the line before this one, return.
        $docComment = $phpcsFile->findPrevious(T_DOC_COMMENT, ($stackPtr - 1));
        if ($docComment !== false) {
            if ($tokens[$docComment]['line'] === ($tokens[$stackPtr]['line'] - 1)) {
                return;
            }
        }

        $comments       = array($stackPtr);
        $currentComment = $stackPtr;
        $lastComment    = $stackPtr;
        while (($currentComment = $phpcsFile->findNext(T_DOC_COMMENT, ($currentComment + 1))) !== false) {
            if ($tokens[$lastComment]['line'] === ($tokens[$currentComment]['line'] - 1)) {
                $comments[]  = $currentComment;
                $lastComment = $currentComment;
            } else {
                break;
            }
        }

        // The $comments array now contains pointers to each token in the
        // comment block.
        $requiredColumn  = strpos($tokens[$stackPtr]['content'], '*');
        $requiredColumn += $tokens[$stackPtr]['column'];

        foreach ($comments as $commentPointer) {
            // Check the spacing after each asterisk.
            $content   = $tokens[$commentPointer]['content'];
            $firstChar = substr($content, 0, 1);
            $lastChar  = substr($content, -1);
            if ($firstChar !== '/' && $lastChar !== '/') {
                $matches = array();
                preg_match('|^(\s+)?\*(\s+)?|', $content, $matches);
                if (empty($matches) === false) {
                    if (isset($matches[2]) === false) {
                        $error = 'Expected 1 space between asterisk and comment; 0 found';
                        $phpcsFile->addError($error, $commentPointer, 'NoSpaceBeforeComment');
                    }
                }
            }//end if

            // Check the alignment of each asterisk.
            $currentColumn  = strpos($content, '*');
            $currentColumn += $tokens[$commentPointer]['column'];

            if ($currentColumn === $requiredColumn) {
                // Star is aligned correctly.
                continue;
            }

            $error = 'Expected %s space(s) before asterisk; %s found';
            $data  = array(
                      ($requiredColumn - 1),
                      ($currentColumn - 1),
                     );
            $phpcsFile->addError($error, $commentPointer, 'SpaceBeforeAsterisk', $data);
        }//end foreach

        if (trim($tokens[($lastComment - 1)]['content']) === '*') {
            $error = 'Additional blank line found at the end of doc comment';
            $phpcsFile->addError($error, ($lastComment - 1), 'BlankLine');
        }

    }//end process()


}//end class

?>
