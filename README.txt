/* $Id$ */

Developer Module that assists with code review and version upgrade that
supports a plug-in extensible hook system so contributed modules can
define additional review standards.

Built-in support for:
 - Drupal Coding Standards - http://drupal.org/node/318
 - Handle text in a secure fashion - http://drupal.org/node/28984
 - Converting 4.6.x modules to 4.7.x - http://drupal.org/node/22218
 - Converting 4.7.x modules to 5.x - http://drupal.org/node/64279

Installation
------------

Copy coder.module to your module directory and then enable on the admin
modules page.  Enable the modules that admin/settings/coder works on,
then view the coder results page at admin/coder.

The built-in rules are not complete.  This is a work in progress.

Todo
----
 - add page argument so that someone can visit admin/coder/modulename
   and see the results for the one modulename
 - display text in a pretty fashion, rather than just line numbers
 - complete the drupal built-in review
 - add the security built-in review
 - add the 50 built-in review
 - add the 47 built-in review (low priority, may not be done)

Author
------
Doug Green
douggreen@douggreenconsulting.com
