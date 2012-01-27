<?php
/**
 * Drupal_Sniffs_Array_ArraySniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Drupal_Sniffs_Array_ArraySniff.
 *
 * Checks if the array's are styled in the Drupal way.
 * - Comma after the last array element
 * - Indentation is 2 spaces for multi line array definitions
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Array_ArraySniff implements PHP_CodeSniffer_Sniff
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
        if ($tokens[$lastItem]['code'] !== T_COMMA && !$isInlineArray && $tokens[$lastItem + 1]['code'] !== T_CLOSE_PARENTHESIS) {
            $phpcsFile->addWarning('A comma should follow the last multiline array item. Found: '.$tokens[$lastItem]['content'], $lastItem);
            return;
        }

        if ($tokens[$lastItem]['code'] === T_COMMA && $isInlineArray) {
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

        // Only continue for multi line arrays.
        if ($isInlineArray === true) {
            return;
        }

        // Special case: Opening two multi line structures in one line is ugly.
        if (isset($tokens[$stackPtr]['nested_parenthesis']) === true) {
            end($tokens[$stackPtr]['nested_parenthesis']);
            $outerNesting = key($tokens[$stackPtr]['nested_parenthesis']);
            if ($tokens[$outerNesting]['line'] === $tokens[$stackPtr]['line']) {
                // We could throw a warning here that the start of the array
                // definition should be on a new line by itself, but we just ignore
                // it for now as this is not defined as standard.
                return;
            }
        }

        // Find the first token on this line.
        $firstLineToken = $stackPtr;
        for ($i = $stackPtr; $i >= 0; $i--) {
            // Record the first code token on the line.
            if (in_array($tokens[$i]['code'], PHP_CodeSniffer_Tokens::$emptyTokens) === false) {
                $firstLineToken = $i;
            }

            // It's the start of the line, so we've found our first php token.
            if ($tokens[$i]['column'] === 1) {
                break;
            }
        }

        $lineStart = $stackPtr;
        // Iterate over all lines of this array.
        while ($lineStart < $tokens[$stackPtr]['parenthesis_closer']) {
            // Find next line start.
            $newLineStart = $lineStart;
            while ($tokens[$newLineStart]['line'] == $tokens[$lineStart]['line']) {
                $newLineStart = $phpcsFile->findNext(
                    PHP_CodeSniffer_Tokens::$emptyTokens,
                    $newLineStart + 1,
                    $tokens[$stackPtr]['parenthesis_closer'] + 1,
                    true
                );
                if ($newLineStart === false) {
                    break 2;
                }
            }

            if ($newLineStart === $tokens[$stackPtr]['parenthesis_closer']) {
                // End of the array reached.
                if ($tokens[$newLineStart]['column'] !== $tokens[$firstLineToken]['column']) {
                    $error = 'Array closing indentation error, expected %s spaces but found %s';
                    $data  = array(
                              $tokens[$firstLineToken]['column'] - 1,
                              $tokens[$newLineStart]['column'] - 1,
                             );
                    $phpcsFile->addError($error, $newLineStart, 'ArrayClosingIndentation', $data);
                }

                break;
            }

            // Skip lines in nested structures.
            $innerNesting = end($tokens[$newLineStart]['nested_parenthesis']);
            if ($innerNesting === $tokens[$stackPtr]['parenthesis_closer']
                && $tokens[$newLineStart]['column'] !== ($tokens[$firstLineToken]['column'] + 2)
            ) {
                $error = 'Array indentation error, expected %s spaces but found %s';
                $data  = array(
                          $tokens[$firstLineToken]['column'] + 1,
                          $tokens[$newLineStart]['column'] - 1,
                         );
                $phpcsFile->addError($error, $newLineStart, 'ArrayIndentation', $data);
            }

            $lineStart = $newLineStart;
        }//end while

    }//end process()


}//end class

?>
