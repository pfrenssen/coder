# Coder

[![Build Status](https://travis-ci.org/pfrenssen/coder.svg?branch=8.x-3.x)](https://travis-ci.org/pfrenssen/coder)

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

    which composer

If you get composer not found or similar, follow Composer's installation
instructions.

Install Coder (8.x-3.x) in your global Composer directory in your home directory
(`~/.composer`):

    composer global require drupal/coder

To make the `phpcs` and `phpcbf` commands available globally, add the Composer
bin path to your `$PATH` variable in `~/.profile`, `~/.bashrc` or `~/.zshrc`:

    export PATH="$PATH:$HOME/.composer/vendor/bin"

Register the Drupal and DrupalPractice Standard with PHPCS:

    phpcs --config-set installed_paths ~/.composer/vendor/drupal/coder/coder_sniffer


### Composer Installer Plugins

The Coder package (>= 8.2.11) now works with Composer Installer Plugins,
that find and register standards whenever packages are installed or updated.
To use such a plugin within your project, follow these steps.

    composer require --dev dealerdirect/phpcodesniffer-composer-installer
    composer require --dev drupal/coder

Now, you will see Drupal and DrupalPractice listed in the available PHP
CodeSniffer standards.

    vendor/bin/phpcs -i

The same can be done for a Composer global installation.

    composer global require dealerdirect/phpcodesniffer-composer-installer
    composer global require drupal/coder


## Usage

Check Drupal coding standards

    phpcs --standard=Drupal /file/to/drupal/example_module

Check Drupal best practices

    phpcs --standard=DrupalPractice /file/to/drupal/example_module

Automatically fix coding standards

    phpcbf --standard=Drupal /file/to/drupal/example_module


## Store settings in a phpcs.xml.dist file

In order to save and commit your PHPCS settings to Git you can use a
phpcs.xml.dist file in your project like this:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<ruleset name="myproject">
  <description>PHP CodeSniffer configuration for myproject development.</description>
  <!-- Check all files in the current directory and below. -->
  <file>.</file>
  <arg name="extensions" value="php,module,inc,install,test,profile,theme,css,info,txt,md"/>
  <!-- Change this value to 7 if you want to check Drupal 7 code. -->
  <config name="drupal_core_version" value="8"/>

  <!-- If you have Coder installed locally then you can reference the Drupal
  standards with relative paths. Otherwise simply use "Drupal" and
  "DrupalPractice. -->
  <rule ref="../vendor/drupal/coder/coder_sniffer/Drupal">
    <!-- Example how you would disable a rule you are not compliant with yet:
    <exclude name="Drupal.Commenting.Deprecated"/>
    -->
  </rule>
  <rule ref="../vendor/drupal/coder/coder_sniffer/DrupalPractice"/>

  <!-- Example how you would disable an external rule you do not like:
  <rule ref="PEAR.Functions.ValidDefaultValue.NotAtEnd">
    <severity>0</severity>
  </rule>
  -->
</ruleset>
```

Then you can invoke phpcs without any options and it will read phpcs.xml.dist
from the current directory. This can also be useful for Continues Integration
setups.


## Working with Editors

Drupal Code Sniffer can be used with various editors.

Editors:

- [Atom](https://www.drupal.org/node/1419996)
- [Eclipse](https://www.drupal.org/node/1420004)
- [Komodo](https://www.drupal.org/node/1419996)
- [Netbeans](https://www.drupal.org/node/1420008)
- [PhpStorm](https://www.jetbrains.com/help/phpstorm/php-code-sniffer.html)
- [Sublime Text](https://www.drupal.org/node/1419996)
- [Vim](https://www.drupal.org/node/1419996)
- [Visual Studio Code (VSCode)](https://www.drupal.org/node/1419996)


## Automated Testing (PHPUnit + PHPCS)

Coder Sniffer comes with a PHPUnit test suite to make sure the sniffs work correctly.
Use Composer to install the dependencies:

    composer install

Then execute the tests:

    ./vendor/bin/phpunit

Then execute the coding standards checker on Coder itself:

    ./vendor/bin/phpcs


## Contributing

1. Make sure an issue exists at https://www.drupal.org/project/issues/coder
2. Create a [Pull Request](https://help.github.com/articles/using-pull-requests/) against https://github.com/pfrenssen/coder
3. Post a link to the pull request to the issue on drupal.org and set the issue to
   "needs review"

Thank you!


## Maintainers

- Pieter Frenssen, https://www.drupal.org/u/pfrenssen
- Michael Welford, https://www.drupal.org/u/mikejw
- Klaus Purer, https://www.drupal.org/u/klausi


## Credits

Greg Sherwood and Squiz Pty Ltd, many sniffs are modified copies of their original
work on [PHPCS](https://github.com/squizlabs/PHP_CodeSniffer).
