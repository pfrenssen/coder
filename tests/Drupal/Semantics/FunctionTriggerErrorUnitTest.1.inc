<?php

/**
 * @file
 * Example code lines for FunctionTriggerError sniff test.
 */

// Passes.
// No second parameter, so cannot fail it.
@trigger_error('CommentTestBase is deprecated in drupal 8.4.0');
// Not E_USER_DEPRECATED, so cannot fail it.
@trigger_error('CommentTestBase is deprecated in drupal 8.4.0', E_USER_SOMETHING_ELSE);
// All correct.
@trigger_error('\Drupal\Some\Class\Foo is deprecated in drupal:8.5.0 and wont work in in drupal:9.0.0. Use \Drupal\Foo\Bar instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Alternative using __NAMESPACE__.
@trigger_error(__NAMESPACE__ . '\Foo is deprecated in drupal:8.5.12 and wont work in drupal:9.0.0. Use \Drupal\Foo\Bar instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Alternative https url and cater for E_User_Deprecated in non-upper case.
@trigger_error("\Drupal\Some\Class\Foo is deprecated in drupal:8.11.0 and wont work in drupal:9.0.0. Use \Drupal\Foo. See https://www.drupal.org/project/drupal/issues/2908490", E_User_Deprecated);
// Alternative syntax for contrib project and issue url.
@trigger_error('Function foo_bar() is deprecated in my_module:8.x-2.0 and wont work in my_module:9.x-1.0. Use bar_baz() instead. See https://www.drupal.org/project/my_module/issues/2908490', E_USER_DEPRECATED);
// Cater for sprintf inside the message.
@trigger_error(sprintf('Function %s is deprecated in drupal:8.5.0 and wont work in drupal:9.0.0. Use foo() instead. See http://www.drupal.org/project/my_module/issues/123', $settings), E_USER_DEPRECATED);
// Cater for concatenations in the message.
@trigger_error(__METHOD__ . ' method is deprecated in drupal:8.4.0 and wont work in drupal:9.0.0. Use ' . __CLASS__ . '::bundle() instead. See https://www.drupal.org/project/drupal/issues/12345', E_USER_DEPRECATED);
// Cater for concatenation with nothing else before 'is deprecated.
@trigger_error(__NAMESPACE__ . ' is deprecated in drupal:8.5.0 and wont work in drupal:9.0.0. Use \Drupal\Core\Messenger\MessengerInterface::addMessage() instead. See https://www.drupal.org/node/2774931', E_USER_DEPRECATED);

// ERRORS on general layout.
// Wrong first part - missing word.
@trigger_error('foo() is deprecated drupal:8.5.0 and will not work in drupal:9.0.0. Use bar() instead. See https://www.drupal.org/node/123', E_USER_DEPRECATED);
// Wrong first part - bad spacing.
@trigger_error('foo() is   deprecated in drupal:8.5.0 and will not work in drupal:9.0.0. Use bar() instead. See https://www.drupal.org/node/123', E_USER_DEPRECATED);
// No removal version.
@trigger_error('foo() is deprecated in drupal:8.5.0. See https://www.drupal.org/node/123', E_User_Deprecated);
// Incorrect see url - extra text before http.
@trigger_error('foo() is deprecated in drupal:8.6.0 and will not work in drupal:9.0.0. Use bar() instead. See this http://www.drupal.org/node/123', E_USER_DEPRECATED);
// Incorrect see url - extra text after link.
@trigger_error('foo() is deprecated in drupal:8.6.0 and will not work in drupal:9.0.0. Use bar() instead. See http://www.drupal.org/node/123 for details', E_USER_DEPRECATED);
// Ensure that concatenation at end of string is checked.
@trigger_error('foo() is deprecated in drupal:8.6.0 and will not work in drupal:9.0.0. Use bar() instead. See https://www.drupal.org/node/123' . __FUNCTION__ , E_USER_DEPRECATED);
// WARNINGS for version. Version with only one component.
@trigger_error('foo() is deprecated in drupal:8 and will not work in drupal:9.0.0. Use \bar\baz instead. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
// Wrong first part - no final dot so next .
@trigger_error('foo() is deprecated in drupal:8.5.0 Use bar() instead. See https://www.drupal.org/node/123', E_USER_DEPRECATED);
// Version with only two components.
@trigger_error('foo() is deprecated in drupal:8.6 and will not work in drupal:9.0.0. Use \bar\baz instead. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
// Version without :
@trigger_error('foo() is deprecated in drupal 8.6.0 and will not work in drupal:9.0.0. Use \bar\baz instead. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
// Wrong case for Drupal.
@trigger_error('foo() is deprecated in Drupal:8.6.0 and will not work in drupal:9.0.0. Use \bar\baz instead. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
// Non digit in core format.
@trigger_error('foo() is deprecated in drupal:8.6.x and will not work in drupal:9.0.0. Use \bar\baz instead. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
// No 'x' in contrib format.
@trigger_error('foo() is deprecated in my_project:8.6-2.x and will not work in my_project:9.x-1.0. Use bar() instead. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
// Version more than 2 digits.
@trigger_error('foo() is deprecated in project:19.x-2.0 and will not work in project:182.x-2.0. Use bar() instead. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
// Wrong case for Project.
@trigger_error('foo() is deprecated in project:8.x-2.0 and will not work in Project:9.x-1.0. Use bar() instead. See http://www.drupal.org/node/123', E_USER_DEPRECATED);
// WARNINGS for 'see' url. Incorrect see url - no http.
@trigger_error('foo() is deprecated in drupal:8.6.0 and will not work in drupal:9.0.0. Use bar() instead. See www.drupal.org/node/123', E_USER_DEPRECATED);
// Incorrect see url - no node id.
@trigger_error('foo() is deprecated in drupal:8.6.0 and will not work in drupal:9.0.0. Use bar() instead. See http://www.drupal.org/node/', E_USER_DEPRECATED);
// Incorrect see url - non-numeric node id.
@trigger_error('foo() is deprecated in drupal:8.6.0 and will not work in drupal:9.0.0. Use bar() instead. See https://www.drupal.org/node/123a', E_USER_DEPRECATED);
// Do not want period after url.
@trigger_error('foo() is deprecated in drupal:8.6.0 and will not work in drupal:9.0.0. Use bar() instead. See https://www.drupal.org/node/123.', E_USER_DEPRECATED);
