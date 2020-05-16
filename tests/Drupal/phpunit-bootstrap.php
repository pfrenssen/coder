<?php

require 'vendor/autoload.php';
require 'CoderSniffUnitTest.php';
require 'vendor/squizlabs/php_codesniffer/autoload.php';
// PHPStan does not find the constants in this file, so load it manually.
require_once 'vendor/squizlabs/php_codesniffer/src/Util/Tokens.php';
