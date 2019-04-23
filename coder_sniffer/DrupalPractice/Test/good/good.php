<?php

/**
 * @file
 * This file should throw no warnings/errors.
 */

$form['vocab_fieldset']['entity_view_modes'] = array(
  '#type'     => 'select',
  '#prefix'   => '<div id="entity_view_modes_div">',
  '#suffix'   => '</div>',
  '#title'    => t('View Mode'),
  '#description' => '<p>' . t('test description') . '</p>',
  '#required' => 1,
  '#options'  => array(),
  '#ajax' => array(
    'callback' => 'custom_listing_pages_entity_vocabulary_listing',
    'wrapper'  => 'vocab_fieldset_div',
    'method'   => 'replace',
    'effect'   => 'fade',
  ),
);
print_r($form);

/**
 * Ignoring the array value in a foreach loop is OK.
 */
function test1() {
  $array = array(1, 2);
  foreach ($array as $key => $value) {
    print $key;
  }

  try {
    print 'foo';
  }
  catch (Exception $e) {
    // $e is unused here, which is fine.
    print 'error';
  }

  // Initializing an array on the fly is allowed.
  $items['foo'] = 'bar';
  return $items;
}

/**
 * Variables that are used by reference are allowed to not be read.
 */
function test2() {
  $list = &some_other_function();
  $list = array();
}

/**
 * Variables that are used by reference are allowed to not be read.
 */
function test3(&$variables, $hook) {
  foreach ($variables['items'] as &$item) {
    $item['image'] = 'foo';
  }
}

/**
 * Global variables that are used in ifs should not be flagged as unused.
 */
function test4() {
  global $user;
  $x = 5;
  if ($x == 5) {
    $user = 123;
  }
}

/**
 * A trait containing helper methods for language handling.
 */
trait LangcodeTrait {

  /**
   * Name of language.
   *
   * @var string
   */
  protected $langcode;

  /**
   * A fully-populated language object.
   *
   * @var \Drupal\core\Language\LanguageInterface|null
   */
  protected $lang;

  /**
   * Select language.
   *
   * @param string $langcode
   *   Language code.
   *
   * @return $this
   *   Current instance.
   */
  public function setLangcode($langcode) {
    $this->langcode = $langcode;
    $this->lang = \Drupal::languageManager()->getLanguage($this->langcode);
    return $this;
  }

  /**
   * Get code of currently active language.
   *
   * @return string
   *   Language code.
   */
  public function getLangcode() {
    if (!isset($this->langcode)) {
      $lang = \Drupal::languageManager()->getCurrentLanguage();
      $this->setLangcode($lang->getId());
    }

    return $this->langcode;
  }

}

/**
 * Testing closures.
 */
class ClosureTest extends TestCase {

  /**
   * Use $this in a closure, which should be a defined variable.
   */
  public function getEntities($limit) {
    // Create an array of dummy entities.
    $entities = array_map(function () {
      return $this->prophesize(EntityInterface::class)->reveal();
    }, range(1, $limit));
    return $entities;
  }

}

// Unused variable test.
$clicks = array();
$places = array();

$debug = '';
enumerate_menu('primary', function ($item) use (&$places, &$debug, &$clicks) {
  $pos = array_pop($places);
  $n_clicks = 34 - $pos;
  generateClicks($item['node'], $n_clicks);
  $clicks[$item['mlid']] = $n_clicks;
  $debug .= "\nLink {$item['node']->title} got $n_clicks clicks and should end on $pos place.";
});

class PrivateMethodTest {

  /**
   * This private method is used as array filter callback, so not unused.
   */
  private function usedAsCallback() {
    return FALSE;
  }

  public function test() {
    return array_filter(array(), [$this, 'usedAsCallback']);
  }
}

class StrictSchemaEnabled {

  /**
   * {@inheritdoc}
   */
  protected $strictConfigSchema = TRUE;

}
