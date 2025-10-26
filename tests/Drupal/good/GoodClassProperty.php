<?php

/**
 * Underscores are allowed in properties of config entity classes.
 */
class GoodClassProperty extends ConfigEntityBundleBase {

  /**
   * Default value of the 'Create new revision' checkbox of this node type.
   *
   * @var bool
   */
  protected $new_revision = TRUE;

}
