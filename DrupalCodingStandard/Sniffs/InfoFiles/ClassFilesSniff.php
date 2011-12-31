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
class DrupalCodingStandard_Sniffs_InfoFiles_ClassFilesSniff implements PHP_CodeSniffer_MultiFileSniff
{


    /**
     * Called once per script run to allow for processing of this sniff.
     *
     * @param array(PHP_CodeSniffer_File) $files The PHP_CodeSniffer files processed
     *                                           during the script run.
     *
     * @return void
     */
    public function process(array $files)
    {
        foreach ($files as $phpcsFile) {
            // Execute only on *.info files.
            $fileExtension = strtolower(substr($phpcsFile->getFilename(), -4));
            if ($fileExtension === 'info') {
                $info = parse_ini_file($phpcsFile->getFilename());
                if (isset($info['files']) === true && is_array($info['files']) === true) {
                    foreach ($info['files'] as $file) {
                        $fileName = dirname($phpcsFile->getFilename()).'/'.$file;
                        if (file_exists($fileName) === false) {
                            // We need to find the position of the offending line in the
                            // info file.
                            $ptr   = $this->getPtr($file, $phpcsFile);
                            $error = 'Declared file was not found';
                            $phpcsFile->addError($error, $ptr, 'DeclaredFileNotFound');
                            continue;
                        }

                        foreach ($files as $searchFile) {
                            if ($searchFile->getFilename() === $fileName) {
                                $stackPtr = $searchFile->findNext(array(T_CLASS, T_INTERFACE), 0);
                                if ($stackPtr === false) {
                                    $ptr   = $this->getPtr($file, $phpcsFile);
                                    $error = "It's only necessary to declare files[] if they declare a class or interface.";
                                    $phpcsFile->addError($error, $ptr, 'UnecessaryFileDeclaration');
                                }

                                continue 2;
                            }
                        }
                    }//end foreach
                }//end if
            }//end if
        }//end foreach

    }//end process()


    /**
     * Helper function that returns the position of the file name in the info file.
     *
     * @param string               $fileName File name to search for.
     * @param PHP_CodeSniffer_File $infoFile Info file to search in.
     *
     * @return int|false Returns the stack position if the file name is found, false
     * 									 otherwise.
     */
    protected function getPtr($fileName, PHP_CodeSniffer_File $infoFile)
    {
        foreach ($infoFile->getTokens() as $ptr => $tokenInfo) {
            if (strpos($tokenInfo['content'], $fileName) !== false) {
                return $ptr;
            }
        }

        return false;

    }//end getPtr()


}//end class

?>
