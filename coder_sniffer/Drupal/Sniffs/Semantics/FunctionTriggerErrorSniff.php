<?php
/**
 * \Drupal\Sniffs\Semanitcs\FunctionTriggerErrorSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Semantics;

use PHP_CodeSniffer\Files\File;

/**
 * Checks that the trigger_error deprecation text message adheres to standards.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class FunctionTriggerErrorSniff extends FunctionCall
{


    /**
     * Returns an array of function names this test wants to listen for.
     *
     * @return array
     */
    public function registerFunctionNames()
    {
        return array('trigger_error');

    }//end registerFunctionNames()


    /**
     * Processes this function call.
     *
     * @param PHP_CodeSniffer_File $phpcsFile    The file being scanned.
     * @param int                  $stackPtr     The position of the function call in
     *                                           the stack.
     * @param int                  $openBracket  The position of the opening
     *                                           parenthesis in the stack.
     * @param int                  $closeBracket The position of the closing
     *                                           parenthesis in the stack.
     *
     * @return void
     */
    public function processFunctionCall(
        file $phpcsFile,
        $stackPtr,
        $openBracket,
        $closeBracket
    ) {

        $tokens = $phpcsFile->getTokens();

        // If no second argument then quit.
        if ($this->getArgument(2) === false) {
            return;
        }

        // Only check deprecation messages.
        if (strcasecmp($tokens[$this->getArgument(2)['start']]['content'], 'E_USER_DEPRECATED') !== 0) {
            return;
        }

        // Get the first argument passed to trigger_error().
        $argument = $this->getArgument(1);

        // Apart from an optional __NAMESPACE__ concatenated at the start of the
        // message, the text should be in one string without any further
        // concatenations. This means that in all cases the 'end' content will
        // contain the message text to be checked.
        $message_text = $tokens[$argument['end']]['content'];

        // The standard format for @trigger_error() is:
        // %thing% is deprecated in %in-version%. %extra-info%. See %cr-link%
        // Use (?U) 'ungreedy' before the version so that only the text up to
        // the first '. ' is matched, as there may be more than one sentence in
        // the extra-info part.
        $matches = array();
        preg_match('/[\'\"](.+) is deprecated in (?U)(.+)\. (.+)\. See (.+)[\'\"]/', $message_text, $matches);

        // There should be 5 items in $matches: 0 is full text, 1 = thing,
        // 2 = in-version, 3 = extra-info, 4 = cr-link.
        if (count($matches) !== 5) {
            $error = "The deprecation message %s does not match the standard format: %%thing%% is deprecated in %%in-version%%. %%extra-info%%. See %%cr-link%%";
            $phpcsFile->addError($error, $argument['start'], 'TriggerErrorTextLayout', array($message_text));
        } else {
            // The text follows the basic layout. Now check the version, which
            // should be Drupal:n.n.n or Project:n.x-n.n. All numbers can be one
            // or two digits.
            $in_version = $matches[2];
            if (preg_match('/^Drupal:\d{1,2}\.\d{1,2}\.\d{1,2}$/', $in_version) === 0
                && preg_match('/^[A-Z][\w]+:\d{1,2}\.x\-\d{1,2}\.\d{1,2}$/', $in_version) === 0
            ) {
                $error = "The deprecation version '%s' does not match the standard: Drupal:n.n.n or Project:n.x-n.n";
                $phpcsFile->addWarning($error, $argument['start'], 'TriggerErrorVersion', array($in_version));
            }

            // Check the 'See' link.
            $cr_link = $matches[4];
            if (preg_match('[^http(s*)://www.drupal.org/node/(\d+)$]', $cr_link) === 0
            ) {
                $error = "The change-record url '%s' does not match the standard: http(s)://www.drupal.org/node/n";
                $phpcsFile->addWarning($error, $argument['start'], 'TriggerErrorSeeUrl', array($cr_link));
            }
        }//end if

    }//end processFunctionCall()


}//end class
