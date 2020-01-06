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
     * @param string $testFile The name of the file being tested.
     *
     * @return array<int, int>
     */
    protected function getErrorList(string $testFile): array
    {
        // Basic layout is wrong.
        return [24 => 2];

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
        return [
            // Has version x 2.
            37 => 2,
            // The see url.
            39 => 1,
            // Has version x 2.
            47 => 2,
            // The see url.
            49 => 1,
        ];

    }//end getWarningList()


}//end class
