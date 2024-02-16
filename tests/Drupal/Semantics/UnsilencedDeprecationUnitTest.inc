<?php

/**
 * @file
 * Test data for the relaxed version of FunctionUnsilencedDeprecationSniff coding standard.
 */

// No second parameter, so cannot fail it, both silenced and unsilenced.
@trigger_error('CommentTestBase is deprecated in drupal 8.4.0');
trigger_error('CommentTestBase is deprecated in drupal 8.4.0');

// Not E_USER_DEPRECATED, so cannot fail it, both silenced and unsilenced.
@trigger_error('CommentTestBase is deprecated in drupal 8.4.0', E_USER_SOMETHING_ELSE);
trigger_error('CommentTestBase is deprecated in drupal 8.4.0', E_USER_SOMETHING_ELSE);

// E_USER_DEPRECATED, so silenced is fine...
@trigger_error('CommentTestBase is deprecated in drupal 8.4.0', E_USER_DEPRECATED);
// ... but unsilenced fails.
trigger_error('CommentTestBase is deprecated in drupal 8.4.0', E_USER_DEPRECATED);
