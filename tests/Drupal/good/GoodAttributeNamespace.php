<?php

/**
 * Fully qualified class name is allowed in PHP attributes for now.
 */
#[\Drupal\action_link\Attribute\StateAction(
  id: 'test_always',
  label: new \Drupal\Core\StringTranslation\TranslatableMarkup('Test Always'),
  description: new \Drupal\Core\StringTranslation\TranslatableMarkup('Test Always'),
  directions: [
    'change' => 'change',
  ]
)]
class GoodAttributeNamespace extends StateActionBase {

  /**
   * Partial names are ok in attributes for now.
   */
  #[Assert\NotBlank]
  private bool $bar;

  /**
   * Partially qualified names are ok in attributes for now.
   */
  #[CLI\Command(
    name: 'example',
    aliases: ['example-foo']
  )]
  #[CLI\Option(name: 'pretty_format', description: 'Display the count in pretty format.')]
  public function test(array $options = ['pretty-format' => TRUE]): void {
  }

}
