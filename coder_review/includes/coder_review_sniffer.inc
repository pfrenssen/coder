<?php
/**
 * @file
 * This include file implements coder functionality for PHP_CodeSniffer.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_sniffer_reviews() {
  $rules[] = array(
    '#type' => 'callback',
    '#value' => '_coder_review_sniffer_callback',
  );

  $review = array(
    '#title' => 'Drupal CodeSniffer',
    '#link' => 'http://drupal.org/project/drupalcs',
    '#rules' => $rules,
    '#version' => 2,
    '#image' => 'images/sniffer.png',
  );

  return array('sniffer' => $review);
}

/**
 * Define the rule callbacks for code sniffer.
 */
function _coder_review_sniffer_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // Include the CodeSniffer class once.
  include_once 'PHP/CodeSniffer.php';
  if (!class_exists('PHP_CodeSniffer')) {
    _message(_t('PHP/CodeSniffer.php not found'), 'error');
    return;
  }

  // Allocate the CodeSniffer object once.
  static $phpcs, $phpcs_dir, $phpcs_cli;
  if (!isset($phpcs)) {
    // Allocate the CodeSniffer object.
    // Save the CodeSniffer directory, and restore our default directory.
    $current_dir = getcwd();
    $phpcs = new PHP_CodeSniffer();
    $phpcs_dir = getcwd();
    chdir($current_dir);

    // Bypass the CodeSniffer client which will always look at the command line options.
    class CoderReviewCodeSnifferClient extends PHP_CodeSniffer_CLI {
      protected $values = array();

      public function __construct($allowed_extensions = array()) {
        $this->values = $this->getDefaults();
        $this->values['extensions'] = implode(',', $allowed_extensions);
        $this->errorSeverity = PHPCS_DEFAULT_ERROR_SEV;
        $this->warningSeverity = PHPCS_DEFAULT_WARN_SEV;
      }

      public function getCommandLineValues() {
        return $this->values;
      }
    };

    $allowed_extensions = array_merge(
      $coder_args['#php_extensions'],
      $coder_args['#include_extensions'],
      array('module')
    );
    $phpcs_cli = new CoderReviewCodeSnifferClient($allowed_extensions);
    $phpcs->setCli($phpcs_cli);

    $phpcs->setTokenListeners('Drupal', array());
    $phpcs->populateCustomRules();
    $phpcs->populateTokenListeners();
  }

  // Process this file.
  // Restore the CodeSniffer directory during file processing.
  // Then restore it back to our directory.
  $current_dir = getcwd();
  chdir($phpcs_dir);
  $filename = $coder_args['#filename'];
  $phpcs_file = $phpcs->processFile($filename); // , $coder_args['#raw_contents']);
  chdir($current_dir);

  // Read the errors.
  $errors = $phpcs_file->getErrors();
  $warnings = $phpcs_file->getWarnings();
  if ($errors || $warnings) {
    $ignores = $coder_args['#ignore_lines'];
    $ignores = (empty($ignores) ? array() : $ignores[$review['#review_name']]);

    _coder_review_sniffer_warnings($results, $lines, $ignores, $errors);
    _coder_review_sniffer_warnings($results, $lines, $ignores, $warnings);
  }
}

/**
 * Store the Code Sniffer warnings as Coder warnings.
 */
function _coder_review_sniffer_warnings(&$results, $lines, $ignores, $sniffer_warnings) {
  foreach ($sniffer_warnings as $sniffer_error_no => $sniffer_warning) {
    foreach ($sniffer_warning as $lineno => $warning) {
      $rule['#warning'] = $warning[0]['message'];
      // @todo: set the severiy name.
      $severity_name = 'minor'; // $warning[0]['severity'];
      _coder_review_error($results, $rule, 'sniffer', $severity_name, $lineno, $line[$lineno], $ignores);
    }
  }
}
