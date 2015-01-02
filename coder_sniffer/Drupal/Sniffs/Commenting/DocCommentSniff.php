<?php
/**
 * Ensures doc blocks follow basic formatting.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Ensures doc blocks follow basic formatting.
 *
 * Largely copied from Generic_Sniffs_Commenting_DocCommentSniff, but Drupal @file
 * comments are different.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class Drupal_Sniffs_Commenting_DocCommentSniff implements PHP_CodeSniffer_Sniff
{

    /**
     * A list of tokenizers this sniff supports.
     *
     * @var array
     */
    public $supportedTokenizers = array(
                                   'PHP',
                                   'JS',
                                  );


    /**
     * Returns an array of tokens this test wants to listen for.
     *
     * @return array
     */
    public function register()
    {
        return array(T_DOC_COMMENT_OPEN_TAG);

    }//end register()


    /**
     * Processes this test, when one of its tokens is encountered.
     *
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     * @param int                  $stackPtr  The position of the current token
     *                                        in the stack passed in $tokens.
     *
     * @return void
     */
    public function process(PHP_CodeSniffer_File $phpcsFile, $stackPtr)
    {
        $tokens       = $phpcsFile->getTokens();
        $commentEnd   = $phpcsFile->findNext(T_DOC_COMMENT_CLOSE_TAG, ($stackPtr + 1));
        $commentStart = $tokens[$commentEnd]['comment_opener'];

        $empty = array(
                  T_DOC_COMMENT_WHITESPACE,
                  T_DOC_COMMENT_STAR,
                 );

        $short = $phpcsFile->findNext($empty, ($stackPtr + 1), $commentEnd, true);
        if ($short === false) {
            // No content at all.
            $error = 'Doc comment is empty';
            $phpcsFile->addError($error, $stackPtr, 'Empty');
            return;
        }

        // Ignore doc blocks in functions, this is handled by InlineCommentSniff.
        if (!empty($tokens[$stackPtr]['conditions']) && in_array(T_FUNCTION, $tokens[$stackPtr]['conditions'])) {
            return;
        }

        // The first line of the comment should just be the /** code.
        if ($tokens[$short]['line'] === $tokens[$stackPtr]['line']) {
            $error = 'The open comment tag must be the only content on the line';
            $fix   = $phpcsFile->addFixableError($error, $stackPtr, 'ContentAfterOpen');
            if ($fix === true) {
                $phpcsFile->fixer->beginChangeset();
                $phpcsFile->fixer->addNewline($stackPtr);
                $phpcsFile->fixer->addContentBefore($short, '* ');
                $phpcsFile->fixer->endChangeset();
            }
        }

        // The last line of the comment should just be the */ code.
        $prev = $phpcsFile->findPrevious($empty, ($commentEnd - 1), $stackPtr, true);
        if ($tokens[$commentEnd]['content'] !== '*/') {
            $error = 'Wrong function doc comment end; expected "*/", found "%s"';
            $phpcsFile->addError($error, $commentEnd, 'WrongEnd', array($tokens[$commentEnd]['content']));
        }

        // Check for additional blank lines at the end of the comment.
        if ($tokens[$prev]['line'] < ($tokens[$commentEnd]['line'] - 1)) {
            $error = 'Additional blank lines found at end of doc comment';
            $fix   = $phpcsFile->addFixableError($error, $commentEnd, 'SpacingAfter');
            if ($fix === true) {
                $phpcsFile->fixer->beginChangeset();
                for ($i = ($prev + 1); $i < $commentEnd; $i++) {
                    if ($tokens[($i + 1)]['line'] === $tokens[$commentEnd]['line']) {
                        break;
                    }

                    $phpcsFile->fixer->replaceToken($i, '');
                }

                $phpcsFile->fixer->endChangeset();
            }
        }

        // The short description of @file comments is one line below.
        if ($tokens[$short]['code'] === T_DOC_COMMENT_TAG && $tokens[$short]['content'] == '@file') {
            $fileShort = $phpcsFile->findNext($empty, ($short + 1), $commentEnd, true);
            if ($fileShort !== false) {
                $short = $fileShort;
            }
        }

        // Do not check defgroup sections, they have no short description. Also don't
        // check PHPUnit tests doc blocks because they might not have a description.
        if (in_array($tokens[$short]['content'], array('@defgroup', '@addtogroup', '@}', '@coversDefaultClass'))) {
            return;
        }

        // Check for a comment description.
        if ($tokens[$short]['code'] !== T_DOC_COMMENT_STRING) {
            $error = 'Missing short description in doc comment';
            $phpcsFile->addError($error, $stackPtr, 'MissingShort');
            return;
        }

        // No extra newline before short description.
        if ($tokens[$short]['line'] !== ($tokens[$stackPtr]['line'] + 1) && !isset($fileShort)) {
            $error = 'Doc comment short description must be on the first line';
            $fix   = $phpcsFile->addFixableError($error, $short, 'SpacingBeforeShort');
            if ($fix === true) {
                $phpcsFile->fixer->beginChangeset();
                for ($i = $stackPtr; $i < $short; $i++) {
                    if ($tokens[$i]['line'] === $tokens[$stackPtr]['line']) {
                        continue;
                    } else if ($tokens[$i]['line'] === $tokens[$short]['line']) {
                        break;
                    }

                    $phpcsFile->fixer->replaceToken($i, '');
                }

                $phpcsFile->fixer->endChangeset();
            }
        }

        if ($tokens[($short - 1)]['content'] !== ' ') {
            $error = 'Function comment short description must start with exactly one space';
            $phpcsFile->addError($error, $short, 'ShortStartSpace');
        }

        // Account for the fact that a short description might cover
        // multiple lines.
        $shortContent = $tokens[$short]['content'];
        $shortEnd     = $short;
        for ($i = ($short + 1); $i < $commentEnd; $i++) {
            if ($tokens[$i]['code'] === T_DOC_COMMENT_STRING) {
                if ($tokens[$i]['line'] === ($tokens[$shortEnd]['line'] + 1)) {
                    $shortContent .= $tokens[$i]['content'];
                    $shortEnd      = $i;
                } else {
                    break;
                }
            }
            if ($tokens[$i]['code'] === T_DOC_COMMENT_TAG) {
                break;
            }
        }
        // Remove any trailing white spaces which are detected by other sniffs.
        $shortContent = trim($shortContent);

        if (preg_match('|\p{Lu}|u', $shortContent[0]) === 0 && $shortContent !== '{@inheritdoc}') {
            $error = 'Doc comment short description must start with a capital letter';
            $phpcsFile->addError($error, $short, 'ShortNotCapital');
        }

        $lastChar = substr($shortContent, -1);
        if ($lastChar !== '.' && $shortContent !== '{@inheritdoc}') {
            $error = 'Doc comment short description must end with a full stop';
            $phpcsFile->addError($error, $short, 'ShortFullStop');
        }

        if ($tokens[$short]['line'] !== $tokens[$shortEnd]['line']) {
            $error = 'Doc comment short description must be on a single line, further text should be a separate paragraph';
            $phpcsFile->addError($error, $shortEnd, 'ShortSingleLine');
        }

        $long = $phpcsFile->findNext($empty, ($shortEnd + 1), ($commentEnd - 1), true);
        if ($long === false) {
            return;
        }

        if ($tokens[$long]['code'] === T_DOC_COMMENT_STRING) {
            if ($tokens[$long]['line'] !== ($tokens[$shortEnd]['line'] + 2)) {
                $error = 'There must be exactly one blank line between descriptions in a doc comment';
                $fix   = $phpcsFile->addFixableError($error, $long, 'SpacingBetween');
                if ($fix === true) {
                    $phpcsFile->fixer->beginChangeset();
                    for ($i = ($shortEnd + 1); $i < $long; $i++) {
                        if ($tokens[$i]['line'] === $tokens[$shortEnd]['line']) {
                            continue;
                        } else if ($tokens[$i]['line'] === ($tokens[$long]['line'] - 1)) {
                            break;
                        }

                        $phpcsFile->fixer->replaceToken($i, '');
                    }

                    $phpcsFile->fixer->endChangeset();
                }
            }

            if (preg_match('|\p{Lu}|u', $tokens[$long]['content'][0]) === 0) {
                $error = 'Doc comment long description must start with a capital letter';
                $phpcsFile->addError($error, $long, 'LongNotCapital');
            }
        }//end if

        if (empty($tokens[$commentStart]['comment_tags']) === true) {
            // No tags in the comment.
            return;
        }

        $firstTag = $tokens[$commentStart]['comment_tags'][0];
        $prev     = $phpcsFile->findPrevious($empty, ($firstTag - 1), $stackPtr, true);
        // This does not apply to @file and @code tags.
        if ($tokens[$firstTag]['line'] !== ($tokens[$prev]['line'] + 2)
            && !isset($fileShort) && $tokens[$firstTag]['content'] !== '@code'
        ) {
            $error = 'There must be exactly one blank line before the tags in a doc comment';
            $fix   = $phpcsFile->addFixableError($error, $firstTag, 'SpacingBeforeTags');
            if ($fix === true) {
                $phpcsFile->fixer->beginChangeset();
                for ($i = ($prev + 1); $i < $firstTag; $i++) {
                    if ($tokens[$i]['line'] === $tokens[$firstTag]['line']) {
                        break;
                    }

                    $phpcsFile->fixer->replaceToken($i, '');
                }

                $indent = str_repeat(' ', $tokens[$stackPtr]['column']);
                $phpcsFile->fixer->addContent($prev, $phpcsFile->eolChar.$indent.'*'.$phpcsFile->eolChar);
                $phpcsFile->fixer->endChangeset();
            }
        }

        // Break out the tags into groups and check alignment within each.
        // A tag group is one where there are no blank lines between tags.
        // The param tag group is special as it requires all @param tags to be inside.
        $tagGroups    = array();
        $groupid      = 0;
        $paramGroupid = null;
        $currentTag   = null;
        $previousTag  = null;
        $isNewGroup   = null;
        foreach ($tokens[$commentStart]['comment_tags'] as $pos => $tag) {
            if ($pos > 0) {
                $prev = $phpcsFile->findPrevious(
                    T_DOC_COMMENT_STRING,
                    ($tag - 1),
                    $tokens[$commentStart]['comment_tags'][($pos - 1)]
                );

                if ($prev === false) {
                    $prev = $tokens[$commentStart]['comment_tags'][($pos - 1)];
                }

                $isNewGroup = $tokens[$prev]['line'] !== ($tokens[$tag]['line'] - 1);
                if ($isNewGroup) {
                    $groupid++;
                }
            }

            $currentTag = $tokens[$tag]['content'];
            if ($currentTag === '@param') {
                if (($paramGroupid === null
                    && empty($tagGroups[$groupid]) === false)
                    || ($paramGroupid !== null
                    && $paramGroupid !== $groupid)
                ) {
                    $error = 'Paramater tags must be grouped together in a doc commment';
                    $phpcsFile->addError($error, $tag, 'ParamGroup');
                }

                if ($paramGroupid === null) {
                    $paramGroupid = $groupid;
                }
            // Every new tag section should be separated by a blank line.
            // Exclude @endcode and @endlink.
            } else if (
                $isNewGroup === false
                && !in_array($currentTag, array('@endcode', '@endlink'))
                && $previousTag !== $currentTag
            ) {
                $error = 'Separate the %s and %s sections by a blank line.';
                $phpcsFile->addError($error, $tag, 'TagGroupSpacing', array($previousTag, $currentTag));
            }//end if

            $previousTag = $currentTag;
            $tagGroups[$groupid][] = $tag;
        }//end foreach

        foreach ($tagGroups as $group) {
            $maxLength = 0;
            $paddings  = array();
            foreach ($group as $pos => $tag) {
                $tagLength = strlen($tokens[$tag]['content']);
                if ($tagLength > $maxLength) {
                    $maxLength = $tagLength;
                }

                // Check for a value. No value means no padding needed.
                $string = $phpcsFile->findNext(T_DOC_COMMENT_STRING, $tag, $commentEnd);
                if ($string !== false && $tokens[$string]['line'] === $tokens[$tag]['line']) {
                    $paddings[$tag] = strlen($tokens[($tag + 1)]['content']);
                }
            }

            // Check that there was single blank line after the tag block
            // but account for a multi-line tag comments.
            $lastTag = $group[$pos];
            $next    = $phpcsFile->findNext(T_DOC_COMMENT_TAG, ($lastTag + 3), $commentEnd);
            if ($next !== false) {
                $prev = $phpcsFile->findPrevious(array(T_DOC_COMMENT_TAG, T_DOC_COMMENT_STRING), ($next - 1), $commentStart);
                if ($tokens[$next]['line'] !== ($tokens[$prev]['line'] + 2)) {
                    $error = 'There must be a single blank line after a tag group';
                    $fix   = $phpcsFile->addFixableError($error, $lastTag, 'SpacingAfterTagGroup');
                    if ($fix === true) {
                        $phpcsFile->fixer->beginChangeset();
                        for ($i = ($prev + 1); $i < $next; $i++) {
                            if ($tokens[$i]['line'] === $tokens[$next]['line']) {
                                break;
                            }

                            $phpcsFile->fixer->replaceToken($i, '');
                        }

                        $indent = str_repeat(' ', $tokens[$stackPtr]['column']);
                        $phpcsFile->fixer->addContent($prev, $phpcsFile->eolChar.$indent.'*'.$phpcsFile->eolChar);
                        $phpcsFile->fixer->endChangeset();
                    }
                }
            }//end if

            // Now check paddings.
            foreach ($paddings as $tag => $padding) {
                if ($padding !== 1) {
                    $error = 'Tag value indented incorrectly; expected 1 space but found %s';
                    $data  = array(
                              $padding,
                             );

                    $fix = $phpcsFile->addFixableError($error, ($tag + 1), 'TagValueIndent', $data);
                    if ($fix === true) {
                        $phpcsFile->fixer->replaceToken(($tag + 1), ' ');
                    }
                }
            }
        }//end foreach

        // If there is a param group, it needs to be first; with the exception of
        // @code and @todo tags.
        if ($paramGroupid !== null && $paramGroupid !== 0
            && in_array($tokens[$tokens[$commentStart]['comment_tags'][0]]['content'], array('@code', '@todo')) === false
        ) {
            $error = 'Paramater tags must be defined first in a doc commment';
            $phpcsFile->addError($error, $tagGroups[$paramGroupid][0], 'ParamNotFirst');
        }

        $foundTags = array();
        foreach ($tokens[$stackPtr]['comment_tags'] as $pos => $tag) {
            $tagName = $tokens[$tag]['content'];
            if (isset($foundTags[$tagName]) === true) {
                $lastTag = $tokens[$stackPtr]['comment_tags'][($pos - 1)];
                if ($tokens[$lastTag]['content'] !== $tagName) {
                    $error = 'Tags must be grouped together in a doc comment';
                    $phpcsFile->addError($error, $tag, 'TagsNotGrouped');
                }

                continue;
            }

            $foundTags[$tagName] = true;
        }

    }//end process()


}//end class
