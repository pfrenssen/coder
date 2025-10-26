<?php

/**
 * Testing nested array definitions going over 80 characters.
 */
class GoodNestedArray {

  /**
   * Test method.
   */
  public function foo() {
    foreach ($x as $y) {
      foreach ($a as $b) {
        $form[$policyTypeKey]['directives'][$directiveName]['options']['flags_wrapper']['flags'] = [
          '#type' => 'checkboxes',
          '#parents' => [$policyTypeKey, 'directives', $directiveName, 'flags', 'humans'],
          '#options' => [
            'unsafe-inline' => "'unsafe-inline'",
            'unsafe-eval' => "'unsafe-eval'",
            'unsafe-hashes' => "'unsafe-hashes'",
            'unsafe-allow-redirects' => "'unsafe-allow-redirects'",
            'strict-dynamic' => "'strict-dynamic'",
            'report-sample' => "'report-sample'",
          ],
          '#default_value' => $config->get($policyTypeKey . '.directives.' . $directiveName . '.flags') ?: [],
        ];
      }
    }
  }

}
