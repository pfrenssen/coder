<?php

namespace Drupal\Test\Arrays;

use Drupal\Test\CoderSniffUnitTest;

class ArrayUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getErrorList(string $testFile): array
    {
        switch ($testFile) {
        case 'ArrayUnitTest.inc':
            return [
                13  => 1,
                33  => 1,
                83  => 1,
                88  => 1,
                92  => 1,
                140 => 1,
                141 => 1,
                143 => 1,
                144 => 1,
                146 => 1,
                147 => 1,
                154 => 1,
                155 => 1,
                157 => 1,
                158 => 1,
                160 => 1,
                161 => 1,
                172 => 1,
                173 => 1,
                175 => 1,
                176 => 1,
                178 => 1,
                179 => 1,
                185 => 1,
                186 => 1,
                188 => 1,
                189 => 1,
                191 => 1,
                192 => 1,
            ];
        case 'ArrayUnitTest.1.inc':
            return [
                14 => 1,
                17 => 1,
                20 => 1,
                24 => 1,
            ];
        }//end switch

        return [];

    }//end getErrorList()


    /**
     * Returns the lines where warnings should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of warnings that should occur on that line.
     *
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getWarningList(string $testFile): array
    {
        switch ($testFile) {
        case 'ArrayUnitTest.inc':
            return [
                17 => 1,
                22 => 1,
                23 => 1,
                24 => 1,
                37 => 1,
                42 => 1,
                44 => 1,
                59 => 1,
                76 => 1,
            ];
        }

        return [];

    }//end getWarningList()


}//end class
