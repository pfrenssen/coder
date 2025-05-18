<?php
/**
 * \Drupal\Sniffs\Strings\UnnecessaryStringConcatSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Strings;

use PHP_CodeSniffer\Standards\Generic\Sniffs\Strings\UnnecessaryStringConcatSniff as GenericUnnecessaryStringConcatSniff;

/**
 * Checks that two strings are not concatenated together; suggests using one string instead.
 *
 * @todo Remove in Coder 9.0.0 and replace with GenericUnnecessaryStringConcatSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class UnnecessaryStringConcatSniff extends GenericUnnecessaryStringConcatSniff
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


}//end class
