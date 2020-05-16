<?php

class TestForm extends FormBase {

  public function buildForm($form, $form_state) {
    $form['something'] = \Drupal::service('foobar')->result();
    // Accessing the Drupal version is fine and should not trigger a warning.
    $version = \Drupal::VERSION;
  }

  public static function example() {
    // \Drupal calls are allowed in static methods.
    return \Drupal::service('foobar')->result();
  }

}
