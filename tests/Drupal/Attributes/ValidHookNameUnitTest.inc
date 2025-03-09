<?php

/**
 * @file
 * Contains Test.
 */

/**
 * Valid hook.
 */
#[Hook('valid')]
function module_valid() {

}

/**
 * Single quotes.
 */
#[Hook('hook_info')]
function module_info() {

}

/**
 * Double quotes.
 */
#[Hook("hook_node_load")]
function module_node_load() {

}

/**
 * Not finished hook name. No warning
 */
#[Hook('hook_')]
function module_system() {

}

/**
 * Attribute named argument.
 */
#[Hook(hook: 'hook_node_delete')]
function module_node_delete() {

}

/**
 * Attribute named arguments.
 */
#[Hook(hook: 'hook_node_alter', module: 'custom_module')]
function module_node_alter() {

}

/**
 * "hook" is a part of hook name.
 */
#[Hook('hook_piratehook_view')]
function module_piratehook_view() {

}

/**
 * Implements hook_hookpirate_view().
 *
 * "hook" is a part of hook name.
 */
#[Hook('hook_hookpirate_view')]
function module_hookpirate_view() {

}

/**
 * Valid hook.
 */
#[Hook('valid', 'validMethod', 'module')]
class ValidHooks {

  /**
   *
   */
  public function validMethod() {

  }

}

/**
 *
 */
#[Hook('hook_user_cancel', 'userCancel', 'custom')]
class Hooks {

  /**
   *
   */
  public function userCancel() {

  }

}

/**
 * Named arguments, double quotes.
 */
#[Hook(hook: "hook_user_login", method: "userLogin", module: "views")]
class MyHooks {

  /**
   *
   */
  public function userLogin() {

  }

}
