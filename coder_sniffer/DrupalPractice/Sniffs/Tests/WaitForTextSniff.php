<?php

/**
 * \DrupalPractice\Sniffs\Tests\WaitForText
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace DrupalPractice\Sniffs\Tests;

use PHP_CodeSniffer\Files\File;
use Drupal\Sniffs\Semantics\FunctionCall;

/**
 * Check that waitForText return values are always handled.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class WaitForTextSniff extends FunctionCall
{
    protected $includeMethodCalls = true;

    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array<string>
     */
    public function registerFunctionNames()
    {
        return ['waitForText'];

    }//end registerFunctionNames()


    /**
     * Processes this function call.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int $stackPtr The position of the function call in
     *                                                  the stack.
     * @param int $openBracket The position of the opening
     *                                                  parenthesis in the stack.
     * @param int $closeBracket The position of the closing
     *                                                  parenthesis in the stack.
     *
     * @return void|int
     */
    public function processFunctionCall(
        File $phpcsFile,
             $stackPtr,
             $openBracket,
             $closeBracket
    )
    {
        $tokens = $phpcsFile->getTokens();

        $start = $phpcsFile->findStartOfStatement($stackPtr);
        $end = $phpcsFile->findEndOfStatement($start);
        // We are assigning to a variable, all is well
        if ($tokens[$start]['code'] === T_VARIABLE && $phpcsFile->findNext(T_EQUAL, $start, $end)) {
            return;
        }

        $function = $start - 2;
        if ($tokens[$function]['content'] === 'assertTrue' || $tokens[$function]['content'] === 'assertFalse') {
            return;
        }

        $phpcsFile->addWarning('waitForText functions do not self assert and must be asserted manually', $start, 'WaitForText');

    }//end processFunctionCall()


}//end class
