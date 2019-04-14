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
@trigger_error('\Drupal\Some\Class\Foo is deprecated in drupal:8.5.0. Use \Drupal\Foo\Bar instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Alternative using __NAMESPACE__.
@trigger_error(__NAMESPACE__ . '\Foo is deprecated in drupal:8.5.12. It will be removed from Drupal 9.0. Use \Drupal\Foo\Bar instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Alternative https url and cater for E_User_Deprecated in non-upper case.
@trigger_error("\Drupal\Some\Class\Foo is deprecated in drupal:8.11.0. Use \Drupal\Foo. See https://www.drupal.org/project/drupal/issues/2908490", E_User_Deprecated);
// Alternative syntax for contrib project and issue url.
@trigger_error('Function foo_bar() is deprecated in my_module:8.x-2.0. Use bar_baz() instead. See https://www.drupal.org/project/my_module/issues/2908490', E_USER_DEPRECATED);

// Fails on general layout.
// Wrong first part - missing word.
@trigger_error('drupal_set_message() is deprecated Drupal:8.5.0. It will be removed from Drupal 9.0.0. Use \Drupal\Core\Messenger\MessengerInterface::addMessage() instead. See https://www.drupal.org/node/2774931', E_USER_DEPRECATED);
// Wrong first part - bad spacing.
@trigger_error('drupal_set_message() is   deprecated in Drupal:8.5.0. It will be removed from Drupal 9.0.0. Use \Drupal\Core\Messenger\MessengerInterface::addMessage() instead. See https://www.drupal.org/node/2774931', E_USER_DEPRECATED);
// Wrong first part - no final dot.
@trigger_error('drupal_set_message() is deprecated in Drupal:8.5.0 Use \Drupal\Core\Messenger\MessengerInterface::addMessage() instead. See https://www.drupal.org/node/2774931', E_USER_DEPRECATED);
// Wrong first part - nothing before 'is deprecated'.
@trigger_error(__NAMESPACE__ . ' is deprecated in Drupal:8.5.0. It will be removed from Drupal 9.0.0. Use \Drupal\Core\Messenger\MessengerInterface::addMessage() instead. See https://www.drupal.org/node/2774931', E_USER_DEPRECATED);
// Missing 'extra info' part.
@trigger_error('drupal_set_message() is deprecated in Drupal:8.5.0. See https://www.drupal.org/node/2774931', E_User_Deprecated);
// String would be OK but is in more than one part.
@trigger_error('drupal_set_message() is deprecated in Drupal:8.5.0. ' . 'It will be removed from Drupal 9.0.0. Use \Drupal\Core\Messenger\MessengerInterface::addMessage() instead. See https://www.drupal.org/node/2774931', E_USER_DEPRECATED);

// Fails on version.
// Version with only one component.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Version with only two components.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8.6. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Version without :
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal 8.6.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Wrong case for Drupal.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in Drupal:8.6.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Non digit in core format.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8.6.x. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// No 'x' in contrib format.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in project:8.6-2.x. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Version more than 2 digits.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in project:182.x-2.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Wrong case for Project.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in Project:8.6-2.x. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490', E_USER_DEPRECATED);

// Fails on 'see' url.
// Incorrect see url - no http.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8.6.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Incorrect see url - extra text before http.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8.6.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See this http://www.drupal.org/node/2908490', E_USER_DEPRECATED);
// Incorrect see url - extra text after link.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8.6.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/2908490 for details', E_USER_DEPRECATED);
// Incorrect see url - no node id.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8.6.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See http://www.drupal.org/node/', E_USER_DEPRECATED);
// Incorrect see url - non-numeric node id.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8.6.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See https://www.drupal.org/node/123a', E_USER_DEPRECATED);
// Do not want period after url.
@trigger_error(__NAMESPACE__ . '\CommentTestBase is deprecated in drupal:8.6.0. Use \Drupal\Tests\comment\Functional\Views\CommentTestBase instead. See https://www.drupal.org/node/123.', E_USER_DEPRECATED);
