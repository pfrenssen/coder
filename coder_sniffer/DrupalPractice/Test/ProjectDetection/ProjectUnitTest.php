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
     * @var PHP_CodeSniffer_File|PHPUnit_Framework_MockObject_MockObject
     */
    protected $phpcsFile;

    /**
     * {@inheritdoc}
     */
    public function setUp()
    {
        parent::setUp();
        $this->phpcsFile = $this->getMockBuilder('PHP_CodeSniffer_File')
            ->disableOriginalConstructor()
            ->getMock();
    }

    /**
     * Tests the extending classes Sniff class.
     */
    public function testInfoFileDetection()
    {
				// @see https://www.drupal.org/project/coder/issues/2962880
        $this->markTestIncomplete('This test relies on code that has been removed in PHP_CodeSniffer 3.x.');

        $this->phpcsFile->expects($this->any())
          ->method('getFilename')
          // The file does not exist, but doesn't matter for this test.
          ->will($this->returnValue(dirname(__FILE__) . '/modules/drupal6/test.php'));

        $this->assertEquals(Project::getInfoFile($this->phpcsFile), dirname(__FILE__) . '/modules/drupal6/testmodule.info');

    }

    /**
     * Tests the extending classes Sniff class.
     */
    public function testInfoFileNestedDetection()
    {
				// @see https://www.drupal.org/project/coder/issues/2962880
        $this->markTestIncomplete('This test relies on code that has been removed in PHP_CodeSniffer 3.x.');

        $this->phpcsFile->expects($this->any())
          ->method('getFilename')
          // The file does not exist, but doesn't matter for this test.
          ->will($this->returnValue(dirname(__FILE__) . '/modules/drupal6/nested/test.php'));

        $this->assertEquals(Project::getInfoFile($this->phpcsFile), dirname(__FILE__) . '/modules/drupal6/testmodule.info');
    }

    /**
     * Tests the extending classes Sniff class.
     *
     * @dataProvider coreVersionProvider
     */
    public function testCoreVersion($filename, $core_version)
    {
				// @see https://www.drupal.org/project/coder/issues/2962880
        $this->markTestIncomplete('This test relies on code that has been removed in PHP_CodeSniffer 3.x.');

        $this->phpcsFile->expects($this->any())
          ->method('getFilename')
          // The file does not exist, but doesn't matter for this test.
          ->will($this->returnValue($filename));

        $this->assertEquals(Project::getCoreVersion($this->phpcsFile), $core_version);
    }

    /**
     * Data provider for testCoreVersion().
     */
    public function coreVersionProvider() {
        return array(
            array(dirname(__FILE__) . '/modules/drupal6/nested/test.php', '6.x'),
            array(dirname(__FILE__) . '/modules/drupal7/test.php', '7.x'),
            array(dirname(__FILE__) . '/modules/drupal8/test.php', '8.x'),
        );
    }

}//end class
