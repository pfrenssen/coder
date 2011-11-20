<?php
/**
 * A test to ensure that arrays conform to the array coding standard.
 *
 * PHP version 5
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * A test to ensure that arrays conform to the array coding standard. Largely copied
 * from Squiz_Sniffs_Arrays_ArrayDeclarationSniff.
 *
 * @category  PHP
 * @package   PHP_CodeSniffer
 * @author    Greg Sherwood <gsherwood@squiz.net>
 * @author    Marc McIntyre <mmcintyre@squiz.net>
 * @copyright 2006 Squiz Pty Ltd (ABN 77 084 670 600)
 * @license   http://matrix.squiz.net/developer/tools/php_cs/licence BSD Licence
 * @version   Release: 1.3.1
 * @link      http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_Sniffs_Array_ArrayDeclarationSniff implements PHP_CodeSniffer_Sniff
{


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_ARRAY);

    }//end register()


    /**
     * Processes this sniff, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The current file being checked.
     * @param int                  $stackPtr  The position of the current token in the
     *                                        stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens = $phpcsFile->getTokens();

        // Array keyword should be lower case.
        if (strtolower($tokens[$stackPtr]['content']) !== $tokens[$stackPtr]['content']) {
            $error = 'Array keyword should be lower case; expected "array" but found "%s"';
            $data  = array($tokens[$stackPtr]['content']);
            $phpcsFile->addError($error, $stackPtr, 'NotLowerCase', $data);
        }

        $arrayStart   = $tokens[$stackPtr]['parenthesis_opener'];
        $arrayEnd     = $tokens[$arrayStart]['parenthesis_closer'];
        $keywordStart = $tokens[$stackPtr]['column'];

        if ($arrayStart != ($stackPtr + 1)) {
            $error = 'There must be no space between the Array keyword and the opening parenthesis';
            $phpcsFile->addError($error, $stackPtr, 'SpaceAfterKeyword');
        }

        // Check for empty arrays.
        $content = $phpcsFile->findNext(array(T_WHITESPACE), ($arrayStart + 1), ($arrayEnd + 1), true);
        if ($content === $arrayEnd) {
            // Empty array, but if the brackets aren't together, there's a problem.
            if (($arrayEnd - $arrayStart) !== 1) {
                $error = 'Empty array declaration must have no space between the parentheses';
                $phpcsFile->addError($error, $stackPtr, 'SpaceInEmptyArray');

                // We can return here because there is nothing else to check. All code
                // below can assume that the array is not empty.
                return;
            }
        }

        if ($tokens[$arrayStart]['line'] === $tokens[$arrayEnd]['line']) {
            // Single line array.
            // Check if there are multiple values. If so, then it has to be multiple lines
            // unless it is contained inside a function call or condition.
            $valueCount = 0;
            $commas     = array();
            for ($i = ($arrayStart + 1); $i < $arrayEnd; $i++) {
                // Skip bracketed statements, like function calls.
                if ($tokens[$i]['code'] === T_OPEN_PARENTHESIS) {
                    $i = $tokens[$i]['parenthesis_closer'];
                    continue;
                }

                if ($tokens[$i]['code'] === T_COMMA) {
                    $valueCount++;
                    $commas[] = $i;
                }
            }

            if ($valueCount > 0) {

                // We have a multiple value array that is inside a condition or
                // function. Check its spacing is correct.
                foreach ($commas as $comma) {
                    if ($tokens[($comma + 1)]['code'] !== T_WHITESPACE) {
                        $content = $tokens[($comma + 1)]['content'];
                        $error   = 'Expected 1 space between comma and "%s"; 0 found';
                        $data    = array($content);
                        $phpcsFile->addError($error, $comma, 'NoSpaceAfterComma', $data);
                    } else {
                        $spaceLength = strlen($tokens[($comma + 1)]['content']);
                        if ($spaceLength !== 1) {
                            $content = $tokens[($comma + 2)]['content'];
                            $error   = 'Expected 1 space between comma and "%s"; %s found';
                            $data    = array(
                                        $content,
                                        $spaceLength,
                                       );
                            $phpcsFile->addError($error, $comma, 'SpaceAfterComma', $data);
                        }
                    }

                    if ($tokens[($comma - 1)]['code'] === T_WHITESPACE) {
                        $content     = $tokens[($comma - 2)]['content'];
                        $spaceLength = strlen($tokens[($comma - 1)]['content']);
                        $error       = 'Expected 0 spaces between "%s" and comma; %s found';
                        $data        = array(
                                        $content,
                                        $spaceLength,
                                       );
                        $phpcsFile->addError($error, $comma, 'SpaceBeforeComma', $data);
                    }
                }//end foreach
            }//end if

            return;
        }//end if

        $nextToken  = $stackPtr;
        $lastComma  = $stackPtr;
        $keyUsed    = false;
        $singleUsed = false;
        $lastToken  = '';
        $indices    = array();
        $maxLength  = 0;

        // Find all the double arrows that reside in this scope.
        while (($nextToken = $phpcsFile->findNext(array(T_DOUBLE_ARROW, T_COMMA, T_ARRAY), ($nextToken + 1), $arrayEnd)) !== false) {
            $currentEntry = array();

            if ($tokens[$nextToken]['code'] === T_ARRAY) {
                // Let subsequent calls of this test handle nested arrays.
                $indices[] = array(
                              'value' => $nextToken,
                             );
                $nextToken = $tokens[$tokens[$nextToken]['parenthesis_opener']]['parenthesis_closer'];
                continue;
            }

            if ($tokens[$nextToken]['code'] === T_COMMA) {
                $stackPtrCount = 0;
                if (isset($tokens[$stackPtr]['nested_parenthesis']) === true) {
                    $stackPtrCount = count($tokens[$stackPtr]['nested_parenthesis']);
                }

                if (count($tokens[$nextToken]['nested_parenthesis']) > ($stackPtrCount + 1)) {
                    // This comma is inside more parenthesis than the ARRAY keyword,
                    // then there it is actually a comma used to seperate arguments
                    // in a function call.
                    continue;
                }

                if ($keyUsed === true && $lastToken === T_COMMA) {
                    $error = 'No key specified for array entry; first entry specifies key';
                    $phpcsFile->addError($error, $nextToken, 'NoKeySpecified');
                    return;
                }

                if ($keyUsed === false) {
                    if ($tokens[($nextToken - 1)]['code'] === T_WHITESPACE) {
                        $content     = $tokens[($nextToken - 2)]['content'];
                        $spaceLength = strlen($tokens[($nextToken - 1)]['content']);
                        //$error       = 'Expected 0 spaces between "%s" and comma; %s found';
                        $data        = array(
                                        $content,
                                        $spaceLength,
                                       );
                        $phpcsFile->addError($error, $nextToken, 'SpaceBeforeComma', $data);
                    }

                    // Find the value, which will be the first token on the line,
                    // excluding the leading whitespace.
                    $valueContent = $phpcsFile->findPrevious(PHP_CodeSniffer_Tokens::$emptyTokens, ($nextToken - 1), null, true);
                    while ($tokens[$valueContent]['line'] === $tokens[$nextToken]['line']) {
                        if ($valueContent === $arrayStart) {
                            // Value must have been on the same line as the array
                            // parenthesis, so we have reached the start of the value.
                            break;
                        }

                        $valueContent--;
                    }

                    $valueContent = $phpcsFile->findNext(T_WHITESPACE, ($valueContent + 1), $nextToken, true);
                    $indices[]    = array('value' => $valueContent);
                    $singleUsed   = true;
                }//end if

                $lastToken = T_COMMA;
                continue;
            }//end if

            if ($tokens[$nextToken]['code'] === T_DOUBLE_ARROW) {
                if ($singleUsed === true) {
                    $error = 'Key specified for array entry; first entry has no key';
                    $phpcsFile->addError($error, $nextToken, 'KeySpecified');
                    return;
                }

                $currentEntry['arrow'] = $nextToken;
                $keyUsed               = true;

                // Find the start of index that uses this double arrow.
                $indexEnd   = $phpcsFile->findPrevious(T_WHITESPACE, ($nextToken - 1), $arrayStart, true);
                $indexStart = $phpcsFile->findPrevious(T_WHITESPACE, $indexEnd, $arrayStart);

                if ($indexStart === false) {
                    $index = $indexEnd;
                } else {
                    $index = ($indexStart + 1);
                }

                $currentEntry['index']         = $index;
                $currentEntry['index_content'] = $phpcsFile->getTokensAsString($index, ($indexEnd - $index + 1));

                $indexLength = strlen($currentEntry['index_content']);
                if ($maxLength < $indexLength) {
                    $maxLength = $indexLength;
                }

                // Find the value of this index.
                $nextContent           = $phpcsFile->findNext(array(T_WHITESPACE), ($nextToken + 1), $arrayEnd, true);
                $currentEntry['value'] = $nextContent;
                $indices[]             = $currentEntry;
                $lastToken             = T_DOUBLE_ARROW;
            }//end if
        }//end while

        // Check for mutli-line arrays that should be single-line.
        $singleValue = false;

        if (empty($indices) === true) {
            $singleValue = true;
        } else if (count($indices) === 1) {
            if ($lastToken === T_COMMA) {
                // There may be another array value without a comma.
                $exclude     = PHP_CodeSniffer_Tokens::$emptyTokens;
                $exclude[]   = T_COMMA;
                $nextContent = $phpcsFile->findNext($exclude, ($indices[0]['value'] + 1), $arrayEnd, true);
                if ($nextContent === false) {
                    $singleValue = true;
                }
            }

            if ($singleValue === false && isset($indices[0]['arrow']) === false) {
                // A single nested array as a value is fine.
                if ($tokens[$indices[0]['value']]['code'] !== T_ARRAY) {
                    $singleValue === true;
                }
            }
        }

        /*
            This section checks for arrays that don't specify keys.

            Arrays such as:
               array(
                'aaa',
                'bbb',
                'd',
               );
        */

        if ($keyUsed === false && empty($indices) === false) {
            $count     = count($indices);
            $lastIndex = $indices[($count - 1)]['value'];

            $trailingContent = $phpcsFile->findPrevious(T_WHITESPACE, ($arrayEnd - 1), $lastIndex, true);
            if ($tokens[$trailingContent]['code'] !== T_COMMA) {
                $error = 'Comma required after last value in array declaration';
                $phpcsFile->addError($error, $trailingContent, 'NoCommaAfterLast');
            }
        }//end if

        /*
            Below the actual indentation of the array is checked.
            Errors will be thrown when a key is not aligned, when
            a double arrow is not aligned, and when a value is not
            aligned correctly.
            If an error is found in one of the above areas, then errors
            are not reported for the rest of the line to avoid reporting
            spaces and columns incorrectly. Often fixing the first
            problem will fix the other 2 anyway.

            For example:

            $a = array(
                  'index'  => '2',
                 );

            In this array, the double arrow is indented too far, but this
            will also cause an error in the value's alignment. If the arrow were
            to be moved back one space however, then both errors would be fixed.
        */

        $numValues = count($indices);

        $indicesStart = ($keywordStart + 1);
        $arrowStart   = ($indicesStart + $maxLength + 1);
        $valueStart   = ($arrowStart + 3);
        foreach ($indices as $index) {
            if (isset($index['index']) === false) {
                continue;
            }

            if (($tokens[$index['index']]['line'] === $tokens[$stackPtr]['line'])) {
                $error = 'The first index in a multi-value array must be on a new line';
                $phpcsFile->addError($error, $stackPtr, 'FirstIndexNoNewline');
                continue;
            }

            // Check comma spaces.
            if ($tokens[$index['value']]['code'] !== T_ARRAY) {
                $nextComma = false;
                for ($i = ($index['value'] + 1); $i < $arrayEnd; $i++) {
                    // Skip bracketed statements, like function calls.
                    if ($tokens[$i]['code'] === T_OPEN_PARENTHESIS) {
                        $i = $tokens[$i]['parenthesis_closer'];
                        continue;
                    }

                    if ($tokens[$i]['code'] === T_COMMA) {
                        $nextComma = $i;
                        break;
                    }
                }

                // Check that there is no space before the comma.
                if ($nextComma !== false && $tokens[($nextComma - 1)]['code'] === T_WHITESPACE) {
                    $content     = $tokens[($nextComma - 2)]['content'];
                    $spaceLength = strlen($tokens[($nextComma - 1)]['content']);
                    $error       = 'Expected 0 spaces between "%s" and comma; %s found';
                    $data        = array(
                                    $content,
                                    $spaceLength,
                                   );
                    $phpcsFile->addError($error, $nextComma, 'SpaceBeforeComma', $data);
                }
            }
        }//end foreach

    }//end process()


}//end class

?>
