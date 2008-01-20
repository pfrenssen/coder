<?php
TEST: Control structures

--INPUT--
function menu_tree_page_data($menu_name = 'navigation') {
  // Check whether the current menu has any links set to be expanded.
  if (in_array($menu_name, $expanded)) {
    // Collect all the links set to be expanded, and then add all of
    // their children to the list as well.
    do {
      $result = db_query();
      while ($item = db_fetch_array($result)) {
        $num_rows = TRUE;
      }
      $placeholders = implode(', ', array_fill(0, count($args), '%d'));
    } while ($num_rows);
  }
}

--INPUT--
function format_case() {
  switch ($moo) {
    case 'foo':
      $bar = $baz;
      break;

    case 'fee':
    default:
      $bar = $bay;
      return;
  }
}

--INPUT--
function case_return() {
  switch ($moo) {
    case 'foo':
      if ($bar) {
        return $baz;
      }
      $baz = $bar;
      break;

    case 'fee':
      $bar = $bay;
      return;
  }
}

--INPUT--
function language_url_rewrite(&$path, &$options) {
  switch (variable_get('language_negotiation', LANGUAGE_NEGOTIATION_NONE)) {
    case LANGUAGE_NEGOTIATION_PATH_DEFAULT:
      $default = language_default();
      // Intentionally no break here.
    case LANGUAGE_NEGOTIATION_PATH:
      if (!empty($options['language']->prefix)) {
        $options['prefix'] = $options['language']->prefix .'/';
      }
      break;
  }
}

