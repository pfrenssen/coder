<?php
// $Id$

/**
 * @file
 * Coder format shell invocation script.
 * 
 * @param string $sourcefile
 * 	 The file containing the source code to process, optionally including a
 *   filepath.
 * @param string $targetfile
 * 	 The file to save the formatted source code to, optionally including a
 *   filepath.
 * @param string --batch-replace $path
 * 	 A path to process recursively. Each file in the given path will be replaced
 *   with the formatted source code. Each original file is automatically backed
 * 	 up to <filename>.coder.orig.
 * @param string --undo $path
 *   A path to search for *.coder.orig files and revert changes. Each formatted
 *   file (without extension .coder.orig) is replaced with its backup file.
 * @param string --file-inc $file_inc
 *   The full path to file.inc in a Drupal installation. Only needed if the
 *   path provided for --batch-replace or --undo is not the root directory of a
 *   Drupal installation. Allows to recursively format a CVS checkout folder.
 * 
 * @usage
 *   php coder_format.php <sourcefile> <targetfile>
 *   php coder_format.php --batch-replace <path> [--file-inc <path_to_file.inc>]
 *   php coder_format.php --undo <path> [--file-inc <path_to_file.inc>]
 * 
 * @example
 *   php coder_format.php node.module node.module.formatted
 *   php coder_format.php node.module node.module
 *   php coder_format.php --batch-replace /home/drupal
 *   php coder_format.php --undo /home/drupal
 * 
 * If either --batch-replace or --undo parameter is given, Coder Format assumes
 * that the specified path is the root of a Drupal installation. Coder Format
 * needs includes/file.inc for batch processing. You can optionally specify the
 * location of file.inc with the --file-inc parameter.
 * 
 * Notes:
 * - Encapsulate file and path arguments with double quotes on Windows.
 */

require_once realpath(dirname($_SERVER['PHP_SELF'])) .'/coder_format.inc';

if (!empty($_SERVER['argv'])) {
  // Remove self-reference.
  array_shift($_SERVER['argv']);
  
  $undo     = false;
  $file_inc = null;
  
  foreach ($_SERVER['argv'] as $arg) {
    switch ($arg) {
      case '--undo':
        $op   = array_shift($_SERVER['argv']);
        $root = array_shift($_SERVER['argv']);
        $undo = true;
        coder_format_recursive($root, true);
        break;
        
      case '--batch-replace':
        $op   = array_shift($_SERVER['argv']);
        $root = array_shift($_SERVER['argv']);
        break;
        
      case '--file-inc':
        array_shift($_SERVER['argv']);
        $file_inc = array_shift($_SERVER['argv']);
        break;
    }
  }
  
  if (isset($root)) {
    coder_format_recursive($root, $undo, $file_inc);
    exit;
  }
  
  // Process a single file.
  $sourcefile = array_shift($_SERVER['argv']);
  $targetfile = array_shift($_SERVER['argv']);
  
  coder_format_file($sourcefile, $targetfile);
}


