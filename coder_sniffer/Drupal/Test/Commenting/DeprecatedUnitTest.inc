<?php

/**
 * @file
 *
 * @deprecated in drupal:8.4.0 and is removed from drupal:8.6.0.
 * Use something else instead.
 * This file doc block passes the 'Deprecated' standards checks.
 *
 * @see https://www.drupal.org/node/1234
 */

/**
 * This doc block will also pass the 'Deprecated' standards checks.
 *
 * @deprecated in drupal:8.6.0 and is removed from drupal:9.0.0.
 *   This function block is OK.
 * @see https://www.drupal.org/project/drupal/issues/5678
 */
function x() {
}

/**
 * @deprecated in Drupal 8.4.
 *   This will fail basic layout.
 *   It also has no 'see' url.
 */

/**
 * This is here to ensure that the 'see' url has to be in the same block.
 * @see https://www.drupal.org/node/5678
 */

/**
 * This block will produce three warnings.
 *
 * @Deprecated in drupal 8.4.0 and is removed from drupal:9.0.
 *   Both version formats are incorrect, and the see url is wrong.
 * @see http:drupal.org
 */
function y() {
}

/**
 * This block will produce three warnings.
 *
 * @deprecated in My_Proj:8.x-1.8 and is removed from my_Proj:8.x-2.
 *   Both version formats are incorrect, and the see url ends with a period.
 * @see http://www.drupal.org/node/7890.
 */
function z() {
}

/**
 * This doc block will pass the 'Deprecated' standards checks.
 *
 * @deprecated in myproj:8.x-1.8 and is removed from myproj:8.x-2.0.
 *   This function block is OK.
 * @see http://www.drupal.org/node/7890
 */
