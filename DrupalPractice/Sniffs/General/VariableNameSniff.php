<?php
/**
 * DrupalPractice_Sniffs_General_VariableNameSniff
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Check the usage of the t() function to not escape translateable strings with back
 * slashes. Also checks that the first argument does not use string concatenation.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalPractice_Sniffs_General_VariableNameSniff extends Drupal_Sniffs_Semantics_FunctionCall
{


    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array
     */
    public function registerFunctionNames()
    {
        return array('variable_get');

    }//end registerFunctionNames()


    /**
     * Processes this function call.
     *
     * @param PHP_CodeSniffer_File $phpcsFile
     *   The file being scanned.
     * @param int $stackPtr
     *   The position of the function call in the stack.
     * @param int $openBracket
     *   The position of the opening parenthesis in the stack.
     * @param int $closeBracket
     *   The position of the closing parenthesis in the stack.
     * @param Drupal_Sniffs_Semantics_FunctionCallSniff $sniff
     *   Can be used to retreive the function's arguments with the getArgument()
     *   method.
     *
     * @return void
     */
    public function processFunctionCall(
        PHP_CodeSniffer_File $phpcsFile,
        $stackPtr,
        $openBracket,
        $closeBracket,
        Drupal_Sniffs_Semantics_FunctionCallSniff $sniff
    ) {
        $tokens = $phpcsFile->getTokens();
        
        // We assume that the sequence '#default_value' => variable_get(...)
        // indicates a variable that the module owns.
        $arrow = $phpcsFile->findPrevious(PHP_CodeSniffer_Tokens::$emptyTokens, ($stackPtr - 1), null, true);
        if ($arrow === false || $tokens[$arrow]['code'] !== T_DOUBLE_ARROW) {
            return;
        }
        $arrayKey = $phpcsFile->findPrevious(PHP_CodeSniffer_Tokens::$emptyTokens, ($arrow - 1), null, true);
        if ($arrayKey === false
            || $tokens[$arrayKey]['code'] !== T_CONSTANT_ENCAPSED_STRING
            || substr($tokens[$arrayKey]['content'], 1, -1) !== '#default_value'
        ) {
            return;
        }

        $argument = $sniff->getArgument(1);

        if ($argument === false || $tokens[$argument['start']]['code'] !== T_CONSTANT_ENCAPSED_STRING) {
            return;
        }

        $moduleName = DrupalPractice_Project::getName($phpcsFile);
        print_r($moduleName);
        if ($moduleName === false) {
            return;
        }
        $variableName = substr($tokens[$argument['start']]['content'], 1, -1);

    }//end processFunctionCall()


}//end class
