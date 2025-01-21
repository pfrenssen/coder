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
#[Hook('hook_node_view')]
function module_node_view() {

}

/**
 * Double quotes.
 */
#[Hook("hook_node_load")]
function module_node_load() {

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
 * "hook" is a part of hook name. No warning.
 */
#[Hook('hook_hookpirate_view')]
function module_hookpirate_view() {

}

/**
 * "hook_info" is exception. No warning.
 */
#[Hook('hook_info')]
function module_hook_info() {

}

/**
 * Named argument. "hook_info" is exception. No warning.
 */
#[Hook(hook: 'hook_info')]
function mymodule_hook_info() {

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
