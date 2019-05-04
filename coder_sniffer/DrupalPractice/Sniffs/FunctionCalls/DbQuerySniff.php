<?php
/**
 * \DrupalPractice\Sniffs\FunctionCalls\DbQuerySniff
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace DrupalPractice\Sniffs\FunctionCalls;

use PHP_CodeSniffer\Files\File;
use Drupal\Sniffs\Semantics\FunctionCall;
use DrupalPractice\Project;

/**
 * Check that UPDATE/DELETE queries are not used in db_query() in Drupal 7.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DbQuerySniff extends FunctionCall
{


    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array
     */
    public function registerFunctionNames()
    {
        return ['db_query'];

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
        File $phpcsFile,
        $stackPtr,
        $openBracket,
        $closeBracket
    ) {
        // This check only applies to Drupal 7, not Drupal 6.
        if (Project::getCoreVersion($phpcsFile) !== '7.x') {
            return;
        }

        $tokens   = $phpcsFile->getTokens();
        $argument = $this->getArgument(1);

        $query_start = '';
        for ($start = $argument['start']; $tokens[$start]['code'] === T_CONSTANT_ENCAPSED_STRING && empty($query_start) === true; $start++) {
            // Remove quote and white space from the beginning.
            $query_start = trim(substr($tokens[$start]['content'], 1));
            // Just look at the first word.
            $parts       = explode(' ', $query_start);
            $query_start = $parts[0];

            if (in_array(strtoupper($query_start), ['INSERT', 'UPDATE', 'DELETE', 'TRUNCATE']) === true) {
                $warning = 'Do not use %s queries with db_query(), use %s instead';
                $phpcsFile->addWarning($warning, $start, 'DbQuery', [$query_start, 'db_'.strtolower($query_start).'()']);
            }
        }

    }//end processFunctionCall()


}//end class
