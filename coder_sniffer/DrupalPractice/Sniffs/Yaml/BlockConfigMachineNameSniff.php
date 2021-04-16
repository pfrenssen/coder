<?php

/**
 * \DrupalPractice\Sniffs\Yaml\BlockConfigMachineNameSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace DrupalPractice\Sniffs\Yaml;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;
use Symfony\Component\Yaml\Yaml;
use Symfony\Component\Yaml\Exception\ParseException;

/**
 * Checks that in block.block.*.yml files the machine name of the theme is used.
 *
 * Used as prefix.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class BlockConfigMachineNameSniff implements Sniff
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
     * @return void|int
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $filename = $phpcsFile->getFilename();
        if (!preg_match('/block\.block\.(.*)\.yml$/', $filename)) {
            return ($phpcsFile->numTokens + 1);
        }

        $contents = file_get_contents($filename);
        try {
            $info = Yaml::parse($contents);
        } catch (ParseException $e) {
            // If the YAML is invalid we ignore this file.
            return ($phpcsFile->numTokens + 1);
        }

        if (!isset($info['id']) && !isset($info['theme'])) {
          // If there is no 'id' and 'theme' this can be a config translation,
          // we ignore this file.
          return ($phpcsFile->numTokens + 1);
        }

        $block_id = $info['id'];
        $theme_machine_name = $info['theme'];
        if (!preg_match('/^' . $theme_machine_name . '_/', $block_id)
        ) {
            $warning = 'The machine name of the block should be prefixed by the machine name of the theme it is placed in, separated with an underscore. Example: bartik_help';
            $phpcsFile->addWarning($warning, $stackPtr, 'BlockConfigMachineName');
        }

        // Only run this sniff once per file.
        return ($phpcsFile->numTokens + 1);

    }//end process()


}//end class
