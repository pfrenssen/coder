<?php
/**
 * \DrupalPractice\Sniffs\FunctionCalls\InsecureUnserializeSniff
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace DrupalPractice\Sniffs\FunctionCalls;

use PHP_CodeSniffer\Files\File;
use Drupal\Sniffs\Semantics\FunctionCall;

/**
 * Check that unserialize() limits classes that may be unserialized.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class InsecureUnserializeSniff extends FunctionCall
{


    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array<string>
     */
    public function registerFunctionNames()
    {
        return ['unserialize'];

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
          $tokens   = $phpcsFile->getTokens();
          $argument = $this->getArgument(2);
          if (!$argument) {
              $this->fail($phpcsFile, $closeBracket);
              return;
          }

          $allowed_classes_key_start = $phpcsFile->findNext(T_CONSTANT_ENCAPSED_STRING, $argument['start'], $argument['end'], false, '\'allowed_classes\'');
          if (!$allowed_classes_key_start) {
            $allowed_classes_key_start = $phpcsFile->findNext(T_CONSTANT_ENCAPSED_STRING, $argument['start'], $argument['end'], false, '"allowed_classes"');
          }
          if (!$allowed_classes_key_start) {
              $this->fail($phpcsFile, $argument['end']);
              return;
          }
          $allowed_classes_arrow = $phpcsFile->findNext(T_DOUBLE_ARROW, $allowed_classes_key_start, $argument['end'], false);
          if (!$allowed_classes_arrow) {
              $this->fail($phpcsFile, $argument['end']);
              return;
          }

          $allowed_classes_value = $phpcsFile->findNext(T_WHITESPACE, $allowed_classes_arrow + 1, $argument['end'], true);
          if ($tokens[$allowed_classes_value]['code'] === T_TRUE) {
              $this->fail($phpcsFile, $allowed_classes_value);
          }

    }//end processFunctionCall()

    /**
     * Record a violation of the standard.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $position  The stack position of the violation.
     *
     * @return void
     */
    protected function fail($phpcsFile, $position) {
        $phpcsFile->addError('unserialize() is insecure unless allowed classes are limited.', $position, 'InsecureUnserialize');
    }


}//end class
