<?php
/**
 * Drupal Coding Standard.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @version   CVS: $Id: DrupalCodingStandard.php 269131 2008-11-17 05:04:07Z squiz $
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

if (class_exists('PHP_CodeSniffer_Standards_CodingStandard', true) === false) {
    throw new PHP_CodeSniffer_Exception('Class PHP_CodeSniffer_Standards_CodingStandard not found');
}

/**
 * Drupal Coding Standard.
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @version   Release: 1.2.2
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class PHP_CodeSniffer_Standards_Drupal_DrupalCodingStandard extends PHP_CodeSniffer_Standards_CodingStandard
{

    /**
     * Return a list of external sniffs to include with this standard.
     *
     * The Drupal standard uses some generic sniffs.
     *
     * @return array
     */
    public function getIncludedSniffs()
    {
        return array(
                'Generic/Sniffs/NamingConventions/UpperCaseConstantNameSniff.php',
                'Generic/Sniffs/PHP/UpperCaseConstantSniff.php',
                'Generic/Sniffs/PHP/DisallowShortOpenTagSniff.php',
                'Generic/Sniffs/WhiteSpace/DisallowTabIndentSniff.php',
                'Generic/Sniffs/Functions/OpeningFunctionBraceKernighanRitchieSniff.php',
                'Generic/Sniffs/WhiteSpace/DisallowTabIndentSniff.php',
                'Generic/Sniffs/Formatting/SpaceAfterCastSniff.php',
                'Generic/Sniffs/PHP/NoSilencedErrorsSniff.php',
                'MySource/Sniffs/Debug/DebugCodeSniff.php',
               );

    }//end getIncludedSniffs()


}//end class
?>
