<?php

/**
 * @file
 * Test module.
 */

/**
 * @param \stdClass $arg
 *   Something here.
 * @param object $blarg
 *   Another thing.
 */
function module_handler_test_all2_hook(object $arg, object $blarg) {
  return $arg;
}
