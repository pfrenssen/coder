<?php

use \Drupal\node\Entity\NodeType;

class TestForm extends FormBase {

  public function buildForm($form, $form_state) {
    $form['something'] = NodeType::load('article');
  }

  public static function something() {
    // Global classes are allowed in static methods.
    return NodeType::load('article');
  }

}
