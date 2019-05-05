<?php

namespace MyNamespace\triggerErrorTest;

/**
 * @file
 * Example code for strict deprecated FunctionTriggerError sniff test.
 */

// This conforms the strict version of the trigger_error deprecation standard.
@trigger_error('Class \foo is deprecated in drupal:8.5.0 and is removed from drupal:9.0.0. Use \bar instead. See https://www.drupal.org/node/123', E_USER_DEPRECATED);

// This fails the strict version of the trigger_error deprecation standard.
@trigger_error('Class \foo is deprecated in drupal:8.5.0 but instead use \bar in drupal:9.0.0. See https://www.drupal.org/node/123', E_USER_DEPRECATED);

/**
 * Test function one.
 *
 * @deprecated in drupal:8.5.0 and is removed from drupal:9.0.0.
 *   Use class bar instead.
 * @see https://www.drupal.org/node/123
 */
class foo {
}

