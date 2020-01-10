<?php

/**
 * \DrupalPractice\Sniffs\InfoFiles\CoreVersionRequirementSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace DrupalPractice\Sniffs\InfoFiles;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;
use Symfony\Component\Yaml\Yaml;
use Symfony\Component\Yaml\Exception\ParseException;

/**
 * Checks if the *.info.yml file contains core_version_requirement.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class CoreVersionRequirementSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [T_INLINE_HTML];

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The current file being processed.
     * @param int                         $stackPtr  The position of the current token
     *                                               in the stack passed in $tokens.
     *
     * @return int
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $fileExtension = strtolower(substr($phpcsFile->getFilename(), -9));
        if ($fileExtension !== '.info.yml') {
            return ($phpcsFile->numTokens + 1);
        }

        $contents = file_get_contents($phpcsFile->getFilename());
        try {
            $info = Yaml::parse($contents);
        } catch (ParseException $e) {
            // If the YAML is invalid we ignore this file.
            return ($phpcsFile->numTokens + 1);
        }

        // Test modules can omit the core_version_requirement key.
        if (isset($info['package']) === true && $info['package'] === 'Testing') {
            return ($phpcsFile->numTokens + 1);
        }

        if (isset($info['core_version_requirement']) === false) {
            $warning = '"core_version_requirement" property is missing in the info.yml file';
            $phpcsFile->addWarning($warning, $stackPtr, 'CoreVersionRequirement');
        }

        return ($phpcsFile->numTokens + 1);

    }//end process()


}//end class
