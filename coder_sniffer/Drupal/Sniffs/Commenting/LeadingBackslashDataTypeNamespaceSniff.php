<?php
/**
 * \Drupal\Sniffs\Commenting\LeadingBackslashDataTypeNamespaceSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Commenting;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;
use PHP_CodeSniffer\Util\Tokens;

/**
 * Checks that data types in param, return, var, coversDefaultClass and throws tags are fully namespaced,
 * even when the full namespace is given without the leading backslash.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class LeadingBackslashDataTypeNamespaceSniff implements Sniff
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
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token in
     *                                               the stack passed in $tokens.
     *
     * @return void
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // Only check use statements in the global scope.
        if (empty($tokens[$stackPtr]['conditions']) === false) {
            return;
        }

        // Seek to the end of the statement and get the string before the semicolon.
        $semiColon = $phpcsFile->findEndOfStatement($stackPtr);
        if ($tokens[$semiColon]['code'] !== T_SEMICOLON) {
            return;
        }

        $classPtr = $phpcsFile->findPrevious(
            Tokens::$emptyTokens,
            ($semiColon - 1),
            null,
            true
        );

        if ($tokens[$classPtr]['code'] !== T_STRING) {
            return;
        }

        // Replace @var, @return, @param and @throws data types in doc comments
        // with the fully qualified class names, even when the full namespace is
        // given without the leading backslash.
        $useNamespacePtr = $phpcsFile->findNext([T_STRING], ($stackPtr + 1));
        $useNamespaceEnd = $phpcsFile->findNext(
            [
                T_NS_SEPARATOR,
                T_STRING,
            ],
            ($useNamespacePtr + 1),
            null,
            true
        );
        $fullNamespace   = $phpcsFile->getTokensAsString($useNamespacePtr, ($useNamespaceEnd - $useNamespacePtr));

        $tag = $phpcsFile->findNext(T_DOC_COMMENT_TAG, ($stackPtr + 1));

        while ($tag !== false) {
            if (($tokens[$tag]['content'] === '@var'
                || $tokens[$tag]['content'] === '@return'
                || $tokens[$tag]['content'] === '@param'
                || $tokens[$tag]['content'] === '@coversDefaultClass'
                || $tokens[$tag]['content'] === '@see'
                || $tokens[$tag]['content'] === '@throws')
                && isset($tokens[($tag + 1)]) === true
                && $tokens[($tag + 1)]['code'] === T_DOC_COMMENT_WHITESPACE
                && isset($tokens[($tag + 2)]) === true
                && $tokens[($tag + 2)]['code'] === T_DOC_COMMENT_STRING
            ) {
                if (strtok($tokens[($tag + 2)]['content'], ' $') === $fullNamespace
                    || strtok($tokens[($tag + 2)]['content'], '|') === $fullNamespace
                    || strtok($tokens[($tag + 2)]['content'], '::') === $fullNamespace
                ) {
                    $error = 'Data types in %s tags need to be fully namespaced with leading backspace';
                    $data  = [$tokens[$tag]['content']];
                    $fix   = $phpcsFile->addFixableError($error, ($tag + 2), 'LeadingBackslashDataTypeNamespace', $data);
                    if ($fix === true) {
                        $replacement = '\\'.$fullNamespace;
                        if ($tokens[$tag]['content'] === '@param' || $tokens[$tag]['content'] === '@var' || $tokens[$tag]['content'] === '@return') {
                            if (strpos($tokens[($tag + 2)]['content'], '|') !== false) {
                                $replacement = '\\'.$fullNamespace.$this->subStringFromNeedle($tokens[($tag + 2)]['content'], '|');
                            } else if (strpos($tokens[($tag + 2)]['content'], ' $') !== false) {
                                $replacement = '\\'.$fullNamespace.$this->subStringFromNeedle($tokens[($tag + 2)]['content'], ' $');
                            }
                        } else if ($tokens[$tag]['content'] === '@see' && strpos($tokens[($tag + 2)]['content'], '::') !== false) {
                            $replacement = '\\'.$fullNamespace.$this->subStringFromNeedle($tokens[($tag + 2)]['content'], '::');
                        }

                        $phpcsFile->fixer->replaceToken(($tag + 2), $replacement);
                    }//end if
                }//end if
            }//end if

            $tag = $phpcsFile->findNext(T_DOC_COMMENT_TAG, ($tag + 1));
        }//end while

    }//end process()


    /**
     * Returns a substring of the given input string, starting from the given needle.
     *
     * @param string $input  The input string.
     * @param string $needle The needle.
     *
     * @return string
     */
    private function subStringFromNeedle(string $input, string $needle): string
    {
        $offset = (-1 * (strlen($input) - strpos($input, $needle)));

        return substr($input, $offset);

    }//end subStringFromNeedle()


}//end class
