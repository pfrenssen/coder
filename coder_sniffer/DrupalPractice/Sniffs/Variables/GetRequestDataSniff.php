<?php
/**
 * \DrupalPractice\Sniffs\Variables\GetRequestDataSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace DrupalPractice\Sniffs\Variables;

use DrupalPractice\Project;
use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * Ensures that Symfony request object is used to access super globals.
 *
 * Checks the usage of @author tags.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class GetRequestDataSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return [T_VARIABLE];

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token
     *                                               in the stack passed in $tokens.
     *
     * @return void|int
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        if (Project::getCoreVersion($phpcsFile) < 8) {
            // No need to check this file again, mark it as done.
            return ($phpcsFile->numTokens + 1);
        }

        $tokens  = $phpcsFile->getTokens();
        $varName = $tokens[$stackPtr]['content'];
        if ($varName !== '$_REQUEST'
            && $varName !== '$_GET'
            && $varName !== '$_POST'
            && $varName !== '$_FILES'
        ) {
            return;
        }

        // If we get to here, the super global was used incorrectly.
        // First find out how it is being used.
        $usedVar = '';

        $openBracket = $phpcsFile->findNext(T_WHITESPACE, ($stackPtr + 1), null, true);
        if ($tokens[$openBracket]['code'] === T_OPEN_SQUARE_BRACKET) {
            $closeBracket = $tokens[$openBracket]['bracket_closer'];
            $usedVar      = $phpcsFile->getTokensAsString(($openBracket + 1), ($closeBracket - $openBracket - 1));
        }

        $type  = 'SuperglobalAccessed';
        $error = 'The %s super global must not be accessed directly; inject the request.stack service and use $stack->getCurrentRequest()->';
        $data  = [$varName];

        $requestPropertyMap = [
            '$_REQUEST' => 'request',
            '$_GET'     => 'query',
            '$_POST'    => 'request',
            '$_FILES'   => 'files',
        ];

        if ($usedVar !== '') {
            $type  .= 'WithVar';
            $error .= '%s->get(%s)';
            $data[] = $requestPropertyMap[$varName];
            $data[] = $usedVar;
        }

        $error .= ' instead';
        $phpcsFile->addError($error, $stackPtr, $type, $data);

    }//end process()


}//end class
