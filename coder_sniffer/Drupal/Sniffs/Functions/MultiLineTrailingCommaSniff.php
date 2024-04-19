<?php
/**
 * \Drupal\Sniffs\Functions\MultiLineTrailingCommaSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Functions;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;
use PHP_CodeSniffer\Util\Tokens;

/**
 * Multi-line function declarations need to have a trailing comma on the last
 * parameter.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class MultiLineTrailingCommaSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        if (version_compare(PHP_VERSION, '8.0.0') < 0) {
            return [];
        }

        return [
            T_FUNCTION,
            T_CLOSURE,
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

        $function = $tokens[$stackPtr];
        if ($tokens[$function['parenthesis_opener']]['line'] === $tokens[$function['parenthesis_closer']]['line']) {
            return;
        }

        $lastTrailingComma = $phpcsFile->findPrevious(
            Tokens::$emptyTokens,
            ($function['parenthesis_closer'] - 1),
            $function['parenthesis_opener'],
            true
        );
        if ($tokens[$lastTrailingComma]['code'] !== T_COMMA) {
            $error = 'Multi-line function declarations must have a trailing comma after the last parameter';
            $fix   = $phpcsFile->addFixableError($error, $lastTrailingComma, 'MissingTrailingComma');
            if ($fix === true) {
                $phpcsFile->fixer->addContent($lastTrailingComma, ',');
            }
        }

    }//end process()


}//end class
