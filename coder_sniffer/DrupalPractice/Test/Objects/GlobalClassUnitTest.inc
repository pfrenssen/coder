<?php

use \Drupal\node\Entity\NodeType;

class TestForm extends FormBase {

  public function buildForm($form, $form_state) {
    $form['something'] = NodeType::load('article');
  }

  public static function something() {
    return NodeType::load('article');
  }

}
