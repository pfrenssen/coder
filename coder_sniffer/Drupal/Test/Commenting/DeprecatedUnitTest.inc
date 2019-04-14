<?php

/**
 * @file
 *
 * @deprecated in drupal:8.4.0 and will be removed from drupal:8.6.0.
 * Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead.
 * This file doc block passes the 'Deprecated' standards checks.
 *
 * @see https://www.drupal.org/node/1234
 */

/**
 * This doc block will also pass the 'Deprecated' standards checks.
 *
 * @deprecated in drupal:8.6.0 and will be removed from drupal:9.0.0.
 *   This function block is OK.
 * @see https://www.drupal.org/project/drupal/issues/5678
 */
function x() {
}

/**
 * @deprecated in Drupal 8.4. This will fail basic layout.
 *   Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead.
 *   It also has no 'see' url.
 */

/**
 * This is here to ensure that the 'see' url has to be in the same block.
 * @see https://www.drupal.org/node/5678
 */

/**
 * This function does something.
 *
 * @Deprecated in drupal 8.4.0 and will be removed from drupal:9.0.
 *   Both version formats are incorrect, and the see url is wrong.
 * @see http:drupal.org
 */
function y() {
}

/**
 * This function does something.
 *
 * @deprecated in My_Proj:8.x-1.8 and will be removed from my_Proj:8.x-2.
 *   Both version formats are incorrect, and the see url ends with a period.
 * @see http://www.drupal.org/node/7890.
 */
function z() {
}
