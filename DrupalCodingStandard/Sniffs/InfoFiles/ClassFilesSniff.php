<?php
/**
 * DrupalCodingStandard_Sniffs_InfoFiles_ClassFilesSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks files[] entries in info files. Only files containing classes/interfaces
 * should be listed.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_InfoFiles_ClassFilesSniff implements PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_INLINE_HTML);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token
     *                                        in the stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $fileExtension = strtolower(substr($phpcsFile->getFilename(), -4));
        // Execute only on *.info files and only once.
        if ($fileExtension === 'info' && $stackPtr === 0) {
            $tokens = $phpcsFile->getTokens();
            // @todo: make use of parse_ini_string() (since PHP 5.3) as soon as
            // PHP 5.2 is really dead.
            $info = parse_ini_file($phpcsFile->getFilename());
            if (isset($info['files']) === true && is_array($info['files']) === true) {
                foreach ($info['files'] as $file) {
                    $fileName = dirname($phpcsFile->getFilename()).'/'.$file;
                    if (file_exists($fileName) === true) {
                        $contents = file_get_contents($fileName);
                    } else {
                        $contents = '';
                    }

                    // Tokenize the referenced file to check if it contains a
                    // class/interface token.
                    $fileTokens = token_get_all($contents);
                    $found      = false;
                    foreach ($fileTokens as $token) {
                        if (isset($token[0]) === true && ($token[0] === T_CLASS || $token[0] === T_INTERFACE)) {
                            $found = true;
                            break;
                        }
                    }

                    if ($found === false) {
                        // We need to find the position of the offending line in the
                        // info file.
                        foreach ($tokens as $ptr => $tokenInfo) {
                            if (strpos($tokenInfo['content'], $file) !== false) {
                                break;
                            }
                        }

                        $error = "It's only necessary to declare files[] if they declare a class or interface.";
                        $phpcsFile->addError($error, $ptr, 'UnecessaryFileDeclaration');
                    }
                }//end foreach
            }//end if
        }//end if

    }//end process()


}//end class

?>
