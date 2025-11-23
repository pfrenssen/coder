<?php
/**
 * \Drupal\Sniffs\Classes\FullyQualifiedNamespaceSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Classes;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;
use PHP_CodeSniffer\Util\Tokens;

/**
 * Checks that class references do not use FQN but use statements.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class FullyQualifiedNamespaceSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [
            T_NAME_FULLY_QUALIFIED,
            T_NAME_QUALIFIED,
        ];
    }


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

        // Skip this sniff in *api.php files because they want to have fully
        // qualified names for documentation purposes.
        if (substr($phpcsFile->getFilename(), -8) === '.api.php') {
            return ($phpcsFile->numTokens + 1);
        }

        // We are only interested in a backslash embedded between strings, which
        // means this is a class reference with more than one namespace part.
        if (substr_count($tokens[$stackPtr]['content'], '\\') === 1
            && strpos($tokens[$stackPtr]['content'], '\\') === 0
        ) {
            return;
        }

        // Skip names in PHP attributes, no standards defined yet.
        if (isset($tokens[$stackPtr]['attribute_closer']) === true) {
            return $tokens[$stackPtr]['attribute_closer'];
        }

        // Check if this is a use statement and ignore those.
        $before = $phpcsFile->findPrevious((Tokens::EMPTY_TOKENS + Tokens::NAME_TOKENS + [T_COMMA => T_COMMA, T_AS => T_AS]), ($stackPtr - 1), null, true);
        if ($tokens[$before]['code'] === T_USE || $tokens[$before]['code'] === T_NAMESPACE) {
            return;
        }

        $fullName  = trim($tokens[$stackPtr]['content'], '\\ ');
        $parts     = explode('\\', $fullName);
        $className = end($parts);

        // Check if there is a use statement already for this class and
        // namespace.
        $conflict     = false;
        $alreadyUsed  = false;
        $aliasName    = false;
        $useStatement = $phpcsFile->findNext(T_USE, 0);
        while ($useStatement !== false && empty($tokens[$useStatement]['conditions']) === true) {
            $endPtr      = $phpcsFile->findEndOfStatement($useStatement);
            $useEnd      = ($phpcsFile->findNext((Tokens::EMPTY_TOKENS + Tokens::NAME_TOKENS), ($useStatement + 1), null, true) - 1);
            $useFullName = trim($phpcsFile->getTokensAsString(($useStatement + 1), ($useEnd - $useStatement)));

            // Check if use statement contains an alias.
            $asPtr = $phpcsFile->findNext(T_AS, ($useEnd + 1), $endPtr);
            if ($asPtr !== false) {
                $aliasName = trim($phpcsFile->getTokensAsString(($asPtr + 1), ($endPtr - 1 - $asPtr)));
            }

            if (strcasecmp($useFullName, $fullName) === 0) {
                $alreadyUsed = true;
                break;
            }

            $parts        = explode('\\', $useFullName);
            $useClassName = end($parts);

            // Check if the resulting classname would conflict with another
            // use statement.
            if ($aliasName === $className || $useClassName === $className) {
                $conflict = true;
                break;
            }

            $aliasName = false;
            // Check if we're currently in a multi-use statement.
            if ($tokens[$endPtr]['code'] === T_COMMA) {
                $useStatement = $endPtr;
                continue;
            }

            $useStatement = $phpcsFile->findNext(T_USE, ($endPtr + 1));
        }//end while

        if ($conflict === false) {
            $classStatement = $phpcsFile->findNext(T_CLASS, 0);
            while ($classStatement !== false) {
                $afterClassStatement = $phpcsFile->findNext(T_WHITESPACE, ($classStatement + 1), null, true);
                // Check for 'class ClassName' declarations.
                if ($tokens[$afterClassStatement]['code'] === T_STRING) {
                    $declaredName = $tokens[$afterClassStatement]['content'];
                    if ($declaredName === $className) {
                        $conflict = true;
                        break;
                    }
                }

                $classStatement = $phpcsFile->findNext(T_CLASS, ($classStatement + 1));
            }
        }

        $error = 'Namespaced classes/interfaces/traits should be referenced with use statements';
        if ($conflict === true) {
            $fix = false;
            $phpcsFile->addError($error, $stackPtr, 'UseStatementMissing');
        } else {
            $fix = $phpcsFile->addFixableError($error, $stackPtr, 'UseStatementMissing');
        }

        if ($fix === true) {
            $phpcsFile->fixer->beginChangeset();

            // Use alias name if available.
            if ($aliasName !== false) {
                $phpcsFile->fixer->replaceToken($stackPtr, $aliasName);
            } else {
                $phpcsFile->fixer->replaceToken($stackPtr, $className);
            }

            // Insert use statement at the beginning of the file if it is not there
            // already. Also check if another sniff (for example
            // UnusedUseStatementSniff) has already deleted the use statement, then
            // we need to add it back.
            if ($alreadyUsed === false
                || $phpcsFile->fixer->getTokenContent($useStatement) !== $tokens[$useStatement]['content']
            ) {
                if ($aliasName !== false) {
                    $use = "use $fullName as $aliasName;";
                } else {
                    $use = "use $fullName;";
                }

                // Check if there is a group of use statements and add it there.
                $useStatement = $phpcsFile->findNext(T_USE, 0);
                if ($useStatement !== false && empty($tokens[$useStatement]['conditions']) === true) {
                    $phpcsFile->fixer->addContentBefore($useStatement, "$use\n");
                } else {
                    // Check if there is a namespace declaration and add it there.
                    $namespace = $phpcsFile->findNext(T_NAMESPACE, 0);
                    if ($namespace !== false) {
                        $beginning = $phpcsFile->findEndOfStatement($namespace);
                        $phpcsFile->fixer->addContent($beginning, "\n\n$use\n");
                    } else {
                        // Check if there is an @file comment.
                        $fileComment = $phpcsFile->findNext(T_WHITESPACE, 1, null, true);
                        if ($tokens[$fileComment]['code'] === T_DOC_COMMENT_OPEN_TAG) {
                            $beginning = $tokens[$fileComment]['comment_closer'];
                            $phpcsFile->fixer->addContent($beginning, "\n\n$use\n");
                        } else {
                            $phpcsFile->fixer->addContent(0, "\n\n$use\n");
                        }
                    }
                }
            }//end if

            $phpcsFile->fixer->endChangeset();
        }//end if
    }
}
