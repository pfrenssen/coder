<?php
/**
 * \Drupal\Sniffs\NamingConventions\ValidFunctionNameSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\NamingConventions;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Standards\Generic\Sniffs\NamingConventions\CamelCapsFunctionNameSniff;

/**
 * \Drupal\Sniffs\NamingConventions\ValidFunctionNameSniff.
 *
 * Extends
 * \PHP_CodeSniffer\Standards\Generic\Sniffs\NamingConventions\CamelCapsFunctionNameSniff
 * to also check global function names outside the scope of classes.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class ValidFunctionNameSniff extends CamelCapsFunctionNameSniff
{

    /**
     * A list of function prefixes which may not respect naming convention.
     *
     * @var string[]
     */
    protected $allowedFunctionPrefixes = [
        'template_preprocess',
        'theme',
    ];


    /**
     * Processes the tokens outside the scope.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being processed.
     * @param int                         $stackPtr  The position where this token was
     *                                               found.
     *
     * @return void
     */
    protected function processTokenOutsideScope(File $phpcsFile, $stackPtr)
    {
        $functionName = $phpcsFile->getDeclarationName($stackPtr);
        if ($functionName === '') {
            // Ignore closures.
            return;
        }

        $isApiFile     = substr($phpcsFile->getFilename(), -8) === '.api.php';
        $isHookExample = substr($functionName, 0, 5) === 'hook_';
        if ($isApiFile === true && $isHookExample === true) {
            // Ignore for example hook_ENTITY_TYPE_insert() functions in .api.php
            // files.
            return;
        }

        if ($functionName !== strtolower($functionName)) {
            $expected = strtolower(preg_replace('/([^_])([A-Z])/', '$1_$2', $functionName));
            $error    = 'Invalid function name, expected %s but found %s';
            $data     = [
                $expected,
                $functionName,
            ];
            $phpcsFile->addError($error, $stackPtr, 'InvalidName', $data);
        }

        // Validate function names only in *.module files.
        $isModuleFile = substr($phpcsFile->getFilename(), -7) === '.module';
        if ($isModuleFile === true) {
            // Check if the function prefix is allowed to not respect standard.
            foreach ($this->allowedFunctionPrefixes as $allowedFunctionPrefix) {
                if (substr($functionName, 0, strlen($allowedFunctionPrefix)) === $allowedFunctionPrefix) {
                    return;
                }
            }

            $moduleName = substr(basename($phpcsFile->getFilename()), 0, -7);
            if (preg_match("/^_?$moduleName\_.+/", $functionName) === 0) {
                $error = 'All functions defined in a module file must be prefixed with the module\'s name, found "%s" but expected "%s"';
                $data  = [
                    $functionName,
                    $moduleName . '_' . $functionName,
                ];
                $phpcsFile->addError($error, $stackPtr, 'InvalidPrefix', $data);
            }
        }
    }
}
