<?php

// Should use placeholder in t() instead.
$x = t('Test') . ': ' . $variable;

// Concatenating markup is fine.
$x = '<p>' . t('Test') . '</p>';

// Concatenating some symbols is fine.
$x = '(' . t('Test') . ')';
$x = '[' . t('Test') . ']';
$x = '- ' . t('Test') . ' -';
$x = '<' . t('Test') . '>';
$x = '«' . t('Test') . '»';

// Concatenating markup with text in it is not fine.
$x = '<p>' . t('Test') . '</p><p>More text.</p>';

// Concatenating symbols with text in it is not fine.
$x = t('Test') . ' (disabled)';

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
