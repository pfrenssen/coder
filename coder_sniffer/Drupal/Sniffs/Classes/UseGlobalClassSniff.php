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

        // Find the first declaration, marking the end of the use statements.
        $bodyStart = $phpcsFile->findNext([T_CLASS, T_INTERFACE, T_TRAIT, T_FUNCTION], 0);

        // Ensure we are in the global scope, to exclude trait use statements.
        if (empty($tokens[$stackPtr]['conditions']) === false) {
            return;
        }

        // End of the full statement.
        $stmtEnd = $phpcsFile->findNext(T_SEMICOLON, $stackPtr);

        $lineStart = $stackPtr;
        // Iterate through a potential multiline use statement.
        while (false !== $lineEnd = $phpcsFile->findNext([T_SEMICOLON, T_COMMA], ($lineStart + 1), ($stmtEnd + 1))) {
            // We are only interested in imports that contain no backslash,
            // which means this is a class without a namespace.
            // Also skip function imports.
            if ($phpcsFile->findNext(T_NS_SEPARATOR, $lineStart, $lineEnd) !== false
                || $phpcsFile->findNext(T_STRING, $lineStart, $lineEnd, false, 'function') !== false
            ) {
                $lineStart = $lineEnd;
                continue;
            }

            // The first string token is the class name.
            $class     = $phpcsFile->findNext(T_STRING, $lineStart, $lineEnd);
            $className = $tokens[$class]['content'];
            // If there is more than one string token, the last one is the alias.
            $alias     = $phpcsFile->findPrevious(T_STRING, $lineEnd, $stackPtr);
            $aliasName = $tokens[$alias]['content'];

            $error = 'Non-namespaced classes/interfaces/traits should not be referenced with use statements';
            $phpcsFile->addFixableError($error, $class, 'RedundantUseStatement');

            $phpcsFile->fixer->beginChangeset();

            // Remove the entire line by default.
            $start = $lineStart;
            $end   = $lineEnd;
            $next  = $phpcsFile->findNext(T_WHITESPACE, ($end + 1), null, true);

            if ($tokens[$lineStart]['code'] === T_COMMA) {
                // If there are lines before this one,
                // then leave the ending delimiter in place.
                $end = ($lineEnd - 1);
            } else if ($tokens[$lineEnd]['code'] === T_COMMA) {
                // If there are lines after, but not before,
                // then leave the use keyword.
                $start = $class;
            } else if ($tokens[$next]['code'] === T_USE) {
                // If the whole statement is removed, and there is one after it,
                // then also remove the linebreaks.
                $end = ($next - 1);
            }

            for ($i = $start; $i <= $end; $i++) {
                $phpcsFile->fixer->replaceToken($i, '');
            }

            // Find all usages of the class, and add a leading backslash.
            // Only start looking after the end of the use statement block.
            for ($i = $bodyStart; $i !== false; $i = $phpcsFile->findNext(T_STRING, ($i + 1), null, false, $aliasName)) {
                $before = $phpcsFile->findPrevious(T_WHITESPACE, ($i - 1), null, true);
                $after  = $phpcsFile->findNext(T_WHITESPACE, ($i + 1), null, true);

                if (true === self::isClassReference($tokens[$before]['code'], $tokens[$after]['code'])) {
                    $phpcsFile->fixer->replaceToken($i, '\\'.$className);
                }
            }

            $phpcsFile->fixer->endChangeset();

            $lineStart = $lineEnd;
        }//end while

    }//end process()


    /**
     * Check if a particular string token is a reference to a class.
     *
     * @param int|string $before The first non-space token type before the string.
     * @param int|string $after  The first non-space token type after the string.
     *
     * @return bool TRUE if the string token is a class reference.
     */
    private static function isClassReference($before, $after): bool
    {
        // Look for any of the following:
        // Constructor calls: "new Class(".
        if ($before === T_NEW && $after === T_OPEN_PARENTHESIS) {
            return true;
        }

        // Trait usage: "use Class;" and "use Class {".
        if ($before === T_USE
            && ($after === T_SEMICOLON || $after === T_OPEN_CURLY_BRACKET)
        ) {
            return true;
        }

        // Type hints and static calls: "Class::" or "Class $var" with no preceding backslash.
        if ($before !== T_NS_SEPARATOR
            && ($after === T_DOUBLE_COLON || $after === T_VARIABLE)
        ) {
            return true;
        }

        // Inheritance: "Class {", preceded by "implements", "extends", ",".
        if (($before === T_COMMA || $before === T_EXTENDS || $before === T_IMPLEMENTS)
            && $after === T_OPEN_CURLY_BRACKET
        ) {
            return true;
        }

        // Return type hints: ": Class;", "?Class;", ": Class {", "?Class {".
        if (($before === T_COLON || $before === T_NULLABLE)
            && ($after === T_OPEN_CURLY_BRACKET || $after === T_SEMICOLON)
        ) {
            return true;
        }

        return false;

    }//end isClassReference()


}//end class
