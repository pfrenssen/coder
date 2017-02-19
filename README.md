# Coder

[![Build Status](https://travis-ci.org/klausi/coder.svg?branch=8.x-2.x)](https://travis-ci.org/klausi/coder)

Coder is a library for automated Drupal code reviews and coding standard fixes. It
defines rules for [PHP_CodeSniffer](https://github.com/squizlabs/PHP_CodeSniffer)

Built-in support for:
- "Drupal": Coding Standards https://www.drupal.org/coding-standards
- "DrupalPractice": Best practices for Drupal module development

Online documentation: https://www.drupal.org/node/1419980

Note that Javascript support has been removed. To check and fix Javascript files
please use [ESLint](http://eslint.org/) and see the
[Drupal ESLint](https://www.drupal.org/node/1955232) documentation.


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

To make the phpcs and phpcbf commands available globally, add the composer bin path
to your $PATH variable in ~/.profile, ~/.bashrc or ~/.zshrc:
```
export PATH="$PATH:$HOME/.composer/vendor/bin"
```

Register the Drupal and DrupalPractice Standard with PHPCS:
```
phpcs --config-set installed_paths ~/.composer/vendor/drupal/coder/coder_sniffer
```


## Usage

Check Drupal coding standards
```
phpcs --standard=Drupal /file/to/drupal/example_module
```

Check Drupal best practices
```
phpcs --standard=DrupalPractice /file/to/drupal/example_module
```

Automatically fix coding standards
```
phpcbf --standard=Drupal /file/to/drupal/example_module
```


## Working with Editors

Drupal Code Sniffer can be used with various editors.

Editors:

* Eclipse: https://www.drupal.org/node/1420004
* Komodo: https://www.drupal.org/node/1419996
* Netbeans: https://www.drupal.org/node/1420008
* Sublime Text: https://www.drupal.org/node/1419996
* vim: https://www.drupal.org/node/1419996


## Automated Testing (PHPUnit + PHPCS)

Coder Sniffer comes with a PHPUnit test suite to make sure the sniffs work correctly.
Use Composer to install the dependencies:

```
composer install
```

Then execute the tests:
```
./vendor/bin/phpunit
```

Then execute the coding standards checker on Coder itself:
```
./vendor/bin/phpcs
```


## Contributing

1. Make sure an issue exists at https://www.drupal.org/project/issues/coder
2. Create a [pull request](https://help.github.com/articles/using-pull-requests/) against https://github.com/klausi/coder
3. Post a link to the pull request to the issue on drupal.org and set the issue to
   "needs review"

Thank you!


## Maintainers
Klaus Purer, https://www.drupal.org/u/klausi


## Credits

Greg Sherwood and Squiz Pty Ltd, many sniffs are modified copies of their original
work on [PHPCS](https://github.com/squizlabs/PHP_CodeSniffer).
