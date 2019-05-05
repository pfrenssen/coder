<?php

// Test correct indenting of trait conflict resolution.
class MyClass {
  use MyTrait {
     myTraitMethod as renamedTraitMethod;
  }
}

setTimeout(function() {
  $timer = parseInt($target, 10);
  if (timer > 0) {
    $target--;
  }
  else {
    // <dt>.
       $kint->toggleChildren(target . parentNode);
       if ($kint->currentPlus !== -1) {
      $kint->fetchVisiblePluses();
       }
  }
}, 300);

/**
 * Implements hook_field_formatter_view().
 */
function example_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();
  switch ($display['type']) {
    case 'example_schema_format':
            foreach ($items as $delta => $item) {
        $element[$delta]['#markup'] = theme('example_formatter_default', $item);
            }
      break;
  }
  return $element;

}

/**
 * A test function.
 */
function test_form() {
  /*
   * Here is some long form comment which is discouraged but not forbidden,
   * I think? It goes over multiple lines in this pseudo doc comment style.
   * And even more text now.
   */
  $form['submit_connection'] = [
    '#prefix' => "<br style='clear:both'/>",
    '#name' => 'enter_connection_settings',
    '#type' => 'submit',
    '#value' => t('Enter connection settings'),
    '#weight' => 100,
  ];
}
