<?php
/**
 * DrupalCodingStandard_Sniffs_NamingConventions_ValidVariableNameSniff.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Serge Shirokov <bolter.fire@gmail.com>
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

if (class_exists('PHP_CodeSniffer_Standards_AbstractVariableSniff', true) === false) {
    $error = 'Class PHP_CodeSniffer_Standards_AbstractVariableSniff not found';
    throw new PHP_CodeSniffer_Exception($error);
}

/**
 * DrupalCodingStandard_Sniffs_NamingConventions_ValidVariableNameSniff.
 *
 * Checks the naming of member variables.
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Serge Shirokov <bolter.fire@gmail.com>
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @version   Release: 1.2.0RC3
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_NamingConventions_ValidVariableNameSniff

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

        // If it's a private member, it must have an underscore on the front.
        /*if ($isPublic === false && $memberName{0} !== '_') {
            $error = "Private member variable \"$memberName\" must be
                prefixed with an underscore";
            $phpcsFile->addError($error, $stackPtr);
            return;
        }*/

        // Even if it's a private member, it must have an underscore on the front.
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

      $varName     = ltrim($tokens[$stackPtr]['content'], '$');

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

      if (preg_match('/[A-Z]/', $varName) && PHP_CodeSniffer::isCamelCaps($varName, false, true, false) == true) {
        $error = "Variable \"$varName\" is camel caps format. do not use mixed case (camelCase), use lower case and _";
        $phpcsFile->addError($error, $stackPtr);
      }

        // Strange error with $stackPtr prevents us from using this code.
        /* $tokens = $phpcsFile->getTokens();

        $stst = $stackPtr;

        $memberProps = $phpcsFile->getMemberProperties($stst);
        if (empty($memberProps) === true) {
            return;
        }

        $memberName     = ltrim($tokens[$stackPtr]['content'], '$');
        $isGlobal       = ($memberProps['scope'] === 'global') ? false : true;

        if ($isGlobal && $memberName{0} != '_') {
            $error = "Global variable \"$memberName\" must be prefixed
                with an underscore before module name";
            $phpcsFile->addError($error, $stackPtr);
            return;
        }  */
        // We don't care about normal variables.
        return;

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
        // We don't care about normal variables.
        return;

    }//end processVariableInString()


}//end class

?>
