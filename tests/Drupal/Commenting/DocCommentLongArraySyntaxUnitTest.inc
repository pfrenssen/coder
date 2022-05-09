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
