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
        return ['trigger_error'];

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

        // Extract the message text to check. If if it formed using sprintf()
        // then find the single overall string using ->findNext.
        if ($tokens[$argument['start']]['code'] === T_STRING
            && strcasecmp($tokens[$argument['start']]['content'], 'sprintf') === 0
        ) {
            $message_position = $phpcsFile->findNext(T_CONSTANT_ENCAPSED_STRING, $argument['start']);
            // Remove the quotes using substr, because trim would take multiple
            // quotes away and possibly not report a faulty message.
            $message_text = substr($tokens[$message_position]['content'], 1, ($tokens[$message_position]['length'] - 2));
        } else {
            // If not sprintf() then extract and store all the items except
            // whitespace, concatenation operators and comma. This will give all
            // real content such as concatenated strings and constants.
            for ($i = $argument['start']; $i <= $argument['end']; $i++) {
                if (in_array($tokens[$i]['code'], [T_WHITESPACE, T_STRING_CONCAT, T_COMMA]) === false) {
                    // For strings, remove the quotes using substr not trim.
                    if ($tokens[$i]['code'] === T_CONSTANT_ENCAPSED_STRING) {
                        $message_parts[] = substr($tokens[$i]['content'], 1, ($tokens[$i]['length'] - 2));
                    } else {
                        $message_parts[] = $tokens[$i]['content'];
                    }
                }
            }

            $message_text = implode(' ', $message_parts);
        }//end if

        // The standard format for @trigger_error() is:
        // %thing% is deprecated in %in-version%. %extra-info%. See %cr-link%
        // Use (?U) 'ungreedy' before the version so that only the text up to
        // the first '. ' is matched, as there may be more than one sentence in
        // the extra-info part.
        $matches = [];
        preg_match('/(.+) is deprecated in (?U)(.+)\. (.+)\. See (.+)$/', $message_text, $matches);

        // There should be 5 items in $matches: 0 is full text, 1 = thing,
        // 2 = in-version, 3 = extra-info, 4 = cr-link.
        if (count($matches) !== 5) {
            $error = "The deprecation message '%s' does not match the standard format: %%thing%% is deprecated in %%in-version%%. %%extra-info%%. See %%cr-link%%";
            $phpcsFile->addError($error, $argument['start'], 'TriggerErrorTextLayout', [$message_text]);
        } else {
            // The text follows the basic layout. Now check that the version
            // matches drupal:n.n.n or project:n.x-n.n. The text must be all
            // lower case and numbers can be one or two digits.
            $in_version = $matches[2];
            if (preg_match('/^drupal:\d{1,2}\.\d{1,2}\.\d{1,2}$/', $in_version) === 0
                && preg_match('/^[a-z\d_]+:\d{1,2}\.x\-\d{1,2}\.\d{1,2}$/', $in_version) === 0
            ) {
                $error = "The deprecation version '%s' does not match the standard: drupal:n.n.n or project:n.x-n.n";
                $phpcsFile->addWarning($error, $argument['start'], 'TriggerErrorVersion', [$in_version]);
            }

            // Check the 'See' link.
            $cr_link = $matches[4];
            // Allow for the alternative 'node' or 'project/aaa/issues' format.
            preg_match('[^http(s*)://www.drupal.org/(node|project/\w+/issues)/(\d+)(\.*)$]', $cr_link, $matches);
            // If matches[4] is not blank it means that the url is correct but it
            // ends with a period. As this can be a common mistake give a specific
            // message to assist in fixing.
            if (isset($matches[4]) === true && empty($matches[4]) === false) {
                $error = "The 'See' url '%s' should not end with a period.";
                $phpcsFile->addWarning($error, $argument['start'], 'TriggerErrorPeriodAfterSeeUrl', [$cr_link]);
            } else if (empty($matches) === true) {
                $error = "The 'See' url '%s' does not match the standard: http(s)://www.drupal.org/node/n or http(s)://www.drupal.org/project/aaa/issues/n";
                $phpcsFile->addWarning($error, $argument['start'], 'TriggerErrorSeeUrlFormat', [$cr_link]);
            }
        }//end if

    }//end processFunctionCall()


}//end class
