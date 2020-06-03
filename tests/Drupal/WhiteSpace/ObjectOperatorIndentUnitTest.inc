<?php

/**
 * @file
 * Tests coding standards for chained method calls.
 */

use Drupal\Core\Entity\EntityTypeManagerInterface;

/**
 * Function containing test code for chained method calls.
 */
function chained_method_calls(EntityTypeManagerInterface $entityTypeManager) {
  // Some standards compliant chained method calls.
  $result = $entityTypeManager->getStorage($this->entityTypeId)->loadMultiple($this->ids);
  $result = $entityTypeManager->getStorage($this->entityTypeId)
    ->loadMultiple($this->ids);
  $result = $entityTypeManager
    ->getStorage($this->entityTypeId)
    ->loadMultiple($this->ids);

  // Some non-compliant calls.
  $result = $entityTypeManager
  ->getStorage($this->entityTypeId)->loadMultiple($this->ids);
  $result = $entityTypeManager->getStorage($this->entityTypeId)
      ->loadMultiple($this->ids);
  $result = $entityTypeManager->getStorage($this->entityTypeId)
                              ->loadMultiple($this->ids);
  $result = $entityTypeManager
  ->getStorage($this->entityTypeId)
  ->loadMultiple($this->ids);
}
