# Coder

Coder is a library for automated Drupal code reviews. It defines rules for
[PHP_CodeSniffer](https://github.com/squizlabs/PHP_CodeSniffer)

Built-in support for:
- "Drupal": Coding Standards https://www.drupal.org/coding-standards
- "DrupalPractice": Best practices for Drupal module development

Online documentation: https://drupal.org/node/1419980


## Installation

You will want to use the most recent Coder 8.x-2.x dev code. Edit your
~/.composer/composer.json file and add the minimum stability setting to dev:

```
{
    "minimum-stability": "dev"
}
```

Then install Coder 8.x-2.x (dev version):

```
composer global require drupal/coder:dev-8.x-2.x
```


## Usage

Drupal coding standards:
```
~/.composer/vendor/bin/phpcs --standard=/home/klausi/.composer/vendor/drupal/coder/coder_sniffer/Drupal /path/to/example.module
```

DrupalPractice:
```
~/.composer/vendor/bin/phpcs --standard=/home/klausi/.composer/vendor/drupal/coder/coder_sniffer/DrupalPractice /path/to/example.module
```

Replace /home/klausi with the full path to your home directory. Of course that
command is annoyingly long, so you can setup a shell alias in your ~/.bashrc or
~/-zshrc:

```
alias drupalcs="~/.composer/vendor/bin/phpcs --standard=/home/klausi/.composer/vendor/drupal/coder/coder_sniffer/Drupal --extensions='php,module,inc,install,test,profile,theme,js,css,info,txt'"
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
