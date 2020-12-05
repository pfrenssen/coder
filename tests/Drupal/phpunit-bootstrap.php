<?php

require 'vendor/autoload.php';
require 'tests/Drupal/CoderSniffUnitTest.php';
require 'vendor/squizlabs/php_codesniffer/autoload.php';
// Psalm needs these lines, otherwise does not autoload correctly.
require_once 'vendor/phpunit/phpunit/src/Framework/TestCase.php';
require_once 'vendor/squizlabs/php_codesniffer/src/Util/Tokens.php';
require_once 'vendor/squizlabs/php_codesniffer/src/Util/Common.php';
require_once 'vendor/squizlabs/php_codesniffer/src/Files/LocalFile.php';
require_once 'vendor/squizlabs/php_codesniffer/src/Files/File.php';
require_once 'vendor/squizlabs/php_codesniffer/src/Exceptions/RuntimeException.php';
require_once 'vendor/squizlabs/php_codesniffer/src/Fixer.php';
require_once 'vendor/squizlabs/php_codesniffer/src/Ruleset.php';
