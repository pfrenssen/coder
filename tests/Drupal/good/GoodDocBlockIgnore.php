<?php

/**
 * Test class.
 */
class GoodDocBlockIgnore {

  /**
   * The wrapped HTTP kernel.
   */
  protected \Closure $httpKernel;

  /**
   * The entity for this result.
   *
   * @var \Drupal\Core\Entity\EntityInterface
   */
  // phpcs:ignore Drupal.NamingConventions.ValidVariableName.LowerCamelName,PSR2.Classes.PropertyDeclaration.Underscore
  public $_entity = NULL;

  /**
   * Gets a fallback id for a missing plugin.
   *
   * This method should be implemented in extending classes that also implement
   * FallbackPluginManagerInterface. It is called by
   * PluginManagerBase::handlePluginNotFound on the abstract class, and
   * therefore should be defined as well on the abstract class to prevent static
   * analysis errors.
   *
   * @param string $plugin_id
   *   The ID of the missing requested plugin.
   * @param array $configuration
   *   An array of configuration relevant to the plugin instance.
   *
   * phpcs:ignore Drupal.Commenting.FunctionComment.InvalidNoReturn
   * @return string
   *   The id of an existing plugin to use when the plugin does not exist.
   *
   * @throws \BadMethodCallException
   *   If the method is not implemented in the concrete plugin manager class.
   */
  protected function getFallbackPluginId($plugin_id, array $configuration = []) {
    throw new \BadMethodCallException(static::class . '::getFallbackPluginId() not implemented.');
  }

  /**
   * {@inheritdoc}
   *
   * phpcs:ignore Drupal.Commenting.FunctionComment.MissingReturnComment
   * @return \Drupal\Core\Condition\ConditionInterface
   */
  public function &get($instance_id) {
    return 'x';
  }

  /**
   * Whether this breadcrumb builder should be used to build the breadcrumb.
   *
   * @param \Drupal\Core\Routing\RouteMatchInterface $route_match
   *   The current route match.
   * phpcs:ignore Drupal.Commenting.FunctionComment.ParamNameNoMatch
   * @param \Drupal\Core\Cache\CacheableMetadata $cacheable_metadata
   *   The cacheable metadata to add to if your check varies by or depends
   *   on something. Anything you specify here does not have to be repeated in
   *   the build() method as it will be merged in automatically.
   *
   * @return bool
   *   TRUE if this builder should be used or FALSE to let other builders
   *   decide.
   *
   * @todo Uncomment new method parameters before drupal:12.0.0, see
   *   https://www.drupal.org/project/drupal/issues/3459277.
   */
  public function applies(RouteMatchInterface $route_match /* , CacheableMetadata $cacheable_metadata */) {
    return TRUE;
  }

}
