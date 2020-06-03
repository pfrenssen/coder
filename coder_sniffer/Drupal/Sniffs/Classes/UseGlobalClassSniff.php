<?php
/**
 * \Drupal\Sniffs\Classes\UseGlobalClassSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Classes;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * Checks non-namespaced classes are referenced by FQN, not imported.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class UseGlobalClassSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [T_USE];

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The PHP_CodeSniffer file where the
     *                                               token was found.
     * @param int                         $stackPtr  The position in the PHP_CodeSniffer
     *                                               file's token stack where the token
     *                                               was found.
     *
     * @return void|int Optionally returns a stack pointer. The sniff will not be
     *                  called again on the current file until the returned stack
     *                  pointer is reached. Return $phpcsFile->numTokens + 1 to skip
     *                  the rest of the file.
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // Ensure we are in the global scope, to exclude trait use statements.
        if (empty($tokens[$stackPtr]['conditions']) === false) {
            return;
        }

        // We are only interested in use statements that contain no backslash,
        // which means this is a class without a namespace.
        $stmtEnd = $phpcsFile->findNext(T_SEMICOLON, $stackPtr);
        if ($phpcsFile->findNext(T_NS_SEPARATOR, $stackPtr, $stmtEnd) !== false) {
            return;
        }

        // The first string token is the class name.
        $class     = $phpcsFile->findNext(T_STRING, $stackPtr, $stmtEnd);
        $className = $phpcsFile->getTokensAsString($class, 1);
        // If there is more than one string token, the last one is the alias.
        $alias     = $phpcsFile->findPrevious(T_STRING, $stmtEnd, $stackPtr);
        $aliasName = $phpcsFile->getTokensAsString($alias, 1);

        $error = 'Non-namespaced classes/interfaces/traits should not be referenced with use statements';
        $phpcsFile->addFixableError($error, $stackPtr, 'RedundantUseStatement');

        $phpcsFile->fixer->beginChangeset();

        // Remove the use statement.
        for ($i = $stackPtr; $i <= $stmtEnd; $i++) {
            $phpcsFile->fixer->replaceToken($i, '');
        }

        // Find all usages of the class, and add a leading backslash.
        for ($i = $stackPtr; $i !== false; $i = $phpcsFile->findNext(T_STRING, ($i + 1), null, false, $aliasName)) {
            $before = $phpcsFile->findPrevious(T_WHITESPACE, ($i - 1), null, true);
            $after  = $phpcsFile->findNext(T_WHITESPACE, ($i + 1), null, true);

            // Look for any of the following:
            // "new Class(",
            // "Class::" with no preceding backslash,
            // "Class $var" with no preceding backslash,
            // ": Class {".
            if (($tokens[$before]['code'] === T_NEW && $tokens[$after]['code'] === T_OPEN_PARENTHESIS)
                || ($tokens[$before]['code'] !== T_NS_SEPARATOR && $tokens[$after]['code'] === T_DOUBLE_COLON)
                || ($tokens[$before]['code'] !== T_NS_SEPARATOR && $tokens[$after]['code'] === T_VARIABLE)
                || ($tokens[$before]['code'] === T_COLON && $tokens[$after]['code'] === T_OPEN_CURLY_BRACKET)
            ) {
                $phpcsFile->fixer->replaceToken($i, '\\'.$className);
            }
        }

        $phpcsFile->fixer->endChangeset();

    }//end process()


}//end class
