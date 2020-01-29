<?php

require 'vendor/autoload.php';
require 'CoderSniffUnitTest.php';
require 'vendor/squizlabs/php_codesniffer/autoload.php';

// Support older versions of PHPUnit on older PHP versions.
if (class_exists('PHPUnit_Framework_TestCase') === true && class_exists('PHPUnit\Framework\TestCase') === false) {
    class_alias('PHPUnit_Framework_TestCase', 'PHPUnit\Framework\TestCase');
}
