<?php
// $Id$

/**
 * @file
 * Invokes the Coder Upgrade conversion routines as a separate process.
 *
 * Using this script:
 * - helps to minimize the memory usage by the web interface process
 * - helps to avoid hitting memory and processing time limits by the PHP process
 * - enables a batch processing workflow
 *
 * Parameters to this script:
 * @param string $parameters
 *   Path to a file containing runtime parameters
 *
 * The parameters should be stored as the serialized value of an associative
 * array with the following keys:
 * - paths: paths to files and modules
 * - theme_cache: path to core theme information cache
 * - variables: variables used by coder_upgrade
 * - upgrades: array to be passed to coder_upgrade_start()
 * - extensions: ditto
 * - items: ditto
 *
 * @see coder_upgrade_conversions_prepare()
 * @see coder_upgrade_parameters_save()
 * @see coder_upgrade_start()
 *
 * To execute this script, run a command from the directory the standard output
 * file is written to:
 * - curl "http://dev.d7x.loc/coder_upgrade.run.php" > coder-upgrade-run.txt
 *
 * Copyright 2009-10 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

echo "Start\n";
echo 'Peak 1: ' . number_format(memory_get_peak_usage(TRUE), 0, '.', ',') . " bytes\n";
echo 'Curr 1: ' . number_format(memory_get_usage(TRUE), 0, '.', ',') . " bytes\n";

/**
 * Root directory of Drupal installation.
 */
define('DRUPAL_ROOT', getcwd());

ini_set('display_errors', 1);
ini_set('memory_limit', '128M');
ini_set('max_execution_time', 180);
set_error_handler("error_handler");
set_exception_handler("exception_handler");

// Read command line arguments.
$path = extract_arguments();
if (is_null($path)) {
  return 'No path to parameter file';
}

// Load runtime parameters.
$parameters = unserialize(file_get_contents($path));

// Extract individual array items by key.
foreach ($parameters as $key => $variable) {
  $$key = $variable;
}

// Set directory paths.
$files_base = $paths['files_base'];
$modules_base = $paths['modules_base'];

// Load parser module so we can log memory use.
require_once DRUPAL_ROOT . '/' . $modules_base . '/grammar_parser/grammar_parser.module';

pgp_log_memory_use('', TRUE);
pgp_log_memory_use('load runtime parameters');

// Load core theme cache.
$upgrade_theme_registry = array();
if (is_file($theme_cache)) {
//   echo "yea, found the theme cache\n";
  $upgrade_theme_registry = unserialize(file_get_contents($theme_cache));
}
pgp_log_memory_use('load core theme cache');

// Load coder_upgrade bootstrap code.
$path = $modules_base . '/coder/coder_upgrade';
$files = array(
  'coder_upgrade.inc',
  'coder_upgrade.module',
  'conversions/coder_upgrade.list.inc',
  'conversions/coder_upgrade.main.inc',
);
foreach ($files as $file) {
  require_once DRUPAL_ROOT . '/' . $path . "/$file";
}

// $trace_base = DRUPAL_ROOT . '/' . $files_base . '/coder_upgrade/coder_upgrade_';
// $trace_file = $trace_base . '1.trace';
// xdebug_start_trace($trace_file);
pgp_log_memory_use('load coder_upgrade bootstrap code');
// xdebug_stop_trace();

// Apply conversion functions.
$success = coder_upgrade_start($upgrades, $extensions, $items);

// $trace_file = $trace_base . '2.trace';
// xdebug_start_trace($trace_file);
pgp_log_memory_use('finish');
// xdebug_stop_trace();

return $success;

/**
 * Returns command line arguments.
 *
 * @return mixed
 *   String or array of command line arguments.
 */
function extract_arguments() {
  switch (php_sapi_name()) {
    case 'apache':
    case 'apache2handler': // This is the value when running curl.
      if (!isset($_GET['file'])) {
        echo 'file parameter is not set';
        return;
      }
      $filename = $_GET['file'];
      $action = isset($_GET['action']) ? $_GET['action'] : '';
      break;

    case 'cli':
      $skip_args = 2;
      if ($_SERVER['argc'] == 2) {
        $skip_args = 1;
      }
      elseif ($_SERVER['argc'] < 2) {
        echo 'file parameter is not set' . "\n";
        return;
      }
      foreach ($_SERVER['argv'] as $index => $arg) {
        // First two arguments are usually script filename and '--'.
        // Sometimes the '--' is omitted.
        if ($index < $skip_args) continue;
        list($key, $value) = explode('=', $arg);
        $arguments[$key] = $value;
      }
      if (!isset($arguments['file'])) {
        echo 'file parameter is not set' . "\n";
        return;
      }
      $filename = $arguments['file'];
      $action = isset($arguments['action']) ? $arguments['action'] : '';
      break;
  }
  return $filename;
}

function exception_handler($e) {
  try {
    // ... normal exception stuff goes here
  }
  catch (Exception $e) {
    print get_class($e) . " thrown within the exception handler. Message: " . $e->getMessage() . " on line " . $e->getLine();
  }
}

function error_handler($code, $message, $file, $line) {
  if (0 == error_reporting()) {
    return;
  }
  throw new ErrorException($message, 0, $code, $file, $line);
}
