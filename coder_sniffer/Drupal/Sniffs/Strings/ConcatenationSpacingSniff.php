<?php
/**
 * Drupal_Sniffs_Strings_ConcatenationSpacingSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Makes sure there are the needed spaces between the concatenation operator (.) and
 * the strings being concatenated.
 *
 * Largely copied from Squiz_Sniffs_Strings_ConcatenationSpacingSniff and modified to
 * allow newlines.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Strings_ConcatenationSpacingSniff implements PHP_CodeSniffer_Sniff
{

    /**
     * The number of spaces before and after a string concat.
     *
     * @var int
     */
    public $spacing = 1;


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_STRING_CONCAT);

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
        $this->spacing = (int) $this->spacing;

        $tokens = $phpcsFile->getTokens();
        if ($tokens[($stackPtr - 1)]['code'] !== T_WHITESPACE) {
            $before = 0;
        } else {
            if ($tokens[($stackPtr - 2)]['line'] !== $tokens[$stackPtr]['line']) {
                $before = 'newline';
            } else {
                $before = $tokens[($stackPtr - 1)]['length'];
            }
        }

        if ($tokens[($stackPtr + 1)]['code'] !== T_WHITESPACE) {
            $after = 0;
        } else {
            if ($tokens[($stackPtr + 2)]['line'] !== $tokens[$stackPtr]['line']) {
                $after = 'newline';
            } else {
                $after = $tokens[($stackPtr + 1)]['length'];
            }
        }

        $phpcsFile->recordMetric($stackPtr, 'Spacing before string concat', $before);
        $phpcsFile->recordMetric($stackPtr, 'Spacing after string concat', $after);

        if (($before === $this->spacing || $before === 'newline')
            && ($after === $this->spacing || $after === 'newline')
        ) {
            return;
        }

        if ($this->spacing === 0) {
            $message = 'Concat operator must not be surrounded by spaces';
            $data    = array();
        } else {
            if ($this->spacing > 1) {
                $message = 'Concat operator must be surrounded by %s spaces';
            } else {
                $message = 'Concat operator must be surrounded by a single space';
            }

            $data = array($this->spacing);
        }

        $fix = $phpcsFile->addFixableError($message, $stackPtr, 'PaddingFound', $data);

        if ($fix === true) {
            $padding = str_repeat(' ', $this->spacing);
            if ($tokens[($stackPtr - 1)]['code'] === T_WHITESPACE) {
                $phpcsFile->fixer->replaceToken(($stackPtr - 1), $padding);
            } else if ($this->spacing > 0) {
                $phpcsFile->fixer->addContent(($stackPtr - 1), $padding);
            }

            if ($tokens[($stackPtr + 1)]['code'] === T_WHITESPACE) {
                $phpcsFile->fixer->replaceToken(($stackPtr + 1), $padding);
            } else if ($this->spacing > 0) {
                $phpcsFile->fixer->addContent($stackPtr, $padding);
            }
        }

    }//end process()


}//end class
