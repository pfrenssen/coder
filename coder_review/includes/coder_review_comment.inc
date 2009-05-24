<?php
// $Id$

/**
 * @file
 * This include file implements coder functionality for comments.
 */

/**
 * Implementation of hook_reviews().
 */
function coder_review_comment_reviews() {
  $rules = array(
    array(
      '#type' => 'grep_invert',
      '#source' => 'comment',
      '#value' => '$Id',
      '#case-sensitive' => TRUE,
      '#warning_callback' => '_coder_review_comment_Id_warning',
      '#filename-not' => array('patch'),
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^.*\$Id.*$',
      '#not' => '^\/\/\s+\$Id\$|^\/\/\s+\$Id' . ':[^\$]+\$',
      '#case-sensitive' => TRUE,
      '#warning_callback' => '_coder_review_comment_Id_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^\*',
      '#warning' => 'indent secondary line of comment one space ',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^\s*\*.+|^\/\*.+',
      '#not' => '^\s*\*\s+|^\/\*\s+|^\/\*\*|^\s*\*\/',
      '#warning' => 'put a space between the asterisk and the comment text',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '.*\@see\s*',
      '#not' => '^(\s*\*|\/\/)\s*\@see.*$',
      '#warning' => '@see should always be at the beginning of a line, never inline in other comments.',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\/\/\s*\@see\s*',
      '#warning_callback' => '_coder_review_comment_see_comment_style_warning',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*.*',
      '#not' => '^\@see\s*[a-zA-Z0-9_\.\-:]+\(\)',
      '#warning' => '@see should always be followed by a function name with ()',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*.*?[\.\,;]+\s*$',
      '#warning' => '@see should have no trailing punctuation',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*[a-zA-Z0-9_\.:]+\(\)(\s+[a-zA-Z0-9_\.\-]+)+',
      '#warning' => '@see references should be separated from the function name by ", "',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*[a-zA-Z0-9_\.:]+\(\),.*?(\s+[a-zA-Z0-9_\.\-]+){2,}',
      '#warning' => '@see references should be separated by ", "',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'grep_invert',
      '#source' => 'comment',
      '#value' => '@' . 'file',
      '#warning_callback' => '_coder_review_comment_missing_file_block',
      '#filename-not' => array('patch'),
    ),
    array(
      '#type' => 'callback',
      '#value' => '_coder_review_comment_install_file_block',
      // @NOTE: This is not used.  It only exists to catch potential errors in this code.
      '#warning_callback' => '_coder_review_comment_install_file_callback_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '@' . 'file\s+.+$',
      '#warning_callback' => '_coder_review_comment_invalid_file_block',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => 'Implementation\s+of\s+hook_\w+\(\)\s*$',
      '#warning' => 'Missing period',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => 'Implementation\s+of\s+hook_\w+\s*\.*$',
      '#warning' => 'Missing parenthesis after function name',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^.*\s+of\s+hook_\w+',
      '#not' => '^\s\*\sImplementation\s+of\s+hook_\w+',
      '#warning' => 'Format should be <code>* Implementation of hook_foo().</code>',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => 'implementation\s+of\s+hook_\w+',
      '#warning' => '\'Implementation\' should be at the start of the sentence and begin with a capitialized letter',
      '#severity' => 'minor',
      '#case-sensitive' => TRUE,
    ),
  );
  $review = array(
    '#title' => t('Drupal Commenting Standards'),
    '#link' => 'http://drupal.org/node/318',
    '#rules' => $rules,
    '#description' => t('every developer should use'),
  );
  return array('comment' => $review);
}

/**
 * Define the rule callbacks for comment install file block, see do_coder_review_callback().
 */
function _coder_review_comment_install_file_block(&$coder_args, $review, $rule, $lines, &$results) {
  // Only perform this check on install files.
  $filename = $coder_args['#filename'];
  if (substr($filename, -7) == '.install') {
    $file_found = 0;
    $invalid_file_message = 0;
    foreach ($lines as $lineno => $line) {
      if (preg_match('/^ * @file/', $line[0])) {
        $file_found = 1;
      }
      // Immediately reset $file_found on the next line, and check if the
      // comment matches the required format.
      elseif ($file_found == 1) {
        if (!preg_match('/^ * Install, update and uninstall functions for the \w+ module./', $line[0])) {
          $invalid_file_message = 1;
        }
        $file_found = 0;
      }
    }
    if ($invalid_file_message) {
      $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
      $tmprule = $rule;
      $tmprule['#warning_callback'] = '_coder_review_comment_install_file_warning';
      _coder_review_error($results, $tmprule, $severity_name, $theme_lineno, $theme_line);
    }
  }
}

function _coder_review_comment_install_file_warning() {
  return array(
    '#warning' => t('For .install files, the @file description should be of the format "Install, update and uninstall functions for the XXX module.".'),
    '#link' => 'http://drupal.org/coding-standards',
  );
}

function _coder_review_comment_Id_warning() {
  return array(
    '#warning' => t('Include the CVS keyword $Id' . '$ in each file.  This should be in the format <code>// $Id' . '$</code> or <code>// $Id' . '$</code>'),
    '#link' => 'http://drupal.org/coding-standards',
  );
}

function _coder_review_comment_missing_file_block() {
  return array(
    '#warning' => t('@' . 'file block missing'),
    '#link' => 'http://drupal.org/node/1354#files',
  );
}

function _coder_review_comment_invalid_file_block() {
  return array(
    '#warning' => t('@' . 'file description should be on the following line'),
    '#link' => 'http://drupal.org/node/1354#files',
  );
}

function _coder_review_comment_see_comment_style_warning() {
  return array(
    '#warning' => t('@' . 'see should always be in a comment block rather than as an inline comment.'),
    '#link' => 'http://drupal.org/node/1354',
  );
}
