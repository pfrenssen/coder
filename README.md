# Coder

[![Build Status](https://travis-ci.org/klausi/coder.svg?branch=8.x-2.x)](https://travis-ci.org/klausi/coder)

Coder is a library for automated Drupal code reviews and coding standard fixes. It
defines rules for [PHP_CodeSniffer](https://github.com/squizlabs/PHP_CodeSniffer)

Built-in support for:
- "Drupal": Coding Standards https://www.drupal.org/coding-standards
- "DrupalPractice": Best practices for Drupal module development

Online documentation: https://drupal.org/node/1419980


## Installation

First, make sure Composer is installed correctly:
```
which composer
```

If you get composer not found or similar, follow Composer's installation
instructions.

Install Coder (8.x-2.x) in your global Composer directory in your home directory
(~/.composer):
```
composer global require drupal/coder
```

Make the phpcs and phpcbf commands globally available:
```
sudo ln -s ~/.composer/vendor/bin/phpcs /usr/local/bin
sudo ln -s ~/.composer/vendor/bin/phpcbf /usr/local/bin
```
(Alternatively you can add those to your $PATH variable in ~/.profile.)

Register the Drupal and DrupalPractice Standard with PHPCS:
```
phpcs --config-set installed_paths ~/.composer/vendor/drupal/coder/coder_sniffer
```


## Usage

Check Drupal coding standards
```
phpcs --standard=Drupal --extensions=php,module,inc,install,test,profile,theme /file/to/drupal/example_module
```

Check Drupal best practices
```
phpcs --standard=DrupalPractice --extensions=php,module,inc,install,test,profile,theme /file/to/drupal/example_module
```

Automatically fix coding standards
```
phpcbf --standard=Drupal --extensions=php,module,inc,install,test,profile,theme /file/to/drupal/example_module
```


## Working with Editors

Drupal Code Sniffer can be used with various editors.

Editors:

* Eclipse: http://drupal.org/node/1420004
* Komodo: http://drupal.org/node/1419996
* Netbeans: http://drupal.org/node/1420008
* Sublime Text: http://drupal.org/node/1419996
* vim: http://drupal.org/node/1419996


## Automated Testing (PHPUnit)

Coder Sniffer comes with a PHPUnit test suite to make sure the sniffs work correctly.
Use Composer to install the dependencies:

```
composer install
```

Then execute the tests:
```
./vendor/bin/phpunit
```


## Maintainers
Klaus Purer, https://www.drupal.org/u/klausi


## Credits

Greg Sherwood and Squiz Pty Ltd, many sniffs are modified copies of their original
work on [PHPCS](https://github.com/squizlabs/PHP_CodeSniffer).
