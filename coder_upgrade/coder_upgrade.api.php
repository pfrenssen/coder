<?php
// $Id$

/**
 * @file
 * Hooks provided by the Coder Upgrade module.
 *
 * Copyright 2009-10 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alters a function call using the grammar parser.
 *
 * This hook allows contributed modules to alter a function call object using
 * the grammar parser. The function call may be a stand-alone statement or part
 * of an expression in another statement. For example:
 * @code
 *   foo($bar); // Stand-alone.
 *   if (foo($bar)) { // Embedded.
 *     // Do something.
 *   }
 * @endcode
 *
 * Coder Upgrade will call this alter hook for each function call in the file
 * that was parsed. Refer to the grammar parser documentation for details of the
 * function call object.
 *
 * @see PGPFunctionCall
 *
 * @param PGPFunctionCall $item
 *   A function call object of the expression or statement.
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function hook_upgrade_call_FUNCTION_NAME_alter(&$item, &$reader) {
  // Change the function name.
  $item->name = 'new_name';

  if ($item->parameters->count() > 0) {
    // Delete the first parameter.
    $item->deleteParameter();
  }
}

/**
 * Alters a hook function using the grammar parser.
 *
 * This hook allows contributed modules to alter a function object using the
 * grammar parser. The function block may be inside an interface or class, or a
 * stand-alone statement block. For example:
 * @code
 *   function foo($bar) { // Stand-alone.
 *     if ($bar) {
 *       // Do something.
 *     }
 *   }
 *   class example {
 *     function foo($bar) { // Embedded.
 *       if ($bar) {
 *         // Do something.
 *       }
 *     }
 *   }
 * @endcode
 *
 * Coder Upgrade will call this alter hook for each hook function in the file
 * that was parsed. However, the function name must follow the naming convention
 * for a hook, i.e, your_module_name_hook. If your module declares a hook for
 * another module or otherwise digresses from the standard naming convention,
 * then you will need to add a converion routine to the list to be able to alter
 * this function.
 *
 * Refer to the grammar parser documentation for details of the function object
 * (i.e. PGPClass).
 *
 * @see hook_upgrades
 * @see PGPClass
 *
 * @param PGPNode $node
 *   A node object containing a PGPClass (or function) item.
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function hook_upgrade_hook_HOOK_NAME_alter(&$node, &$reader) {
  global $_coder_upgrade_module_name;

  // Get the function object.
  $item = &$node->data;

  // Rename the function.
  $item->name = $_coder_upgrade_module_name . '_new_hook_name';
  // Update the document comment.
  $item->comment['value'] = preg_replace('@\* Implement\s+@', "* Implements ", $item->comment['value']);

  if ($item->parameters->count() > 1) {
    // Switch the first two parameters.
    $p0 = $item->getParameter(0);
    $p1 = $item->getParameter(1);
    $item->setParameter(0, $p1);
    $item->setParameter(1, $p0);
  }
}

/**
 * Declares upgrade sets for an API (or set of APIs).
 *
 * This hook should only be used if the conversion routines are other than a
 * change to a function call or to a hook function. The Coder Upgrade module
 * already invokes an alter hook that allows any function call or hook function
 * to be changed.
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
 *
 * Group changes by processing order (beginning, middle, or end). The beginning
 * and end conversion routines are applied only at the module or directory
 * level and the routine will need to determine what to do and how (regex or
 * parser) to do it.
 *
 * The middle changes are applied at the file level and the list entry must
 * specify the handler (regex or parser). The regex changes are applied before
 * the parser changes.
 * @code
 *   function your_module_name_conversions_list() {
 *     $list = array(
 *       'beginning' => array(),
 *       'middle' => array(),
 *       'end' => array(),
 *     );
 *
 *     $list['middle'] = array(
 *       array(
 *         'name' => 'your_routine_name_regex',
 *         'type' => 'regex',
 *         'file' => 'your_module_name.regex_conversions.inc',
 *       ),
 *       array(
 *         'name' => 'your_routine_name_parser',
 *         'type' => 'parser',
 *         'file' => 'your_module_name.parser_conversions.inc',
 *       ),
 *     );
 *
 *     return $list;
 *   }
 * @endcode
 *
 * The $file parameter passed to your regex conversion routine is a reference
 * to the text of the code file to be upgraded. Be sure to include an ampersand
 * before $file so that your changes will be incorporated into the output code
 * file.
 * @code
 *   function your_module_name_convert_your_routine_name_regex(&$file) {
 *     // Do something to $file.
 *   }
 * @endcode
 *
 * The $reader parameter passed to your parser conversion routine is a
 * reference to the object containing the grammar statements of the file to
 * convert. In PHP5, an ampersand is not needed before $reader as objects are
 * automatically passed by reference.
 * @code
 *   function your_module_name_convert_your_routine_name_parser(&$reader) {
 *     // Do something with $reader.
 *   }
 * @endcode
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
 *
 * To use the sample functions included in this api file:
 * - Copy and paste the sample functions to a file in your module.
 * - Replace "your_module_name" with your actual module name.
 * - Replace "function_name" and "hook_name" with actual names.
 * - Complete the conversions_list() routine.
 * - Duplicate the sample conversion routine for each entry in your list,
 *   replacing "your_routine_name" with the appropriate value and changing the
 *   comment block to describe the upgrade applied by the routine.
 */

