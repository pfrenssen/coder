<?php

/**
 * @file
 * Example inline comments.
 */

//Inline comments need to be prefixed by a space after the //.
// But there should be only one space.
//  Not more.
// Now follows a list:
// - item 1
//    wrong indentation level here.
// - now follows a sub-list:
//    - should be only 2 additional spaces.
//   - this one is correct.

test();

// Something missing full stop
// @todo Comment after it.
some_function();

// should start with a capital letter.
test();

// A longer comment that spans over multiple lines but correctly ends with a
// period is obviously allowed.
test();

// Comments ending with a URL are not required to end with a period, see
// https://www.drupal.org/coding-standards
test();

// Comments ending with a function reference don't need to end with a period
// some_function_call($some_variable)
test();

// Comments ending with a function reference don't need to end with a period
// a_function_call_without_arguments()
test();

// Comments should end in a period, even if the last word contains any symbols
// that are non-alphanumerical
test();

//  2 spaces instead of one.
test();

/**
 * Test function.
 */
function example_custom_block_view($delta = '') {
  // This example is adapted from node.module.
  $block = array();

  switch ($delta) {
    case 'my_block':
      //    $block['subject'] = t('custom twitter feed');
      $block['content'] = test();;
      break;
  }
  return $block;
}

/**
 * Test class with properties.
 */
class Test {

  /**
   * Wrong style with var keyword, but the doc block is fine.
   */
  var $varStyle;

}

// People want to pretty-format their numbered lists, do not throw an error for
// this:
// 1. A link has the current path in its 'data-drupal-link-system-path'
//   attribute.
// 2. We are on the front page and a link has the special '<front>' value in
//     its 'data-drupal-link-system-path' attribute.
// 3. This one is super correct, I tell you. What more can I write so that the
//    line gets longer?
// 4. No indentation on the next line is also fine as I'm going to demonstrate
// here.
test();
