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
            ->will($this->returnValue(dirname(__FILE__).'/modules/drupal6/test.php'));

        $this->assertEquals(Project::getInfoFile($this->phpcsFile), dirname(__FILE__).'/modules/drupal6/testmodule.info');

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
            ->will($this->returnValue(dirname(__FILE__).'/modules/drupal6/nested/test.php'));

        $this->assertEquals(Project::getInfoFile($this->phpcsFile), dirname(__FILE__).'/modules/drupal6/testmodule.info');

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
                dirname(__FILE__).'/modules/drupal6/nested/test.php',
                '6.x',
            ],
            [
                dirname(__FILE__).'/modules/drupal7/test.php',
                '7.x',
            ],
            [
                dirname(__FILE__).'/modules/drupal8/test.php',
                '8.x',
            ],
        ];

    }//end coreVersionProvider()


}//end class
