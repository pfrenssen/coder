<?php

class TestForm extends FormBase {

  public function buildForm($form, $form_state) {
    $form['something'] = t('Example text');
    $form['another'] = $this->t('test');
  }

  public static function example() {
    // t() calls are allowed in static methods.
    return t('Example text');
  }

}
