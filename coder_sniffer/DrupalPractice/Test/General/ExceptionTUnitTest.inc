<?php

class ExceptionExample {

  public function foo() {
    throw new Exception(t('Error'));
  }

  public function bar() {
    throw new Exception($this->t('Error'));
  }

  public function lorem() {
    return t('Error');
  }

  public function ipsum() {
    return $this->t('Error');
  }

}
