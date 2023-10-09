<?php
/**
 * @file
 * Tests array declarations.
 */

$array = [
  'data' => 'my-data',
  'animal' => 'squirrel',
  'inline' => [],
  'inline3' => ['one', 'two', 'three'],
  'inline_long_ok' => ['one', 'two', 'three', 'four', 'five'],
  'inline_long_nok' => ['one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten', 'eleven', 'twelve'],
  'nested' => [
  ],
  'nested2' => [
    'a'
  ],
  'nested3' => [
    'a' => 'a',
    'b' => [
      'c'
    ]
  ]
];

$array = array(
  'data' => 'my-data',
  'animal' => 'squirrel',
  'inline' => array(),
  'inline3' => array('one', 'two', 'three'),
  'inline_long_ok' => array('one', 'two', 'three', 'four', 'five'),
  'inline_long_nok' => array('one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten', 'eleven', 'twelve'),
  'nested' => array(
  ),
  'nested2' => array(
    'a'
  ),
  'nested3' => array(
    'a' => 'a',
    'b' => array(
      'c'
    ),
  )
);

$derivatives["entity:$entity_type_id"] = array(
  'label' => t('Create @entity_type path alias', array('@entity_type' => $entity_type->getLowercaseLabel())),
  'category' => t('Path'),
  'entity_type_id' => $entity_type_id,
  'context' => array(
    'entity' => ContextDefinition::create("entity:$entity_type_id")
      ->setLabel($entity_type->getLabel())
      ->setRequired(TRUE)
      ->setDescription(t('The @entity_type for which to create a path alias.', array('@entity_type' => $entity_type->getLowercaseLabel()))),
    'alias' => ContextDefinition::create('string')
      ->setLabel(t('Path alias'))
      ->setRequired(TRUE)
      ->setDescription(t("Specify an alternative path by which the content can be accessed. For example, 'about' for an about page. Use a relative path and do not add a trailing slash."))
  ),
  'provides' => array(),
) + $base_plugin_definition;

$derivatives["entity:$entity_type_id"] = [
  'label' => t('Create @entity_type path alias', ['@entity_type' => $entity_type->getLowercaseLabel()]),
  'category' => t('Path'),
  'entity_type_id' => $entity_type_id,
  'context' => [
    'entity' => ContextDefinition::create("entity:$entity_type_id")
      ->setLabel($entity_type->getLabel())
      ->setRequired(TRUE)
      ->setDescription(t('The @entity_type for which to create a path alias.', ['@entity_type' => $entity_type->getLowercaseLabel()])),
    'alias' => ContextDefinition::create('string')
      ->setLabel(t('Path alias'))
      ->setRequired(TRUE)
      ->setDescription(t("Specify an alternative path by which the content can be accessed. For example, 'about' for an about page. Use a relative path and do not add a trailing slash."))
  ],
  'provides' => [],
] + $base_plugin_definition;

$a = array('1', '2', array(
  '3',
    ),
);

$a = ['1', '2', [
  '3',
    ],
];

$a = array(
'x' => 'y',
);

$backend->expects($this->exactly(1))
  ->method('get')
  ->will($this->onConsecutiveCalls(
    (object) array('data' => array('hook' => array(
      'module_handler_test' => array(),
      'module_handler_test_missing' => array(),
    )))
  ));

$backend->expects($this->exactly(1))
  ->method('get')
  ->will($this->onConsecutiveCalls(
    (object) ['data' => ['hook' => [
      'module_handler_test' => [],
      'module_handler_test_missing' => [],
    ]]]
  ));

/**
 * Nested closure in an array should not throw errors.
 */
function mymodule_libraries_info() {
  $libraries['some-library'] = array(
    'xautoload' => function ($adapter) {
      /** @var \Drupal\xautoload\Adapter\LocalDirectoryAdapter $adapter */
      // Scan sites/all/libraries/some-library/composer.json to look for
      // autoload information.
      $adapter->composerJson('composer.json');
    },
  );
  return $libraries;
}

/**
 * Multiline ternary operators should have another level of indentation enforced
 * for their components. Single increase only.
 */

// Indexed.
$array = [
  'root tests',
  ('expect' === 'pass')
    ? 'reason'
    : 'indented as expected',
  ('expect' === 'fail')
  ? 'reason'
  : 'no indentation',
  ('expect' === 'fail')
      ? 'reason'
      : 'too much indentation',
  ('expect' === 'fail')
? 'reason'
: 'negative indentation',
  [
    'multidimensional tests',
    ('expect' === 'pass')
      ? 'reason'
      : 'indented as expected',
    ('expect' === 'fail')
    ? 'reason'
    : 'no indentation',
    ('expect' === 'fail')
        ? 'reason'
        : 'too much indentation',
    ('expect' === 'fail')
  ? 'reason'
  : 'negative indentation',
  ],
];

// Associative.
$array = [
  'root_item_1' => 'root tests',
  'root_item_2' => ('expect' === 'pass')
    ? 'reason'
    : 'indented as expected',
  'root_item_3' => ('expect' === 'fail')
  ? 'reason'
  : 'no indentation',
  'root_item_4' => ('expect' === 'fail')
      ? 'reason'
      : 'too much indentation',
  'root_item_5' => ('expect' === 'fail')
? 'reason'
: 'negative indentation',
  'root_item_6' => [
    'multidimensional_item_1' => ('expect' === 'pass')
      ? 'reason'
      : 'indented as expected',
    'multidimensional_item_2' => ('expect' === 'fail')
    ? 'reason'
    : 'no indentation',
    'multidimensional_item_3' => ('expect' === 'fail')
        ? 'reason'
        : 'too much indentation',
    'multidimensional_item_4' => ('expect' === 'fail')
  ? 'reason'
  : 'negative indentation',
  ],
];
