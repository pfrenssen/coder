<?php
/**
 * Drupal_Sniffs_Formatting_SpaceOperatorSniff.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Peter Philipp <peter.philipp@cando-image.com>
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Drupal_Sniffs_Formatting_SpaceOperatorSniff.
 *
 * Ensures there is a single space after a operator
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Peter Philipp <peter.philipp@cando-image.com>
 * @version   Release: 1.2.2
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Formatting_SpaceOperatorSniff implements PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        // Do not examine "=>" assignment, this is handled by other sniffs.
        $assignments = array_diff(PHP_CodeSniffer_Tokens::$assignmentTokens, array(T_DOUBLE_ARROW));
        $tokens = array_merge(
             $assignments,
             PHP_CodeSniffer_Tokens::$equalityTokens,
             PHP_CodeSniffer_Tokens::$comparisonTokens,
             PHP_CodeSniffer_Tokens::$arithmeticTokens,
             array(T_INLINE_THEN)
         );

         return $tokens;

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * Operations to check for:
     * $i = 1 + 1;
     * $i = $i + 1;
     * $i = (1 + 1) - 1;
     *
     * Operations to ignore:
     * array($i => -1);
     * $i = -1;
     * range(-10, -1);
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token in
     *                                        the stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        $has_equality_token = in_array($tokens[$stackPtr - 2]['code'], PHP_CodeSniffer_Tokens::$equalityTokens);

        // Ensure this is a construct to check.
        $lastSyntaxItem = $phpcsFile->findPrevious(
          array(T_WHITESPACE),
          $stackPtr - 1,
          ($tokens[$stackPtr]['column']) * -1,
          true,
          NULL,
          true
        );

        $needs_operator_suffix = in_array($tokens[$lastSyntaxItem]['code'], array(
          T_LNUMBER,
          T_DNUMBER,
          T_CLOSE_PARENTHESIS,
          T_CLOSE_SQUARE_BRACKET,
          T_CLOSE_CURLY_BRACKET,
          T_VARIABLE,
          T_STRING,
          T_CONSTANT_ENCAPSED_STRING,
        ));
        $needs_operator_prefix = !in_array($tokens[$lastSyntaxItem]['code'], array(
          T_OPEN_PARENTHESIS,
          T_EQUAL,
        ));

        if ($needs_operator_suffix && ($tokens[$stackPtr - 2]['code'] !== T_EQUAL
            && $tokens[$stackPtr - 2]['code'] !== T_DOUBLE_ARROW
            && !$has_equality_token
            // Allow "=&" without a space in between.
            && !($tokens[$stackPtr]['code'] === T_EQUAL && $tokens[($stackPtr + 1)]['code'] === T_BITWISE_AND)
            && ($tokens[($stackPtr + 1)]['code'] !== T_WHITESPACE
            || $tokens[($stackPtr + 1)]['content'] != ' '))
        ) {
            $error = 'An operator statement must be followed by a single space';
            $phpcsFile->addError($error, $stackPtr);
        }

        if ($needs_operator_prefix) {
            $error = false;
            if ($tokens[($stackPtr - 1)]['code'] !== T_WHITESPACE) {
                $error = true;
            } else if ($tokens[($stackPtr - 1)]['content'] !== ' '
                && $tokens[$stackPtr]['code'] !== T_EQUAL
            ) {
                $nonWhiteSpace = $phpcsFile->findPrevious(PHP_CodeSniffer_Tokens::$emptyTokens, $stackPtr - 1, null, true);
                // Make sure that the previous operand is on the same line before
                // throwing an error.
                if ($tokens[$nonWhiteSpace]['line'] === $tokens[$stackPtr]['line']) {
                    $error = true;
                }
            }

            if ($error === true) {
                $error = 'There must be a single space before an operator statement';
                $phpcsFile->addError($error, $stackPtr);
            }
        }//end if

    }//end process()


}//end class

?>
