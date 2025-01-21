<?php

/**
 * @file
 * Contains Test.
 */

/**
 * Implements hook_node_view().
 *
 * Single quotes.
 */
#[Hook('hook_node_view')]
function module_node_view() {

}

/**
 * Implements hook_node_load().
 *
 * Double quotes.
 */
#[Hook("hook_node_load")]
function module_node_load() {

}


/**
 * Implements hook_node_delete().
 */
#[Hook(hook: 'hook_node_delete')]
function module_node_delete() {

}

/**
 * Implements hook_node_alter().
 */
#[Hook(hook: 'hook_node_alter', module: 'custom_module')]
function module_node_alter() {

}

/**
 * Implements hook_piratehook_view().
 *
 * "hook" is a part of hook name. No warning.
 */
#[Hook('piratehook_view')]
function module_piratehook_view() {

}

/**
 * Implements hook_hookpirate_view().
 *
 * "hook" is a part of hook name. No warning.
 */
#[Hook('hookpirate_view')]
function module_hookpirate_view() {

}

/**
 * Implements hook_hook_info().
 *
 * "hook_info" is exception. No warning.
 */
#[Hook('hook_info')]
function module_hook_info() {

}

/**
 * Implements hook_hook_info().
 *
 * "hook_info" is exception. No warning.
 */
#[Hook(hook: 'hook_info')]
function module_custom_hook_info() {

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
 *
 */
#[Hook(hook: "hook_user_login", method: "userLogin", module: "views")]
class MyHooks {

  /**
   *
   */
  public function userLogin() {

  }

}
