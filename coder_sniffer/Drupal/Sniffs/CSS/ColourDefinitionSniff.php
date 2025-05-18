<?php
/**
 * \Drupal\Sniffs\CSS\ColourDefinitionSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\CSS;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\DeprecatedSniff;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * \Drupal\Sniffs\CSS\ColourDefinitionSniff.
 *
 * Ensure colors are defined in lower-case.
 *
 * @deprecated in Coder 8.3.30 and will be removed in Coder 9.0.0. Checking CSS
 *   coding standards is not supported anymore, use Stylelint instead with the
 *   Drupal core .stylelintrc.json configuration file.
 * @see        https://git.drupalcode.org/project/drupal/-/blob/11.x/core/.stylelintrc.json
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class ColourDefinitionSniff implements Sniff, DeprecatedSniff
{

    /**
     * A list of tokenizers this sniff supports.
     *
     * @var array<string>
     */
    public $supportedTokenizers = ['CSS'];


    /**
     * Returns the token types that this sniff is interested in.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [T_COLOUR];

    }//end register()


    /**
     * Processes the tokens that this sniff is interested in.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file where the token was found.
     * @param int                         $stackPtr  The position in the stack where
     *                                               the token was found.
     *
     * @return void
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();
        $color  = $tokens[$stackPtr]['content'];

        $expected = strtolower($color);
        if ($color !== $expected) {
            $error = 'CSS colors must be defined in lowercase; expected %s but found %s';
            $data  = [
                $expected,
                $color,
            ];
            $fix   = $phpcsFile->addFixableError($error, $stackPtr, 'NotLower', $data);
            if ($fix === true) {
                $phpcsFile->fixer->replaceToken($stackPtr, $expected);
            }
        }

    }//end process()


    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDeprecationVersion(): string
    {
        return 'Coder 8.3.30';

    }//end getDeprecationVersion()


    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getRemovalVersion(): string
    {
        return 'Coder 9.0.0';

    }//end getRemovalVersion()


    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDeprecationMessage(): string
    {
        return 'Checking CSS coding standards is not supported anymore, use Stylelint instead with the Drupal core .stylelintrc.json configuration file. https://git.drupalcode.org/project/drupal/-/blob/11.x/core/.stylelintrc.json';

    }//end getDeprecationMessage()


}//end class
