<?php
// $Id$

/**
 * @file
 * Other conversion routine file for the coder_upgrade module.
 *
 * The functions in these conversion routine files correspond to the topics in
 * the category roadmap at http://drupal.org/node/394070 that are marked with
 * a "Yes" in the Upgrade column.
 *
 * Copyright 2008-9 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Miscellaneous routine for simple changes.
 *
 * http://drupal.org/node/224333#registry
 * http://drupal.org/node/224333#drupal_set_session (REVERTED)
 * http://drupal.org/node/224333#time
 * http://drupal.org/node/224333#rebuild_functions
 *
 * @param PGPReader $reader
 *   The object containing the grammar statements of the file to convert.
 */
function coder_upgrade_convert_simple_changes(&$reader) {
  $editor = new PGPEditor();
  $types = array(T_STRING);
  $editor->traverse_callback($reader->getFunctionCalls(), 'coder_upgrade_callback_simple_changes', $types);
}

function coder_upgrade_callback_simple_changes(&$item) {
  $name = &$item['name'];
  $value = &$name['value'];
  switch ($name['value']) {
    case 'drupal_rebuild_code_registry':
      $name['value'] = 'registry_rebuild';
      break;

    case 'drupal_rebuild_theme_registry':
      $name['value'] = 'drupal_theme_rebuild';
      break;

    case 'function_exists':
      $name['value'] = 'drupal_function_exists';
      break;

    case 'time':
      $temp = $editor->expressionToStatement('REQUEST_TIME');
      $item = $temp['operand_1'];
      break;
  }
}
