<?php
/**
 * DrupalPractice_Sniffs_Objects_GlobalClassSniff.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Checks that Node::load() calls and friends are not used in forms or controllers.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalPractice_Sniffs_Objects_GlobalClassSniff implements PHP_CodeSniffer_Sniff
{

    /**
     * Class names that should not be called statically, mostly entity classes.
     *
     * @var string[]
     */
    protected $classes = array(
                          'Node',
                          'NodeType',
                          'Term',
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

        // We are only interested in static class method calls, not in the global
        // scope.
        if (in_array($tokens[$stackPtr]['content'], $this->classes) === false
            || $tokens[($stackPtr + 1)]['code'] !== T_DOUBLE_COLON
            || isset($tokens[($stackPtr + 2)]) === false
            || $tokens[($stackPtr + 2)]['code'] !== T_STRING
            || in_array($tokens[($stackPtr + 2)]['content'], array('load', 'loadMultiple')) === false
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
        $classPtr   = key($tokens[$stackPtr]['conditions']);
        $extendsPtr = $phpcsFile->findNext(T_EXTENDS, ($classPtr + 1), $tokens[$classPtr]['scope_opener']);
        if ($extendsPtr === false) {
            return;
        }

        $extendsNamePtr = $phpcsFile->findNext(T_STRING, ($extendsPtr + 1), $tokens[$classPtr]['scope_opener']);

        if ($extendsNamePtr === false || in_array($tokens[$extendsNamePtr]['content'], DrupalPractice_Sniffs_Objects_GlobalDrupalSniff::$baseClasses) === false
        ) {
            return;
        }

        $warning = '%s::%s calls should be avoided in classes, use dependency injection instead';
        $data    = array(
                    $tokens[$stackPtr]['content'],
                    $tokens[($stackPtr + 2)]['content'],
                   );
        $phpcsFile->addWarning($warning, $stackPtr, 'GlobalClass', $data);

    }//end process()


}//end class
