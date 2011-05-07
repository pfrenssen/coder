Drupal Coding Standards Sniffer for PTI

More infos about this "Project": http://drupal.org/node/897116

Requirements:
Eclipse : http://www.eclipse.org
Eclipse PTI (Php Tools Integration) : http://www.phpsrc.org

Installation:
- Install Eclipse http://drupal.org/node/75242
- Install PTI Plugin
- Configure PTI:
  - Open Eclipse preferences -> PHP Tools -> PHP Codesniffer:
    - Add a new "CodeSniffer Standard" - choose this library
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