/**
 * Implements hook_upgrade_call_function_name_alter().
 */
function your_module_name_upgrade_call_function_name_alter(&$item, &$reader) {
  // Change the function name.
  $item->name = 'new_name';

  if ($item->parameters->count() > 0) {
    // Delete the first parameter.
    $item->deleteParameter();
  }
}

/**
 * Implements hook_upgrade_hook_hook_name_alter().
 */
function your_module_name_upgrade_hook_hook_name_alter(&$node, &$reader) {
  global $_coder_upgrade_module_name;

  // Get the function object.
  $item = &$node->data;

  // Rename the function.
  $item->name = $_coder_upgrade_module_name . '_new_hook_name';
  // Update the document comment.
  $item->comment['value'] = preg_replace('@\* Implement\s+@', "* Implements ", $item->comment['value']);

  if ($item->parameters->count() > 1) {
    // Switch the first two parameters.
    $p0 = $item->getParameter(0);
    $p1 = $item->getParameter(1);
    $item->setParameter(0, $p1);
    $item->setParameter(1, $p0);
  }
}

/**
 * Implements hook_upgrades().
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
 * Returns a list of conversion routine suffixes.
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
  $list = array(
    'beginning' => array(),
    'middle' => array(),
    'end' => array(),
  );

  $list['middle'] = array(
    array(
      'name' => 'your_routine_name_regex',
      'type' => 'regex',
      'file' => 'your_module_name.regex_conversions.inc',
    ),
    array(
      'name' => 'your_routine_name_parser',
      'type' => 'parser',
      'file' => 'your_module_name.parser_conversions.inc',
    ),
  );

  return $list;
}

/**
 * Describe the upgrade applied by this routine.
 *
 * @param string $file
 *   The text of the file to convert.
 */
function your_module_name_convert_your_routine_name_regex(&$file) {
  // Do something to $file.
  $hook = 'your_changes'; // Used as the label in the log file.
  $cur = $file;
  $new = $cur;

  $from = array();
  $to = array();

  $from[] = '/(your_module_name)/';
  $to[] = "$1";

  coder_upgrade_do_conversions($from, $to, $new);
  coder_upgrade_save_changes($cur, $new, $file, $hook);
}

/**
 * Describe the upgrade applied by this routine.
 *
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function your_module_name_convert_your_routine_name_parser(&$reader) {
  // Do something with $reader.
}
