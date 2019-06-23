<?php

namespace DrupalPractice\Sniffs\Objects;

use Drupal\Test\CoderSniffUnitTest;

class GlobalFunctionUnitTest extends CoderSniffUnitTest
{


    /**
     * Returns the lines where errors should occur.
     *
     * The key of the array should represent the line number and the value
     * should represent the number of errors that should occur on that line.
     *
     * @return array(int => int)
     */
    protected function getErrorList()
    {
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
     * @return array(int => int)
     */
    protected function getWarningList($testFile=null)
    {
        switch ($testFile) {
        case 'TestForm.php':
            return [
                6 => 1,
                8 => 1,
            ];
        case 'ExampleClass.php':
            return [37 => 1];
        case 'ExampleClassWithDependencyInjection.php':
            return [
                31 => 1,
                38 => 1,
            ];
        case 'ExampleService.php':
            return [
                30 => 1,
                37 => 1,
            ];
        default:
            return [];
        }//end switch

    }//end getWarningList()


    /**
     * Returns a list of test files that should be checked.
     *
     * @param string $testFileBase The base path that the unit tests files will have.
     *
     * @return array The list of test files.
     */
    protected function getTestFiles($testFileBase)
    {
        return [
            __DIR__.'/drupal8/DeclareT.php',
            __DIR__.'/drupal8/example.module',
            __DIR__.'/drupal8/ExampleClass.php',
            __DIR__.'/drupal8/ExampleClassWithDependencyInjection.php',
            __DIR__.'/drupal8/ExampleService.php',
            __DIR__.'/drupal8/TestForm.php',
            __DIR__.'/drupal7/ExampleMigration.php',
        ];

    }//end getTestFiles()


}//end class
