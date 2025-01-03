<?php

namespace Drupal\action_link_test_plugins\Plugin\StateAction;

use Drupal\action_link\Entity\ActionLinkInterface;
use Drupal\action_link\Plugin\StateAction\StateActionBase;
use Drupal\Core\Access\AccessResult;
use Drupal\Core\Session\AccountInterface;

/**
 * Test action which is always usable.
 */
#[\Drupal\action_link\Attribute\StateAction(
  id: 'test_always',
  label: new \Drupal\Core\StringTranslation\TranslatableMarkup('Test Always'),
  description: new \Drupal\Core\StringTranslation\TranslatableMarkup('Test Always'),
  directions: [
    'change' => 'change',
  ]
)]
class TestAlways extends StateActionBase {
}
