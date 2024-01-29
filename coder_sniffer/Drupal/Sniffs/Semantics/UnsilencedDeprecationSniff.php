<?php
/**
 * \Drupal\Sniffs\Semantics\FunctionUnsilencedDeprecationSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Semantics;

use PHP_CodeSniffer\Files\File;

/**
 * Checks that the trigger_error deprecation is silenced by a preceding '@'.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class UnsilencedDeprecationSniff extends FunctionCall
{


    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array<string>
     */
    public function registerFunctionNames()
    {
        return ['trigger_error'];

    }//end registerFunctionNames()


    /**
     * Processes this function call.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile    The file being scanned.
     * @param int                         $stackPtr     The position of the function call in
     *                                                  the stack.
     * @param int                         $openBracket  The position of the opening
     *                                                  parenthesis in the stack.
     * @param int                         $closeBracket The position of the closing
     *                                                  parenthesis in the stack.
     *
     * @return void
     */
    public function processFunctionCall(
        file $phpcsFile,
        $stackPtr,
        $openBracket,
        $closeBracket
    ) {

        $tokens = $phpcsFile->getTokens();

        // If no second argument then quit.
        if ($this->getArgument(2) === false) {
            return;
        }

        // Only check deprecation messages.
        if (strcasecmp($tokens[$this->getArgument(2)['start']]['content'], 'E_USER_DEPRECATED') !== 0) {
            return;
        }

        if ($tokens[($stackPtr - 1)]['type'] !== 'T_ASPERAND') {
            $error = 'All trigger_error calls used for deprecation must be prefixed by an "@"';
            $phpcsFile->addError($error, $stackPtr, 'FunctionUnsilencedDeprecation');
        }

    }//end processFunctionCall()


}//end class
