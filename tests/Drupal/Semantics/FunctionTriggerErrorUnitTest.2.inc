<?php

/**
 * @file
 * Example code for strict deprecated FunctionTriggerError sniff test.
 */

/**
 * Test function one.
 *
 * @deprecated in drupal:8.5.0 and is removed from drupal:9.0.0. Use bar()
 *   instead.
 * @see https://www.drupal.org/node/123
 */
function foo1() {
  // This conforms the strict version of the trigger_error deprecation standard.
  @trigger_error('Function foo1() is deprecated in drupal:8.5.0 and is removed from drupal:9.0.0. Use bar() instead. See https://www.drupal.org/node/123', E_USER_DEPRECATED);
}

/**
 * Test function two.
 *
 * @deprecated in drupal:8.5.0 and is removed from drupal:9.0.0. Use bar()
 *   instead.
 * @see https://www.drupal.org/node/123
 */
function foo2() {
  // This fails the strict version of the trigger_error deprecation standard.
  @trigger_error('Function foo2() is deprecated in drupal:8.5.0 but instead use bar() in drupal:9.0.0. See https://www.drupal.org/node/123', E_USER_DEPRECATED);
}

