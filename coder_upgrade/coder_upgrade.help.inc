<?php
// $Id$

/**
 * @file
 * Help file for the coder_upgrade module.
 *
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Implementation of hook_help().
 */
function coder_upgrade_help($path) {
  $input = file_directory_path() . '/' . variable_get('coder_upgrade_dir_old', DEADWOOD_OLD);
  $output = file_directory_path() . '/' . variable_get('coder_upgrade_dir_new', DEADWOOD_NEW);
  switch ($path) {
    case 'admin/settings/coder/upgrade':
      return tp('This is a list of the variables used by the module conversion suite.');
//    case 'admin/settings/coder/upgrade/api':
//      return tp('The purpose of this module is to automate as much as possible the task of updating a contributed module for Drupal API changes.' .
//                ' This particular version of the module helps update the 6.x version of a contributed module to the version 7.x API.') .
//             tp(' The Drupal API changes from version 6.x to 7.x are listed below.' .
//                ' The second column indicates the availability of conversion code. Check the box next to each category to be included in the set of default conversions applied.' .
//                ' The default choices can be overridden on the !link page.',
//                 array('!link' => l(t('Conversions Build'), 'admin/build/coder/upgrade')));
    case 'admin/development/coder/upgrade':
      return tp('The purpose of this module is to automate as much as possible the task of updating a contributed module for Drupal API changes.' .
                ' This particular module helps update the 6.x version of a contributed module to the version 7.x API.' .
                ' For specific instructions on this page and the module conversion process, click the "more help" link below.');
    case 'admin/help#coder_upgrade':
      return tp('The purpose of this module is to automate as much as possible the task of updating a contributed module for Drupal API changes.' .
                ' This particular module helps update the 6.x version of a contributed module to the version 7.x API.') .
             tp('The conversion process involves searching the module files (i.e., those files with extensions of .info, .install, .module, .inc and .php) for 6.x syntax and replacing the text with the 7.x equivalent.' .
                ' In many cases, this can be easily done in an automated fashion.' .
                ' However, some of the API changes do not easily lend themselves to search and replace automation.' .
                ' In those cases, and when a conversion routine has not been provided, manual intervention by the module maintainer is required.') .
             tp('As a module updater, you may want to restrict the automated conversions that are performed by this module. The default conversions applied are !link.',
                 array('!link' => l(t('set here'), 'admin/settings/coder/upgrade/api'))) .
             tp('<b>Conversion Process</b>') .
             tp('Begin the conversion process by uploading your module\'s files to a directory beneath the module input directory (currently set to !input).' .
                ' A directory (with the same name) for the converted files will automatically be created for you beneath the module output directory (currently set to !output).' .
                ' You may define the name of the module input and output directories !link.',
                 array('!input' => $input,
                       '!output' => $output,
                       '!link' => l(t('here'), 'admin/settings/coder/upgrade'))) .
             tp('To perform the conversions, go to the !link page, select the appropriate runtime parameters based on the instructions below, and click the Run button.' .
                ' In the instructions below, references to checkboxes and other interface items apply to the build page.',
                 array('!input' => $input,
                       '!link' => l(t('Conversions Build'), 'admin/build/coder/upgrade'))) .
             tp('To convert 6.x files to 7.x files do the following:') .
              t('<ol>
                 <li>Select the file types to convert by checking the appropriate boxes in the table.</li>
                 <li>Select the directory containing the files to be converted.</li>
                 <li>Select the conversions to be done by expanding the  "Conversions to apply" fieldset and checking the appropriate boxes therein. The boxes will be initially set based on the default conversions you defined !link.</li>
                 <li>Click the Run button.</li>
                 </ol>
                 The conversions will be done as follows:
                 <ol>
                 <li>The selected directory in the module input directory (!input) will be copied to the module output directory (!output).</li>
                 <li>Files in the output directory will be reviewed one at a time and the selected conversions applied.</li>
                 </ol>
                 After the automated conversions are made:
                 <ol>
                 <li>Review the changes made (using your favorite directory or file comparison utility).</li>
                 <li>Review the "TODO" comments and manually change any remaining conversion items.</li>
                 <li>Copy the directory from the module output directory to the 7.x installation directory for your contributed modules (e.g., sites/all/modules or sites/default/modules).</li>
                 <li>Install the module through the !install and test it. Good luck!</li>
                 </ol>',
                 array('!input' => $input,
                       '!output' => $output,
                       '!link' => l(t('here'), 'admin/settings/coder/upgrade/api'),
                       '!install' => l(t('Drupal interface'), 'admin/build/modules'))) .
             tp('<b>Manual Changes</b>') .
             tp('In some cases, if syntax conversion was not made for a particular conversion item, we have attempted to highlight the applicable portions of the code with a "TODO" message or to provide a single message at the top of the file.' .
                ' As the marking of code is not the primary objective of this module, you may want to install the !coder and use it to mark the areas in your module\'s code needing manual review.',
                 array('!coder' => l(t('Coder Module'), 'http://drupal.org/project/coder/'))) .
             tp('<b>Feedback</b>') .
             tp('If any of the automated conversions are not successful with your module, please submit a project issue to the !issue if you have reason to believe the code could be changed to handle your module\'s code.' .
                ' If you can supply a patch for the change, that would be greatly appreciated too.',
                 array('!issue' => l(t('Coder Upgrade Module'), 'http://drupal.org/project/coder/upgrade/')));
  }
}
