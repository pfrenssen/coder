<?php
/**
 * \Drupal\Sniffs\Files\TxtFileLineLengthSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Files;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * \Drupal\Sniffs\Files\TxtFileLineLengthSniff.
 *
 * Checks all lines in a *.txt or *.md file and throws warnings if they are over 80
 * characters in length.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class TxtFileLineLengthSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_INLINE_HTML);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token in the
     *                                               stack passed in $tokens.
     *
     * @return void
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $fileExtension = strtolower(substr($phpcsFile->getFilename(), -3));
        if ($fileExtension === 'txt' || $fileExtension === '.md') {
            $tokens = $phpcsFile->getTokens();

            $content    = rtrim($tokens[$stackPtr]['content']);
            $lineLength = mb_strlen($content, 'UTF-8');
            // Lines without spaces are allowed to be longer (for example long URLs).
            // Markdown allowed to be longer for lines
            // - without spaces
            // - starting with #
            // - containing URLs (https://)
            // - starting with | (tables).
            if ($lineLength > 80 && preg_match('/^([^ ]+$|#|.*https?:\/\/|\|)/', $content) === 0) {
                $data    = array(
                            80,
                            $lineLength,
                           );
                $warning = 'Line exceeds %s characters; contains %s characters';
                $phpcsFile->addWarning($warning, $stackPtr, 'TooLong', $data);
            }
        }

    }//end process()


}//end class
