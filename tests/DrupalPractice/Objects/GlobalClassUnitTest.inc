<?php

use \Drupal\node\Entity\NodeType;
use Drupal\node\NotEntity\User;

class TestForm extends FormBase {

  public function buildForm($form, $form_state) {
    $form['something'] = NodeType::load('article');
  }

  public static function something() {
    // Global classes are allowed in static methods.
    return NodeType::load('article');
  }

  public function notentity() {
    // This is not a user entity.
    return User::load(1);
  }

}
