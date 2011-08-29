<?php
/**
 * DrupalCodingStandard_Sniffs_Whitespace_ScopeIndentSniff.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @version   CVS: $Id: ScopeIndentSniff.php,v 1.12 2008/03/03 02:51:39 squiz Exp $
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

if (class_exists('Generic_Sniffs_WhiteSpace_ScopeIndentSniff', true) === false) {
    $error = 'Class Generic_Sniffs_WhiteSpace_ScopeIndentSniff not found';
    throw new PHP_CodeSniffer_Exception($error);
}

/**
 * DrupalCodingStandard_Sniffs_Whitespace_ScopeIndentSniff.
 *
 * Checks that control structures are structured correctly, and their content
 * is indented correctly.
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @version   Release: 1.2.0RC3
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_WhiteSpace_ScopeIndentSniff extends Generic_Sniffs_WhiteSpace_ScopeIndentSniff
{
  
    /**
     * The number of spaces code should be indented.
     *
     * @var int
     */
    public $indent = 2;

    /**
     * Any scope openers that should not cause an indent.
     *
     * @var array(int)
     */
//    protected $nonIndentingScopes = array(T_SWITCH);
    // Switch *should* cause indent by coding style guide.
    // see http://drupal.org/coding-standards#controlstruct
    protected $nonIndentingScopes = array();

}//end class

?>
