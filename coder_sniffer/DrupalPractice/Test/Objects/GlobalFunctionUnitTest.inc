<?php

class TestForm extends FormBase {

  public function buildForm($form, $form_state) {
    $form['something'] = t('Example text');
  }

}
