-------------------------------------------------------------------------------
                            Drupal Code Sniffer
-------------------------------------------------------------------------------

Drupal Code Sniffer (drupalcs) is a coding standard validation tool for Drupal
and contributed modules/themes.

Intallation
-----------

Requirements:
  - PEAR
  - PHPCS

- Install PEAR  ( http://pear.php.net/manual/en/installation.php )
- Install PHPCS ( http://pear.php.net/package/PHP_CodeSniffer )
- Sym-link the drupalcs directory into the standards folder for PHP_CodeSniffer.
  The code for that looks like this:

$> sudo ln -sv /path/to/drupalcs/Drupal $(pear config-get php_dir)/PHP/CodeSniffer/Standards 


Usage (running in a shell)
--------------------------

$> phpcs --standard=Drupal --extensions=php,module,inc,install,test,profile,theme /path/to/drupal_module


Installation in Eclipse
-----------------------

Requirements:
Eclipse : http://www.eclipse.org
Eclipse PTI (Php Tools Integration) : http://www.phpsrc.org

- Install Eclipse http://drupal.org/node/75242
- Install PTI Plugin
- Configure PTI:
  - Open Eclipse preferences -> PHP Tools -> PHP Codesniffer:
    - Add a new "CodeSniffer Standard" - choose the folder Drupal in this module.
    - Activate the library by checking the checkbox next to its name.
    - Make sure the Standard Tab Widht configuration is set to 0. Otherwise you won't get notified about evil tabs in the code.

Now you'll have a PHP Tools entry in the right click menu in the navigator.
Or you can just hit "Validate" within the right click menu of a file.

Recommendation:
- Disable the autorun of validators to save performance.
- Configure the "CodeSniffer Standard" to use per project.

Attention:
This is still a draft!!
Please cross check with http://drupal.org/coding-standards and
http://drupal.org/project/coder if the validation is correct

Known Issues:
Documentation Tags just rarly supported - there are many missing / disabled sniffs