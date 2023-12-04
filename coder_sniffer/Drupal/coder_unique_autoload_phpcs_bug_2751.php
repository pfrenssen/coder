<?php

/**
 * Includes classes which are not detected by PHP_CodeSniffer's autoloader.
 *
 * This file has a weird name on purpose because of https://github.com/phpcsstandards/PHP_CodeSniffer/pull/2751
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 * @see      https://github.com/phpcsstandards/PHP_CodeSniffer/issues/1469
 */

// Abstract base classes are not discovered by the autoloader.
require_once 'Sniffs/Semantics/FunctionCall.php';
require_once 'Sniffs/Semantics/FunctionDefinition.php';

// Helper classes also need to be included manually.
require_once __DIR__.'/../DrupalPractice/Project.php';
