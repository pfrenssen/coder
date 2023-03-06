<?php
/**
 * Largely copied from
 * PHP_CodeSniffer\Standards\Squiz\Sniffs\Classes\ClassFileNameSniff.
 *
 * Extended to support anonymous classes and Drupal core version.
 */
namespace Drupal\Sniffs\InfoFiles;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;
use Symfony\Component\Yaml\Yaml;
use Symfony\Component\Yaml\Exception\ParseException;

/**
 * Dependencies should have single entry as an array in Drupal 8 info files.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DependenciesArraySniff implements Sniff
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
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token in the stack passed in $tokens.
     *
     * @return int
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        // Only run this sniff once per info file.
        $fileExtension = strtolower(substr($phpcsFile->getFilename(), -8, 4));
        if ($fileExtension !== 'info') {
            return ($phpcsFile->numTokens + 1);
        }

        try {
            $info = Yaml::parse(file_get_contents($phpcsFile->getFilename()));
        } catch (ParseException $e) {
            // If the YAML is invalid we ignore this file.
            return ($phpcsFile->numTokens + 1);
        }

        if (isset($info['dependencies']) === true && is_array($info['dependencies']) === false) {
            // The $stackPtr will always indicate line 1, but we can get the actual line by
            // searching $tokens to find the dependencies item.
            $tokens = $phpcsFile->getTokens();
            foreach ($tokens as $key => $token) {
                if (preg_match('/dependencies\s*\:/', $token['content']) === 1) {
                    break;
                }
            }

            $error = '"dependencies" should be an array in the info yaml file';
            $phpcsFile->addError($error, $key, 'Dependencies');
        }

        return ($phpcsFile->numTokens + 1);

    }//end process()


}//end class