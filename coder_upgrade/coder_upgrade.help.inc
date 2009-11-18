<?php
// $Id$

/**
 * @file
 * Help file for the coder_upgrade module.
 *
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Implement hook_help().
 */
function coder_upgrade_help($path) {
  $input = file_directory_path() . '/' . variable_get('coder_upgrade_dir_old', DEADWOOD_OLD);
  $output = file_directory_path() . '/' . variable_get('coder_upgrade_dir_new', DEADWOOD_NEW);
  switch ($path) {
    case 'admin/config/development/coder/upgrade/settings':
      return tp('This is a list of the variables used by the automated module conversion suite.');
    case 'admin/config/development/coder/upgrade':
      return tp('The purpose of this module is to automate as much as possible the task of updating a contributed module for Drupal API changes.' .
                ' This particular module helps update the 6.x version of a contributed module to the 7.x version API.' .
                ' Relative to the Drupal core APIs, the list of automated conversions to be applied is indicated !list.' .
                ' For more detailed instructions on the module conversion process, visit the !help page.',
                 array('!list' => l(t('here'), 'http://drupal.org/node/394070'),
                       '!help' => l(t('help'), 'admin/help/coder_upgrade')));
    case 'admin/help#coder_upgrade':
      return tp('The purpose of this module is to automate as much as possible the task of updating a contributed module for Drupal API changes.' .
                ' This particular module helps update the 6.x version of a contributed module to the 7.x version API.' .
                ' Relative to the Drupal core APIs, the list of automated conversions to be applied is indicated !link.' .
                ' Contributed modules that define an API may add their own list of automated conversions to be applied by utilizing the Upgrades API provided by this module.' .
                ' The Upgrades API is documented in coder_upgrade.api.php.',
                 array('!link' => l(t('here'), 'http://drupal.org/node/394070'))) .
             tp('The conversion process involves searching the module files (i.e., those files with extensions of .info, .install, .module, .inc, .profile, .theme, and .php) for 6.x syntax and replacing the text with the 7.x equivalent.' .
                ' In many cases, this can be easily done in an automated fashion.' .
                ' However, some of the API changes do not easily lend themselves to search and replace automation.' .
                ' In those cases, and when a conversion routine has not been provided, manual intervention by the module maintainer is required.') .
             tp('<b>Conversion Process</b>') .
             tp('Begin the conversion process by uploading your module\'s files to a directory beneath the <b>module input directory</b> (currently set to <b>!input</b>).' .
                ' A directory (with the same name) for the converted files will automatically be created for you beneath the <b>module output directory</b> (currently set to <b>!output</b>).' .
                ' You may define the name of the module input and output directories !link.' .
                ' You may also upload your module to a modules directory beneath the sites directory (e.g. sites/all/modules).',
                  array('!input' => $input,
                       '!output' => $output,
                       '!link' => l(t('here'), 'admin/config/development/coder/upgrade/settings'))) .
             tp('To apply the conversions, go to the !link page, select the appropriate runtime parameters based on the instructions below, and click the Convert files button.' .
                ' In the instructions below, references to checkboxes and other interface items apply to the development page.',
                 array('!input' => $input,
                       '!link' => l(t('Conversions Development'), 'admin/config/development/coder/upgrade'))) .
             tp('To convert 6.x files to 7.x files do the following:') .
              t('<ol>
                 <li>On the Upgrades tab, select the conversion (or upgrade) categories to apply by checking the appropriate boxes in the list.</li>
                 <li>On the Extensions tab, select the file types to convert by checking the appropriate boxes in the list.</li>
                 <li>On the Directories tab, select the directories containing the files to be converted.</li>
                 <li>On the Modules tab, select the modules containing the files to be converted.</li>
                 <li>Click the Convert files button.</li>
                 </ol>
                 The conversions will be done as follows for each of the directories and modules selected in steps 3 and 4 above:
                 <ol>
                 <li>The selected directory or module will be copied to the module output directory (!output).</li>
                 <li>Files in the output directory will be reviewed one at a time and the selected conversions applied.</li>
                 </ol>
                 After the automated conversions are applied:
                 <ol>
                 <li>In the Directories and Modules tab, a patch file link is added to the names of each converted directory or module.</li>
                 <li>Click on the patch file link to review the changes made for each directory or module.</li>
                 <li>Otherwise, review the changes using your favorite directory or file comparison utility.</li>
                 <li>Rerun the !coder to highlight the areas in your module\'s code needing manual review.</li>
                 <li>Copy the directory from the module output directory to the 7.x installation directory for your contributed modules (e.g., sites/all/modules or sites/default/modules).</li>
                 <li>Install the module through the !install and test it.</li>
                 </ol>',
                 array('!input' => $input,
                       '!output' => $output,
                       '!coder' => l(t('Coder Review'), 'admin/config/development/coder/review'),
                       '!install' => l(t('Drupal interface'), 'admin/structure/modules'))) .
             tp('<b>Manual Changes</b>') .
             tp('After applying the automated conversions to your module, rerun the !coder to highlight the areas in your module\'s code needing manual review.',
                 array('!coder' => l(t('Coder Review'), 'admin/config/development/coder/review'))) .
             tp('<b>Feedback</b>') .
             tp('If any of the automated conversions are not successful with your module and you believe the conversion code could be changed to handle your module\'s code, please submit a project issue to the !issue <b>specifying "Coder Upgrade"</b> as the Component.' .
                ' If you can also supply a patch for the change, that would be greatly appreciated too.',
                 array('!issue' => l(t('Coder Module'), 'http://drupal.org/node/add/project-issue/coder')));
  }
}
