<?php
/**
 * DrupalCodingStandard_Sniffs_ControlStructures_InlineControlStructureSniff.
 *
 * Verifies that inline control statements are not present.
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @version   Release: 1.3.1
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_ControlStructures_InlineControlStructureSniff extends Generic_Sniffs_ControlStructures_InlineControlStructureSniff
{

    /**
     * A list of tokenizers this sniff supports.
     * Overwriting the Generic Inline Control Structure
     * to only support PHP files.
     *
     * @var array
     */
    public $supportedTokenizers = array(
                                   'PHP',
                                  );
}//end class

?>
