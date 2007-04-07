<?php
// $Id$

function this_is_not_a_menu_hook() {
  $items = array();
  $items[] = array(); // this should not generate an error
  return $items;
}
