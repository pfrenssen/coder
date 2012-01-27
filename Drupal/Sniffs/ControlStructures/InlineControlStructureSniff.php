<?php
/**
 * Drupal_Sniffs_ControlStructures_InlineControlStructureSniff.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Drupal_Sniffs_ControlStructures_InlineControlStructureSniff.
 *
 * Verifies that inline control statements are not present. This Sniff overides
 * the generic sniff because Drupal template files may use the alternative
 * syntax for control structures. See
 * http://www.php.net/manual/en/control-structures.alternative-syntax.php
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_ControlStructures_InlineControlStructureSniff
extends Generic_Sniffs_ControlStructures_InlineControlStructureSniff
{

    /**
     * A list of tokenizers this sniff supports.
     * Overwriting the Generic Inline Control Structure
     * to only support PHP files.
     *
     * @var array
     */
    public $supportedTokenizers = array('PHP');


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

        // Check for the alternate syntax for control structures with colons (:).
        if (isset($tokens[$stackPtr]['parenthesis_closer']) === true) {
            $colonPtr = $tokens[$stackPtr]['parenthesis_closer'] + 1;
        } else {
            $colonPtr = $stackPtr + 1;
        }

        $colon = false;
        while ($tokens[$colonPtr]['code'] === T_WHITESPACE
            || $tokens[$colonPtr]['code'] === T_COLON) {
            if ($tokens[$colonPtr]['code'] === T_COLON) {
                $colon = true;
                break;
            }

            $colonPtr++;
        }

        if (isset($tokens[$stackPtr]['scope_opener']) === false && $colon === false) {
            // Ignore the ELSE in ELSE IF. We'll process the IF part later.
            if (($tokens[$stackPtr]['code'] === T_ELSE) && ($tokens[($stackPtr + 2)]['code'] === T_IF)) {
                return;
            }

            if ($tokens[$stackPtr]['code'] === T_WHILE) {
                // This could be from a DO WHILE, which doesn't have an opening brace.
                $lastContent = $phpcsFile->findPrevious(T_WHITESPACE, ($stackPtr - 1), null, true);
                if ($tokens[$lastContent]['code'] === T_CLOSE_CURLY_BRACKET) {
                    $brace = $tokens[$lastContent];
                    if (isset($brace['scope_condition']) === true) {
                        $condition = $tokens[$brace['scope_condition']];
                        if ($condition['code'] === T_DO) {
                            return;
                        }
                    }
                }
            }

            // This is a control structure without an opening brace,
            // so it is an inline statement.
            if ($this->error === true) {
                $phpcsFile->addError('Inline control structures are not allowed', $stackPtr, 'NotAllowed');
            } else {
                $phpcsFile->addWarning('Inline control structures are discouraged', $stackPtr, 'Discouraged');
            }

            return;
        }//end if

    }//end process()


}//end class

?>
