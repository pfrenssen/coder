<?php
/**
 * Verifies that class/interface/trait methods have scope modifiers.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Scope;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\AbstractScopeSniff;
use PHP_CodeSniffer\Util\Tokens;

/**
 * Verifies that class/interface/trait methods have scope modifiers.
 *
 * Largely copied from
 * \PHP_CodeSniffer\Standards\Squiz\Sniffs\Scope\MethodScopeSniff to have a
 * fixer.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class MethodScopeSniff extends AbstractScopeSniff
{


    /**
     * Constructor.
     */
    public function __construct()
    {
        parent::__construct(Tokens::OO_SCOPE_TOKENS, [T_FUNCTION]);
    }


    /**
     * Processes the function tokens within the class.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file where this token was found.
     * @param int                         $stackPtr  The position where the token was found.
     * @param int                         $currScope The current scope opener token.
     *
     * @return void
     */
    protected function processTokenWithinScope(File $phpcsFile, int $stackPtr, int $currScope)
    {
        $tokens = $phpcsFile->getTokens();

        // Determine if this is a function which needs to be examined.
        $conditions = $tokens[$stackPtr]['conditions'];
        end($conditions);
        $deepestScope = key($conditions);
        if ($deepestScope !== $currScope) {
            return;
        }

        $methodName = $phpcsFile->getDeclarationName($stackPtr);
        if ($methodName === '') {
            // Ignore live coding.
            return;
        }

        $properties = $phpcsFile->getMethodProperties($stackPtr);
        if ($properties['scope_specified'] === false) {
            $error = 'Visibility must be declared on method "%s"';
            $data  = [$methodName];
            $fix   = $phpcsFile->addFixableError($error, $stackPtr, 'Missing', $data);

            if ($fix === true) {
                // No scope modifier means the method is public in PHP, fix that
                // to be explicitly public.
                $phpcsFile->fixer->addContentBefore($stackPtr, 'public ');
            }
        }
    }


    /**
     * Processes a token that is found outside the scope that this test is
     * listening to.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file where this token was found.
     * @param int                         $stackPtr  The position in the stack where this
     *                                               token was found.
     *
     * @return void
     */
    protected function processTokenOutsideScope(File $phpcsFile, int $stackPtr)
    {
    }
}
