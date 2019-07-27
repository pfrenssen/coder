<?php

namespace DrupalPractice\ProjectDetection;

use DrupalPractice\Project;

/**
 * Tests that project and version detection works.
 */
class ProjectUnitTest extends \PHPUnit_Framework_TestCase
{

    /**
     * The mocked file object for testing.
     *
     * @var \PHP_CodeSniffer\Files\File|PHPUnit_Framework_MockObject_MockObject
     */
    protected $phpcsFile;


    /**
     * {@inheritdoc}
     *
     * @return void
     */
    public function setUp()
    {
        parent::setUp();
        $this->phpcsFile = $this->getMockBuilder('\PHP_CodeSniffer\Files\File')
            ->disableOriginalConstructor()
            ->getMock();

    }//end setUp()


    /**
     * Tests the extending classes Sniff class.
     *
     * @return void
     */
    public function testInfoFileDetection()
    {
        $this->phpcsFile->expects($this->any())
            ->method('getFilename')
            ->will($this->returnValue(__DIR__.'/drupal6/test.php'));

        $this->assertEquals(Project::getInfoFile($this->phpcsFile), __DIR__.'/drupal6/testmodule.info');

    }//end testInfoFileDetection()


    /**
     * Tests the extending classes Sniff class.
     *
     * @return void
     */
    public function testInfoFileNestedDetection()
    {
        $this->phpcsFile->expects($this->any())
            ->method('getFilename')
            ->will($this->returnValue(__DIR__.'/drupal6/nested/test.php'));

        $this->assertEquals(Project::getInfoFile($this->phpcsFile), __DIR__.'/drupal6/testmodule.info');

    }//end testInfoFileNestedDetection()


    /**
     * Tests the extending classes Sniff class.
     *
     * @param string $filename    Name of the file that will be checked.
     * @param string $coreVersion Expected core version for the file.
     *
     * @dataProvider coreVersionProvider
     *
     * @return void
     */
    public function testCoreVersion($filename, $coreVersion)
    {
        $this->phpcsFile->expects($this->any())
            ->method('getFilename')
            ->will($this->returnValue($filename));

        $this->assertEquals(Project::getCoreVersion($this->phpcsFile), $coreVersion);

    }//end testCoreVersion()


    /**
     * Data provider for testCoreVersion().
     *
     * @return array
     */
    public function coreVersionProvider()
    {
        return [
            [
                __DIR__.'/drupal6/nested/test.php',
                6,
            ],
            [
                __DIR__.'/drupal7/test.php',
                7,
            ],
            [
                __DIR__.'/drupal8/test.php',
                8,
            ],
            [
                'invalid',
                8,
            ],
        ];

    }//end coreVersionProvider()


    /**
     * Tests the extending classes Sniff class.
     *
     * @param string $filename    Name of the file that will be checked.
     * @param string $projectname Expected project name for the file.
     *
     * @dataProvider projectNameDetectionProvider
     *
     * @return void
     */
    public function testProjectNameDetection($filename, $projectname)
    {
        $this->phpcsFile->expects($this->any())
            ->method('getFilename')
            ->will($this->returnValue($filename));

        $this->assertEquals(Project::getName($this->phpcsFile), $projectname);

    }//end testProjectNameDetection()


    /**
     * Data provider for testProjectNameDetection().
     *
     * @return array
     *   An array of test cases, each test case an array with two elements:
     *   - The filename to check.
     *   - The expected project name.
     */
    public function projectNameDetectionProvider()
    {
        return [
            [
                __DIR__.'/drupal6/testmodule.info',
                'testmodule',
            ],
            [
                __DIR__.'/drupal6/nested/test.php',
                'testmodule',
            ],
            [
                __DIR__.'/drupal7/testmodule.info',
                'testmodule',
            ],
            [
                __DIR__.'/drupal8/testmodule.info.yml',
                'testmodule',
            ],
            [
                __DIR__.'/drupal8/testtheme/testtheme.info.yml',
                'testtheme',
            ],
            [
                __DIR__.'/drupal8/testtheme/testtheme.theme',
                'testtheme',
            ],
            [
                'invalid',
                false,
            ],
        ];

    }//end projectNameDetectionProvider()


}//end class
