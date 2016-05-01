<?php

class TestForm extends FormBase {

  public function buildForm($form, $form_state) {
    $form['something'] = \Drupal::service('foobar')->result();
    // Accessing the Drupal version is fine and should not trigger a warning.
    $version = \Drupal::VERSION;
  }

}
