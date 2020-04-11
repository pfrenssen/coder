<?php
/**
 * \Drupal\Sniffs\Classes\InterfaceImplementedSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Classes;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * Checks that class or interface name does not identical with the filename.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class ClassOrInterfaceNameNotIdenticalWithFilenameSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [
            T_CLASS,
            T_INTERFACE,
        ];

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token in
     *                                               the stack passed in $tokens.
     *
     * @return void
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $tokens    = $phpcsFile->getTokens();
        $pathInfo  = pathinfo($phpcsFile->getFilename());
        $filename  = $pathInfo['filename'];
        $nameIndex = $phpcsFile->findNext(T_STRING, $stackPtr);
        if ($nameIndex === false) {
            return;
        }

        $classOrInterfaceName = $tokens[$nameIndex]['content'];
        if ($filename !== $classOrInterfaceName && strtolower($filename) === strtolower($classOrInterfaceName)) {
            $type = $tokens[$stackPtr]['type'];
            if ($type === 'T_CLASS') {
                $phpcsFile->addFixableWarning("Class name '$classOrInterfaceName' is not identical with its filename '$filename' ", $nameIndex, 'ClassOrInterfaceNameNotIdenticalWithFilename');
            } else {
                $phpcsFile->addFixableWarning("Interface name '$classOrInterfaceName' is not identical with its filename '$filename' ", $nameIndex, 'ClassOrInterfaceNameNotIdenticalWithFilename');
            }

            if ($phpcsFile->fixer !== null) {
                $phpcsFile->fixer->replaceToken($nameIndex, $filename);
            }
        }

    }//end process()


}//end class
