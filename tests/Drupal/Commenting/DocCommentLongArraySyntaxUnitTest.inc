<?php

/**
 * @file
 * Test long array syntax in doc comments.
 */

/**
 * Long array syntax used.
 * @code
 * $table = array(
 *   '#type' => 'table',
 * );
 * @endcode
 */
function test1() {

}

/**
 * Nested long array syntax used.
 * @code
 * $table = array(
 *   '#attributes' => array(
 *     'id' => 'my-module-table',
 *   ),
 * );
 * @endcode
 */
function test2() {

}

/**
 * Malformed code block that does not end correctly, ignore it.
 *
 * The structure of the array is:
 * @code
 * $workflow_array = [
 *   'entity_state' => [
 *     'user' => [
 *       'transition1',
 *       'transition2',
 *     ],
 *   ],
 * ];
 * @code
 */
function test3() {

}

/**
 * The structure of the array @code ['a' => 'b'] @endcode
 */
function test4() {

}
