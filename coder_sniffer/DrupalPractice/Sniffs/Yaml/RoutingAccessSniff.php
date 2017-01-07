<?php

/**
 * DrupalPractice_Sniffs_Yaml_RoutingAccessSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that there are no undocumented open access callbacks in *.routing.yml files.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalPractice_Sniffs_Yaml_RoutingAccessSniff implements PHP_CodeSniffer_Sniff
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
     * @param PHP_CodeSniffer_File $phpcsFile The current file being processed.
     * @param int                  $stackPtr  The position of the current token
     *                                        in the stack passed in $tokens.
     *
     * @return int
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();
        $end    = (count($tokens) + 1);

        $fileExtension = strtolower(substr($phpcsFile->getFilename(), -12));
        if ($fileExtension !== '.routing.yml') {
            return $end;
        }

        if (preg_match('/^[\s]+_access: \'TRUE\'/', $tokens[$stackPtr]['content']) === 1
            && isset($tokens[($stackPtr - 1)]) === true
            && preg_match('/^[\s]*#/', $tokens[($stackPtr - 1)]['content']) === 0
        ) {
            $warning = 'Open page callback found, please add a comment before the line why there is no access restriction';
            $phpcsFile->addWarning($warning, $stackPtr, 'OpenCallback');
        }

    }//end process()


}//end class
