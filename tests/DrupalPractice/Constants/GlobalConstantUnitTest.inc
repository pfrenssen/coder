<?php

const FOO_BAR = 0;

/**
 * Some comment.
 */
const FOO_BAR2 = 1;

/**
 * This should not throw a warning because the constant is deprecated.
 *
 * @deprecated in Drupal 8.1.x.
 */
const FOO_BAR3 = 2;

/**
 * Provides an interface defining a taxonomy vocabulary entity.
 */
interface VocabularyInterface extends ConfigEntityInterface {

  /**
   * Denotes that no term in the vocabulary has a parent.
   */
  const HIERARCHY_DISABLED = 0;

}
