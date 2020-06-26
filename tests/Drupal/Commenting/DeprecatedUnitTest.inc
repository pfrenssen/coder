<?php

/**
 * @file
 * This file contains test cases for DeprecatedSniff.
 *
 * @deprecated in drupal:8.4.0 and is removed from drupal:8.6.0.
 * This file doc block passes the 'Deprecated' standards checks.
 *
 * @see https://www.drupal.org/node/1234
 */

/**
 * This doc block will also pass the 'Deprecated' standards checks.
 *
 * @deprecated in drupal:8.6.0 and is removed from drupal:9.0.0. This function
 *   block is OK.
 * @see https://www.drupal.org/project/drupal/issues/5678
 */

/**
 * This block will fail with two errors.
 *
 * @deprecated in Drupal 8.4.
 *   The basic layout is wrong and there is no 'see' url.
 */

/**
 * This shows that a 'see' url without a 'deprecated' tag does not get reported.
 *
 * @see https://www.example.com/incorrect-for-a-deprecated-see-url
 */

/**
 * This block will produce three warnings (but the basic layout is correct).
 *
 * @Deprecated in Drupal 8.4.0 and is removed from 9.x
 *   Both version formats are incorrect, and the see url is wrong.
 * @see http:drupal.org/node/1234
 */
function y() {
}

/**
 * This contrib block will produce three warnings.
 *
 * @deprecated in My_Proj:8.x-1.8 and is removed from my_Proj-8.x-2
 *   Both version formats are incorrect and see url is incorrect.
 * @see http://www.drupal.org/project/issues/1234
 */
function z() {
}

/**
 * This contrib block will pass the 'Deprecated' standards checks.
 *
 * @deprecated in myproj:8.x-1.8 and is removed from myproj:8.x-2.0.
 *   This function block is OK.
 * @see http://www.drupal.org/node/7890
 */

/**
 * This contrib doc block will pass the 'Deprecated' standards checks.
 *
 * @deprecated in project:8.x-1.0-beta3 and is removed from project:8.x-1.0-rc1.
 *   Not stable versions rc, alpha and etc should be correctly accepted.
 * @see http://www.drupal.org/node/7890
 */
function w() {
}

/**
 * This will produce two errors (no text at all, hence bad layout and no see)
 *
 * @deprecated
 */

/**
 * Will give one error for no extra info and one warning for version format.
 *
 * @deprecated in drupal:8.6.0 and is removed from 9.x
 *
 * @see http://www.drupal.org/node/7890
 */

/**
 * This block has incorrect basic layout but is fixable.
 *
 * @deprecated deprecated in Drupal 8.7, removed in 8.9.x There is extra
 *   text before the 'in' version, wrong case, missing :, missing middle text,
 *   missing 'drupal' and incorrect 'removal' version, no period after version.
 * @see http://www.drupal.org/node/123
 */

/**
 * Incorrect text and the url has trailing punctuation. Both are fixable.
 *
 * @deprecated as of 8.8.3-dev, scheduled for removal in DRUPAL 9
 *   It has 'as of' not 'in', missing word 'drupal', has -dev in first version,
 *   has 'removal', upper-case 'drupal' and short second version.
 * @see http://www.drupal.org/node/123?.
 */

/**
 * This contrib doc block will pass the 'Deprecated' standards checks.
 *
 * @deprecated in project:8.1.0 and is removed from project:8.2.0. This uses
 * semantic versioning, but this is now allowed, so we should allow it as well.
 * @see http://www.drupal.org/node/7890
 */
function meanwhile() {
}
