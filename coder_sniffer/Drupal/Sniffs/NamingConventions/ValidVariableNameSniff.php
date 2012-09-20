<?php
/**
 * Drupal_Sniffs_NamingConventions_ValidVariableNameSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Drupal_Sniffs_NamingConventions_ValidVariableNameSniff.
 *
 * Checks the naming of member variables.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_NamingConventions_ValidVariableNameSniff

    extends PHP_CodeSniffer_Standards_AbstractVariableSniff
{


    /**
     * Processes class member variables.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token
     *                                        in the stack passed in $tokens.
     *
     * @return void
     */
    protected function processMemberVar(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        $memberProps = $phpcsFile->getMemberProperties($stackPtr);
        if (empty($memberProps) === true) {
            return;
        }

        $memberName     = ltrim($tokens[$stackPtr]['content'], '$');
        $isPublic       = ($memberProps['scope'] === 'private') ? false : true;
        $scope          = $memberProps['scope'];
        $scopeSpecified = $memberProps['scope_specified'];

        // Even if it's a private member, it must not have an underscore on the
        // front.
        if ($isPublic === false && $memberName{0} === '_') {
            $error = "Private member variable \"$memberName\" must not be
            	prefixed with an underscore - it is discouraged in PHP 5-specific code";
            $phpcsFile->addError($error, $stackPtr);
            return;
        }

        // If it's not a private member, it must not have an underscore on the front.
        if ($isPublic === true && $scopeSpecified === true && $memberName{0} === '_') {
            $error = ucfirst($scope)." member variable \"$memberName\" must not be
                prefixed with an underscore";
            $phpcsFile->addError($error, $stackPtr);
            return;
        }

        if (strpos($memberName, '_') !== false) {
            $error = 'Class property %s should use lowerCamel naming without underscores';
            $data  = array($tokens[$stackPtr]['content']);
            $phpcsFile->addError($error, $stackPtr, 'LowerCamelName', $data);
        }

    }//end processMemberVar()


    /**
     * Processes normal variables.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file where this token was found.
     * @param int                  $stackPtr  The position where the token was found.
     *
     * @return void
     */
    protected function processVariable(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        $varName = ltrim($tokens[$stackPtr]['content'], '$');

        $phpReservedVars = array(
                            '_SERVER',
                            '_GET',
                            '_POST',
                            '_REQUEST',
                            '_SESSION',
                            '_ENV',
                            '_COOKIE',
                            '_FILES',
                            'GLOBALS',
                           );

        // If it's a php reserved var, then its ok.
        if (in_array($varName, $phpReservedVars) === true) {
            return;
        }

        // If it is a static public variable of a class, then its ok.
        if ($tokens[($stackPtr - 1)]['code'] === T_DOUBLE_COLON) {
            return;
        }

        if (preg_match('/[A-Z]/', $varName)) {
            $error = "Variable \"$varName\" is camel caps format. do not use mixed case (camelCase), use lower case and _";
            $phpcsFile->addError($error, $stackPtr);
        }

    }//end processVariable()


    /**
     * Processes variables in double quoted strings.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file where this token was found.
     * @param int                  $stackPtr  The position where the token was found.
     *
     * @return void
     */
    protected function processVariableInString(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        // We don't care about variables in strings.

    }//end processVariableInString()


}//end class

?>
