# Validation Tools

This document describes how to run the validation tools for the Coder project. At the end of making changes ensure that all validation tools are passing.

## Installation

Ensure you have installed the project dependencies:

```bash
composer install
```

## PHPUnit

Run the unit tests using PHPUnit. The configuration is loaded from `phpunit.xml.dist`.

```bash
./vendor/bin/phpunit
```

## PHP CodeSniffer (PHPCS)

Run code style checks using PHPCS. The configuration is loaded from `phpcs.xml.dist`.

Fix automatically first:
```bash
./vendor/bin/phpcbf
```

Then verify:
```bash
./vendor/bin/phpcs
```

## PHPStan

Run static analysis using PHPStan. The configuration is loaded from `phpstan.neon`.

```bash
./vendor/bin/phpstan analyze
```
