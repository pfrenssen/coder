<?php
/**
 * Drupal_Sniffs_Files_LineLengthSniff.
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

if (class_exists('Generic_Sniffs_Files_LineLengthSniff', true) === false) {
    throw new PHP_CodeSniffer_Exception('Class Generic_Sniffs_Files_LineLengthSniff not found');
}

/**
 * Drupal_Sniffs_Files_LineLengthSniff.
 *
 * Checks all lines in the file, and throws warnings if they are over 80
 * characters in length.
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
class Drupal_Sniffs_Files_LineLengthSniff extends Generic_Sniffs_Files_LineLengthSniff
{

    /**
     * The limit that the length of a line should not exceed.
     *
     * @var int
     */
    public $lineLimit = 80;

    /**
     * The limit that the length of a line must not exceed.
     * But just check the line length of comments....
     *
     * Set to zero (0) to disable.
     *
     * @var int
     */
    public $absoluteLineLimit = 0;

    /**
    * Checks if a line is too long.
    *
    * @param PHP_CodeSniffer_File $phpcsFile   The file being scanned.
    * @param int                  $stackPtr    The token at the end of the line.
    * @param string               $lineContent The content of the line.
    *
    * @return void
    */
    protected function checkLineLength(PHP_CodeSniffer_File $phpcsFile, $stackPtr, $lineContent)
    {
      $tokens = $phpcsFile->getTokens();
      if ($tokens[$stackPtr]['code'] == T_DOC_COMMENT || $tokens[$stackPtr]['code'] == T_COMMENT) {
        return parent::checkLineLength($phpcsFile, $stackPtr, $lineContent);
      }
    }//end checkLineLength()

    /**
     * Returns the length of a defined line.
     *
     * @return integer
     */
    public function getLineLength(PHP_CodeSniffer_File $phpcsFile, $currentLine) {
        $tokens = $phpcsFile->getTokens();

        $tokenCount         = 0;
        $currentLineContent = '';

        $trim = (strlen($phpcsFile->eolChar) * -1);
        for (; $tokenCount < $phpcsFile->numTokens; $tokenCount++) {
          if ($tokens[$tokenCount]['line'] === $currentLine) {
            $currentLineContent .= $tokens[$tokenCount]['content'];
          }
        }

        return strlen($currentLineContent);
    }

}//end class

?>
