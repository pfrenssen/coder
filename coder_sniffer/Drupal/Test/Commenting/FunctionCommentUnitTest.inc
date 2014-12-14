<?php

/**
 * Test.
 *
 * @param string $x
 *   Comment does not end with full stop
 * @param string $y
 *   comment does not start with a capital letter.
 */
function foo($x, $y) {

}

/**
 * Missing exception description in the throws tag.
 *
 * @throws \Drupal\locale\StringStorageException
 */
function test1() {

}

/**
 * Wrong indentation of description for the throws tag.
 *
 * @throws \Drupal\locale\StringStorageException
 *  Description here.
 */
function test2() {

}

/**
 * Description for the throws tag does not start with capital letter.
 *
 * @throws \Drupal\locale\StringStorageException
 *   description here.
 */
function test3() {

}

/**
 * Description for the throws tag does not end with a dot.
 *
 * @throws \Drupal\locale\StringStorageException
 *   Description here
 */
function test4() {

}

/**
 * Description for the throws tag must be on the next line.
 *
 * @throws \Drupal\locale\StringStorageException Description here.
 */
function test5() {

}
