<?php

/**
 * @file
 * Test assignment alignment.
 */

$x  = 5;

$y  = 3;
$zz  = 4;

$a  = 1;
$b  = 2;

$c   = 1;
$dd   = 2;

$e = 1;
$f = 2;
$ggg = 3;
$h   = 4;

/**
 * Example function.
 */
function example_form_submit_redirect_handler($form, &$form_state) {
  $selected_content = $form_state['values']['field_content_type'][LANGUAGE_NONE][0]['value'];
  $_SESSION['file_upload_session'] = TRUE;
  $form_state['redirect']  = 'example/' . $selected_content . '/' . $form_state['nid'] . '/mapping';
}
