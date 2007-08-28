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
  // Remove self-reference
  array_shift($_SERVER['argv']);
  
  $files    = array();
  $undo     = false;
  $file_inc = null;
  
  for ($c = 0, $cc = count($_SERVER['argv']); $c < $cc; ++$c) {
    switch ($_SERVER['argv'][$c]) {
      case '--undo':
        ++$c;
        $root = $_SERVER['argv'][$c];
        $undo = true;
        break;
        
      case '--batch-replace':
        ++$c;
        $root = $_SERVER['argv'][$c];
        break;
        
      case '--file-inc':
        ++$c;
        $file_inc = $_SERVER['argv'][$c];
        break;
      
      default:
        $files[] = $_SERVER['argv'][$c];
        break;
    }
  }
  
  if (isset($root)) {
    coder_format_recursive($root, $undo, $file_inc);
  }
  else {
    // Process a single file
    $sourcefile = $files[0];
    $targetfile = $files[1];
    
    coder_format_file($sourcefile, $targetfile);
  }
}


