<?php
/**
 * DrupalPractice_Sniffs_Objects_GlobalDrupalSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

use Symfony\Component\Yaml\Yaml;

/**
 * Checks that \Drupal::service() and friends is not used in forms, controllers, services.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalPractice_Sniffs_Objects_GlobalDrupalSniff implements PHP_CodeSniffer_Sniff
{

    /**
     * List of base classes where \Drupal should not be used in an extending class.
     *
     * @var string[]
     */
    public static $baseClasses = array(
                                  'BlockBase',
                                  'ConfigFormBase',
                                  'ContentEntityForm',
                                  'ControllerBase',
                                  'FormBase',
                                  'EntityForm',
                                  'WidgetBase',
                                 );


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_STRING);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token
     *                                         in the stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // We are only interested in Drupal:: static method calls, not in the global
        // scope.
        if ($tokens[$stackPtr]['content'] !== 'Drupal'
            || $tokens[($stackPtr + 1)]['code'] !== T_DOUBLE_COLON
            || isset($tokens[($stackPtr + 2)]) === false
            || $tokens[($stackPtr + 2)]['code'] !== T_STRING
            || isset($tokens[($stackPtr + 3)]) === false
            || $tokens[($stackPtr + 3)]['code'] !== T_OPEN_PARENTHESIS
            || empty($tokens[$stackPtr]['conditions']) === true
        ) {
            return;
        }

        // Check that this statement is not in a static function.
        foreach ($tokens[$stackPtr]['conditions'] as $conditionPtr => $conditionCode) {
            if ($conditionCode === T_FUNCTION && $phpcsFile->getMethodProperties($conditionPtr)['is_static'] === true) {
                return;
            }
        }

        // Check if the class extends another class and get the name of the class
        // that is extended.
        $classPtr    = key($tokens[$stackPtr]['conditions']);
        $extendsName = $phpcsFile->findExtendedClassName($classPtr);

        if (($extendsName === false || in_array($extendsName, static::$baseClasses) === false)
            && $this->isServiceClass($phpcsFile, $classPtr) === false
        ) {
            return;
        }

        $warning = '\Drupal calls should be avoided in classes, use dependency injection instead';
        $phpcsFile->addWarning($warning, $stackPtr, 'GlobalDrupal');

    }//end process()


    /**
     * Return true if the given class is a Drupal service registered in *.services.yml.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $classPtr  The position of the class declaration
     *                                        in the token stack.
     *
     * @return bool
     */
    private function isServiceClass(PHP_CodeSniffer_File $phpcsFile, $classPtr)
    {
        // Cache the information per file as this might get called often.
        static $cache;

        if (isset($cache[$phpcsFile->getFilename()]) === true) {
            return $cache[$phpcsFile->getFilename()];
        }

        // Get the namespace of the class if there is one.
        $namespacePtr = $phpcsFile->findPrevious(T_NAMESPACE, ($classPtr - 1));
        if ($namespacePtr === false) {
            $cache[$phpcsFile->getFilename()] = false;
            return false;
        }

        $ymlFile = DrupalPractice_Project::getServicesYmlFile($phpcsFile);
        if ($ymlFile === false) {
            $cache[$phpcsFile->getFilename()] = false;
            return false;
        }

        $services = Yaml::parse(file_get_contents($ymlFile));
        if (isset($services['services']) === false) {
            $cache[$phpcsFile->getFilename()] = false;
            return false;
        }

        $nsEnd           = $phpcsFile->findNext(
            [
             T_NS_SEPARATOR,
             T_STRING,
             T_WHITESPACE,
            ],
            ($namespacePtr + 1),
            null,
            true
        );
        $namespace       = trim($phpcsFile->getTokensAsString(($namespacePtr + 1), ($nsEnd - $namespacePtr - 1)));
        $classNameSpaced = ltrim($namespace.'\\'.$phpcsFile->getDeclarationName($classPtr), '\\');

        foreach ($services['services'] as $service) {
            if (isset($service['class']) === true
                && $classNameSpaced === ltrim($service['class'], '\\')
            ) {
                $cache[$phpcsFile->getFilename()] = true;
                return true;
            }
        }

        return false;

    }//end isServiceClass()


}//end class
