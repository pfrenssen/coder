<?php

/**
 * Deprecation check.
 *
 * @deprecated in drupal:8.7.0 and is removed from drupal:9.0.0.
 * Switch off your television set and go and do something less boring instead.
 * @see http://www.drupal.org/node/123
 */
class GoodDeprecation {

  /**
   * Check trigger_error format.
   */
  public function testDeprecation() {
    @trigger_error('Function testDeprecation() is deprecated in drupal:8.5.0 and is removed from drupal:9.0.0. Why Dont You. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
  }

}
