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
  const FOO = 'bar';

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

// https://www.drupal.org/i/3562081
// Nowdoc should always be literals.
$x = t(
    <<<'TEXT'
    Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer
    placerat id justo nec porttitor. Maecenas at auctor nisi, vitae tempor
    nulla. Donec nisl est, hendrerit ac odio id, maximus bibendum sem. In
    sollicitudin, lorem vitae eleifend congue, nulla enim semper ante, in
    vulputate libero orci in ligula. Ut facilisis quam augue, sed rhoncus
    nisl lobortis a. Curabitur tempor lectus ut suscipit venenatis. Donec
    semper neque eu orci blandit convallis. Morbi arcu turpis, condimentum
    vitae sollicitudin dapibus, commodo et nisl. Nam sed consectetur enim.
    Duis a tellus nisi. Fusce egestas varius convallis.
    TEXT
);
$x = t(
    <<<'TEXT'
    $not_variable
    TEXT
);
$x = t(
    <<<TEXT
    $variable
    TEXT
);
$x = t(
    <<<'TEXT'
    TEXT
);
