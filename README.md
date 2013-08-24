drupalpractice
==============

DrupalPractice sniffer - Uses [PHP_CodeSniffer](https://github.com/squizlabs/PHP_CodeSniffer)
to find common errors that new Drupal developers make in their modules.

Please report bugs in the [drupalpractice issue queue on drupal.org](https://drupal.org/project/issues/drupalpractice).

Please use Github pull requests to contribute patches.

Installation
------------

Same as [Coder Sniffer](http://drupal.org/node/1419988), except that this
sniffer should be linked as DrupalPractice into PHP_CodeSniffer.

  sudo ln -sv /path/to/drupalpractice/DrupalPractice $(pear config-get php_dir)/PHP/CodeSniffer/Standards/DrupalPractice

Usage examples
--------------

  phpcs --standard=DrupalPractice mymodule.module
  phpcs --standard=DrupalPractice --extensions=php,module,inc,install,test,profile,theme /path/to/module
