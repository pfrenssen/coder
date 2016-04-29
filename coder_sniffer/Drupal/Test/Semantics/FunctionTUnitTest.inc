<?php

// Should use placeholder in t() instead.
$x = t('Test') . ': ' . $variable;

// Concatenating markup is fine.
$x = '<p>' . t('Test') . '</p>';

class Test {

  public function doSomething() {
    return $this->t('This string should avoid backslashes for \'');
  }

  public function doSomethingElse() {
    return new TranslatableMarkup('Do not ' . 'concatenate');
  }

  public function doSomethingMore() {
    return new TranslationWrapper(' Do not start with a space');
  }

}
