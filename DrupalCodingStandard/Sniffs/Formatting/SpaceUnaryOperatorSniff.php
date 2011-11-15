<?php
/**
 * DrupalCodingStandard_Sniffs_Formatting_SpaceUnaryOperatorSniff.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Peter Philipp <peter.philipp@cando-image.com>
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * DrupalCodingStandard_Sniffs_Formatting_SpaceUnaryOperatorSniff.
 *
 * Ensures there are no spaces on increment / decrement statements.
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Peter Philipp <peter.philipp@cando-image.com>
 * @version   Release: 1.2.2
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_Formatting_SpaceUnaryOperatorSniff implements PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
         return array(T_DEC, T_INC, T_MINUS, T_PLUS);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
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

        // Find the last syntax item to determine if this is an unary operator.
        $lastSyntaxItem = $phpcsFile->findPrevious(
          array(T_WHITESPACE),
          $stackPtr - 1,
          ($tokens[$stackPtr]['column']) * -1,
          true,
          NULL,
          true
        );
        $operator_suffix_allowed = in_array($tokens[$lastSyntaxItem]['code'], array(
          T_LNUMBER,
          T_DNUMBER,
          T_CLOSE_PARENTHESIS,
          T_CLOSE_CURLY_BRACKET,
          T_CLOSE_SQUARE_BRACKET,
          T_VARIABLE,
        ));

        // Check decrement / increment.
        if ($tokens[$stackPtr]['code'] == T_DEC || $tokens[$stackPtr]['code'] == T_INC) {
          $modifyLeft = substr($tokens[($stackPtr - 1)]['content'], 0, 1) == '$' ||
                        $tokens[($stackPtr + 1)]['content'] == ';';

          if ($modifyLeft && $tokens[($stackPtr - 1)]['code'] === T_WHITESPACE) {
            $error = 'There must not be a single space befora a unary opeator statement';
            $phpcsFile->addError($error, $stackPtr);
          }

          if (!$modifyLeft && substr($tokens[($stackPtr + 1)]['content'], 0, 1) != '$') {
            $error = 'An unary opeator statement must not followed by a single space';
            $phpcsFile->addError($error, $stackPtr);
          }
        }

        // Check plus / minus value assignments or comparisons.
        if ($tokens[$stackPtr]['code'] == T_MINUS || $tokens[$stackPtr]['code'] == T_PLUS) {
          if (!$operator_suffix_allowed
            && $tokens[($stackPtr + 1)]['code'] === T_WHITESPACE
          ) {
            $error = 'An unary opeator statement must not followed by a space';
            $phpcsFile->addError($error, $stackPtr);
          }
        }

    }//end process()


}//end class

?>
