<?php
/**
 * \Drupal\Sniffs\Strings\UnnecessaryStringConcatSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Strings;

use PHP_CodeSniffer\Sniffs\DeprecatedSniff;
use PHP_CodeSniffer\Standards\Generic\Sniffs\Strings\UnnecessaryStringConcatSniff as GenericUnnecessaryStringConcatSniff;

/**
 * Checks that two strings are not concatenated together; suggests using one string instead.
 *
 * @deprecated in Coder 8.3.30 and will be removed in Coder 9.0.0. Use
 *   Generic.Strings.UnnecessaryStringConcat instead.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class UnnecessaryStringConcatSniff extends GenericUnnecessaryStringConcatSniff implements DeprecatedSniff
{


    /**
     * If true, strings concatenated over multiple lines are allowed.
     *
     * Useful if you break strings over multiple lines to work
     * within a max line length.
     *
     * @var boolean
     */
    public $allowMultiline = true;


        /**
         * {@inheritdoc}
         *
         * @return string
         */
    public function getDeprecationVersion(): string
    {
        return 'Coder 8.3.30';

    }//end getDeprecationVersion()


    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getRemovalVersion(): string
    {
        return 'Coder 9.0.0';

    }//end getRemovalVersion()


    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDeprecationMessage(): string
    {
        return 'The custom UnnecessaryStringConcatSniff is deprecated and will be removed in Coder 9.0.0. Use Generic.Strings.UnnecessaryStringConcat instead.';

    }//end getDeprecationMessage()


}//end class
