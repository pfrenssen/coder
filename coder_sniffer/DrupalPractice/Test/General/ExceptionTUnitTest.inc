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

  public function dolor() {
    throw (new PipelineStepExecutionLogicException('Graph data could not be inserted'))->setUserMessage([
      '#markup' => $this->t('Could not store triples in triple store. Reason: @message', [
        '@message' => t('Failed'),
      ]),
    ]);
  }

}
