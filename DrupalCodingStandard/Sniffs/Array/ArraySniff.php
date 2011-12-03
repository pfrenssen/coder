<?php
/**
 * DrupalCodingStandard_Sniffs_Array_ArraySniff.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Peter Philipp <peter.philipp@cando-image.com>
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * DrupalCodingStandard_Sniffs_Array_ArraySniff.
 *
 * Checks if the array's are styled in the Drupal way.
 * - Comma after the last array element
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Peter Philipp <peter.philipp@cando-image.com>
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_Array_ArraySniff implements PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_ARRAY);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token in the
     *                                        stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
      $tokens = $phpcsFile->getTokens();
      $lastItem = $phpcsFile->findPrevious(
          PHP_CodeSniffer_Tokens::$emptyTokens,
          $tokens[$stackPtr]['parenthesis_closer']-1,
          $stackPtr,
          true
      );

      // Empty array.
      if ($lastItem == $tokens[$stackPtr]['parenthesis_opener'] ) {
          return;
      }
      // Inline array.
      $isInlineArray = $tokens[$tokens[$stackPtr]['parenthesis_opener']]['line'] == $tokens[$tokens[$stackPtr]['parenthesis_closer']]['line'];

      // Check if the last item in a multiline array has a "closing" comma.
      if ($tokens[$lastItem]['code'] !== T_COMMA && !$isInlineArray && $tokens[$lastItem+1]['code'] !== T_CLOSE_PARENTHESIS) {
          $phpcsFile->addWarning('A comma should follow the last multiline array item. Found: ' . $tokens[$lastItem]['content'], $lastItem);
          return;
      }

      if ($tokens[$lastItem]['code'] === T_COMMA && $isInlineArray){
          $phpcsFile->addWarning('Last item of an inline array must not be followed by a comma', $lastItem);
      }

      $firstToken = $tokens[$stackPtr]['parenthesis_opener'] + 1;
      if ($isInlineArray && $tokens[$firstToken]['code'] === T_WHITESPACE) {
          $phpcsFile->addWarning('The opening paranthesis of an array should not be followed by a white space', $firstToken);
      }

      $lastToken = $tokens[$stackPtr]['parenthesis_closer'] - 1;
      if ($isInlineArray && $tokens[$lastToken]['code'] === T_WHITESPACE) {
          $phpcsFile->addWarning('The closing paranthesis of an array should not be preceded by a white space', $lastToken);
      }

    }//end process()

}//end class

?>
