<?php
// $Id$

/**
 * @file
 * This include file implements coder functionality for comments.
 */

/**
 * Implementation of hook_reviews().
 */
function coder_comment_reviews() {
  $rules = array(
    array(
      '#type' => 'grep_invert',
      '#source' => 'comment',
      '#value' => '$Id',
      '#case-sensitive' => TRUE,
      '#warning_callback' => '_coder_comment_Id_warning',
      '#filename-not' => array('patch'),
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^.*\$Id.*$',
      '#not' => '^\/\/\s+\$Id\$|^\/\/\s+\$Id'.':[^\$]+\$',
      '#case-sensitive' => TRUE,
      '#warning_callback' => '_coder_comment_Id_warning',
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
      '#warning_callback' => '_coder_comment_see_comment_style_warning',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*.*',
      '#not' => '^\@see\s+((\w+\(\)|[\w\.\-\?\/:\&=]+\.[\w\.\->\?\/:\&=]+)[,\s]+)*(\w+\(\)|[\w\.\->\?\/:\&=]+\.[\w\.\->\?\/:\&=]+)\W*$',
      '#warning' => '@see should always be followed by a filename, a url or a function name with ()',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*\w+.*$',
      '#not' => '^\@see\s+([\w\.\-\(\)\?\/:\&=]+,\s)*[\w\.\-\(\)\?\/:\&=]+?[\w\(\)\/]$',
      '#warning' => '@see references should be separated by "," followed by a single space and with no trailing punctuation',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s.*$',
      '#not' => '^\@see [^\s]',
      '#warning' => '@see should be separated from the references by one space only',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'grep_invert',
      '#source' => 'comment',
      '#value' => '@'.'file',
      '#warning_callback' => '_coder_comment_missing_file_block',
      '#filename-not' => array('patch'),
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '@'.'file\s+.+$',
      '#warning_callback' => '_coder_comment_invalid_file_block',
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

function _coder_comment_Id_warning() {
  return array(
    '#warning' => t('Include the CVS keyword $Id'.'$ in each file.  This should be in the format <code>// $Id'.'$</code> or <code>// $Id'.'$</code>'),
    '#link' => 'http://drupal.org/coding-standards',
  );
}

function _coder_comment_missing_file_block() {
  return array(
    '#warning' => t('@'.'file block missing'),
    '#link' => 'http://drupal.org/node/1354#files',
  );
}

function _coder_comment_invalid_file_block() {
  return array(
    '#warning' => t('@'.'file description should be on the following line'),
    '#link' => 'http://drupal.org/node/1354#files',
  );
}

function _coder_comment_see_comment_style_warning() {
  return array(
    '#warning' => t('@' . 'see should always be in a comment block rather than as an inline comment.'),
    '#link' => 'http://drupal.org/node/1354',
  );
}

