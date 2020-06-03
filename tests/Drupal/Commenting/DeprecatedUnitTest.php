<?php

namespace Drupal\Test\Commenting;

use Drupal\Test\CoderSniffUnitTest;

class DeprecatedUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * There are three deprecated sniffs which produce an error:
     *   'IncorrectTextLayout - for the basic deprecation text, sometimes fixable.
     *   'MissingExtraInfo' - when there is no extra info after the main text.
     *   'DeprecatedMissingSeeTag' - when there is no @see tag.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getErrorList(string $testFile): array
    {
        return [
            // Basic layout is wrong. Missing see url.
            24  => 2,
            // No details given, check that the test gives two errors.
            75  => 2,
            // Layout OK but missing the extra info.
            81  => 1,
            // Text layout is wrong but fixable.
            89  => 1,
            // Text layout is wrong but fixable.
            98  => 1,
            // See Url has trailing punctuation which is fixable.
            101 => 1,
        ];

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * There are two deprecated sniffs which produce a warning:
     *   'DeprecatedVersionFormat' - where the version is written incorrectly.
     *   'DeprecatedWrongSeeUrlFormat' - the url is not to the standard format.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getWarningList(string $testFile): array
    {
        return [
            // Both core versions incorrectly formatted.
            37 => 2,
            // The see url is wrong.
            39 => 1,
            // Both contrib versions incorrectly formatted.
            47 => 2,
            // The see url is wrong.
            49 => 1,
            // Core version incorrectly formatted.
            81 => 1,
        ];

    }//end getWarningList()


}//end class
