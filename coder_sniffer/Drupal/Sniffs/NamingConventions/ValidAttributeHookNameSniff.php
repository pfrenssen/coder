<?php
/**
 * \Drupal\Sniffs\NamingConventions\ValidAttributeHookNameSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\NamingConventions;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * Checks that Hook attribute argument name not starts with "hook_" prefix.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class ValidAttributeHookNameSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [T_ATTRIBUTE];

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

        if ($tokens[$stackPtr + 1]['type'] === 'T_STRING'
            && $tokens[$stackPtr + 1]['content'] === 'Hook'
            && $tokens[$stackPtr + 3]['type'] === 'T_CONSTANT_ENCAPSED_STRING'
            && str_contains($tokens[$stackPtr + 3]['content'], 'hook_')
        ) {
            $hookName = $tokens[$stackPtr + 3]['content'];
            $phpcsFile->addFixableWarning('Hook name should not start with "hook_" prefix. Hook name used:' . $hookName, $stackPtr + 3,'AttributePrefixHookName');
            $phpcsFile->fixer->replaceToken($stackPtr + 3, str_replace('hook_', '', $hookName));
        }

    }//end process()


}//end class
