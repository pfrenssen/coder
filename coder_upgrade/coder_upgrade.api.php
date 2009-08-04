<?php
// $Id$

/**
 * @file
 * Hooks provided by the Coder Upgrade module.
 * 
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Declare upgrade sets for an API (or set of APIs).
 *
 * This hook allows contributed modules to declare upgrade sets for an API
 * supplied by their module, another contributed module, or a core module. Each
 * upgrade set may optionally include a list of conversion routines.
 * 
 * For example, if your module is called "your_module_name," then declare an
 * upgrade set as:
 * @code
 *   function your_module_name_upgrades($include_routines) {
 *     $routines = $include_routines ? your_module_name_conversions_list() : array();
 *     $upgrade = array(
 *       'title' => t('Your module API changes from 6.x to 7.x'),
 *       'link' => 'http://...',
 *       'routines' => $routines,
 *     );
 *     return array('your_module_name' => $upgrade);
 *   }
 * @endcode
 * 
 * The naming convention for a conversion routine is:
 * @code
 *   your_module_name_convert_your_routine_name();
 * @endcode
 * 
 * The array returned by your_module_name_conversions_list() should be a list
 * of the suffixes of your conversion routines. The suffix is the portion of
 * the routine name following "_convert_" above.
 * @code
 *   function your_module_name_conversions_list() {
 *     return array(
 *       'your_routine_name',
 *     );
 *   }
 * @endcode
 *
 * The $file parameter passed to your conversion routine is a reference to the
 * text of the code file to be upgraded. Be sure to include an ampersand before
 * $file so that your changes will be incorporated into the output code file.
 * @code
 *   function your_module_name_convert_your_routine_name(&$file) {
 *     // Do something to $file.
 *   }
 * @endcode
 * 
 * To use the sample functions included in this api file:
 * - Copy and paste the sample functions to a file in your module.
 * - Replace "your_module_name" with your actual module name.
 * - Complete the conversions_list() routine.
 * - Duplicate the sample conversion routine for each entry in your list,
 *   replacing "your_routine_name" with the appropriate value and changing the
 *   comment block to describe the upgrade applied by the routine.
 * 
 * @param boolean $include_routines
 *   Indicates whether to include the list of conversion routines. This list
 *   is only needed when the conversions are to be applied (typically on form
 *   submission). This parameter equals FALSE when this hook is invoked by
 *   Coder Upgrade while building the form, and TRUE when applying the
 *   conversions.
 */
function hook_upgrades($include_routines)  {
  $routines = $include_routines ? your_module_name_conversions_list() : array();
  $upgrade = array(
    'title' => t('Your module API changes from 6.x to 7.x'),
    'link' => 'http://...',
    'routines' => $routines,
  );
  return array('your_module_name' => $upgrade);
}

/**
 * @} End of "addtogroup hooks".
 */

/**
 * Sample functions.
 */

/**
 * Implement hook_upgrades().
 */
function your_module_name_upgrades($include_routines)  {
  $routines = $include_routines ? your_module_name_conversions_list() : array();
  $upgrade = array(
    'title' => t('Your module API changes from 6.x to 7.x'),
    'link' => 'http://...',
    'routines' => $routines,
  );
  return array('your_module_name' => $upgrade);
}

/**
 * Return a list of conversion routine suffixes.
 *
 * The complete function name that Coder Upgrade searches for is:
 *   <your_module_name> . '_convert_' . <your_routine_name>
 *
 * The entries in the array returned from this routine should only contain
 * the <your_routine_name> portion.
 *
 * @return array
 *   Array of conversion routine suffixes.
 */
function your_module_name_conversions_list() {
  return array(
    'your_routine_name',
  );
}

/**
 * Describe the upgrade applied by this routine.
 *
 * @param string $file
 *   The text of the file to convert.
 */
function your_module_name_convert_your_routine_name(&$file) {
  // Do something to $file.
}
