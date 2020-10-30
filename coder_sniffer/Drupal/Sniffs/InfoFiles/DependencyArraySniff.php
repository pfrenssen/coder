<?php

namespace Drupal\Sniffs\InfoFiles;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;
use Symfony\Component\Yaml\Yaml;

/**
 * Dependency should have single entry as an array in Drupal 8 info files.
 *
 * @category PHP
 * @package PHP_CodeSniffer
 * @link http://pear.php.net/package/PHP_CodeSniffer
 */
class DependencyArraySniff implements Sniff {

  /**
   * Returns an array of tokens this test wants to listen for.
   *
   * @return arrayint|string
   *   Returns array or string.
   */
  public function register() {
    return [T_INLINE_HTML];

  }//end register()

  /**
   * Processes this test, when one of its tokens is encountered.
   *
   * @param \PHP_CodeSniffer\Files\File $phpcsFile
   *   The file being scanned.
   * @param int $stackPtr
   *   The position of the current token in the
   *   stack passed in $tokens.
   *
   * @return int
   *   Returns integer.
   */
  public function process(File $phpcsFile, $stackPtr) {
    // Only run this sniff once per info file.
    $fileExtension = strtolower(substr($phpcsFile->getFilename(), -8, 4));
    if ($fileExtension !== 'info') {
      return ($phpcsFile->numTokens + 1);
    }

    $info = Yaml::parse(file_get_contents($phpcsFile->getFilename()));
    if (isset($info['dependencies']) === TRUE) {
      if (!is_array($info['dependencies'])) {
        $error = '"dependencies" should be an array in the info yaml file';
        $phpcsFile->addError($error, $stackPtr, 'Dependencies');
      }
    }

    return ($phpcsFile->numTokens + 1);

  }//End process()

}//End class
