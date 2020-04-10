<?php
/**
 * \Drupal\Sniffs\Classes\InterfaceImplementedSniff.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

namespace Drupal\Sniffs\Classes;

use PHP_CodeSniffer\Files\File;
use PHP_CodeSniffer\Sniffs\Sniff;

/**
 * Checks that interface already announced.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class InterfaceAnnouncedSniff implements Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array<int|string>
     */
    public function register()
    {
        return [T_EXTENDS];

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param \PHP_CodeSniffer\Files\File $phpcsFile The file being scanned.
     * @param int                         $stackPtr  The position of the current token in
     *                                               the stack passed in $tokens.
     *
     * @return void
     */
    public function process(File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        $nsIndex = $phpcsFile->findPrevious(T_NAMESPACE, $stackPtr);

        $ns = '';

        if ($nsIndex !== false) {
            $semicolonIndex = $phpcsFile->findNext(T_SEMICOLON, $nsIndex);

            for ($i = ($nsIndex + 2); $i < $semicolonIndex; $i++) {
                $ns .= $tokens[$i]['content'];
            }
        }

        $classIndex = $phpcsFile->findPrevious(T_STRING, $stackPtr);

        $parentClassIndex = $phpcsFile->findNext(T_STRING, $stackPtr);

        if ($classIndex !== false && $parentClassIndex !== false) {
            $class       = $tokens[$classIndex]['content'];
            $parentClass = $tokens[$parentClassIndex]['content'];
            if ($ns !== '') {
                $class       = "\\$ns\\$class";
                $parentClass = "\\$ns\\$parentClass";
            }

            if (class_exists($class) === true && class_exists($parentClass) === true) {
                // Interfaces implemented by the parent class.
                $parentImplementedInterfacesWithNamespae = class_implements($parentClass);
                if (empty($parentImplementedInterfacesWithNamespae) === true) {
                    return;
                }

                $parentImplementedInterfaces = [];
                foreach ($parentImplementedInterfacesWithNamespae as $item) {
                    $parts = explode("\\", $item);
                    $parentImplementedInterfaces[] = array_pop($parts);
                }

                $expectedImplementsIndex = ($parentClassIndex + 2);
                // Skip no T_IMPLEMENTS right after the parent class.
                if ((isset($tokens[$expectedImplementsIndex]) === true && ($tokens[$expectedImplementsIndex]['type'] === 'T_IMPLEMENTS')) === false) {
                    return;
                }

                $inlineImplementedInterfaces = [];
                $implementsIndex       = $phpcsFile->findNext(T_IMPLEMENTS, $parentClassIndex);
                $openCurlyBracketIndex = $phpcsFile->findNext(T_OPEN_CURLY_BRACKET, $parentClassIndex);

                // Space implements space.
                for ($i = ($implementsIndex + 2); $i < $openCurlyBracketIndex; $i++) {
                    if ($tokens[$i]['type'] === 'T_STRING') {
                        $inlineImplementedInterfaces[$i] = $tokens[$i]['content'];
                    }
                }

                $remove  = [];
                $untouch = [];
                foreach ($inlineImplementedInterfaces as $index => $interface) {
                    if (in_array($interface, $parentImplementedInterfaces, true) === true) {
                        $phpcsFile->addFixableWarning(
                            "The parent class '$parentClass' of '$class' already announced '$interface'",
                            $index,
                            'InterfaceAnnounced'
                        );
                        $remove[] = $interface;
                        continue;
                    }

                    $untouch[] = $interface;
                }

                if ($phpcsFile->fixer !== null) {
                    $left = count($untouch);

                    if ($left === 0) {
                        $first = ($parentClassIndex + 2);
                        // Keep a space, remove all the rest.
                        for ($i = $first; $i < $openCurlyBracketIndex; $i++) {
                            $phpcsFile->fixer->replaceToken($i, '');
                        }

                        return;
                    }

                    $first = ($implementsIndex + 2);

                    if ($left === 1) {
                        // Keep the first whitespace and the last whitespace.
                        for ($i = $first; $i < ($openCurlyBracketIndex - 1); $i++) {
                            if ($tokens[$i]['type'] === 'T_STRING' && $tokens[$i]['content'] === $untouch[0]) {
                                continue;
                            }

                            $phpcsFile->fixer->replaceToken($i, '');
                        }

                        return;
                    }

                    if ($left > 1) {
                        for ($i = $first; $i < ($openCurlyBracketIndex - 1); $i++) {
                            if (in_array($tokens[$i]['content'], $remove, true) === true) {
                                $phpcsFile->fixer->replaceToken($i, '');

                                // If it is not the last interface, the comma right after it should be removed as well.
                                if ($tokens[$i]['type'] === 'T_STRING' && $tokens[($i + 1)]['type'] === 'T_COMMA') {
                                    $phpcsFile->fixer->replaceToken(($i + 1), '');
                                }

                                // If it is the last interface, the comma right before it should be removed as well.
                                if ($tokens[$i]['type'] === 'T_STRING' && $tokens[($i + 1)]['type'] === 'T_WHITESPACE') {
                                    $phpcsFile->fixer->replaceToken(($i - 1), '');
                                }
                            }
                        }
                    }
                }//end if
            }//end if
        }//end if

    }//end process()


}//end class
