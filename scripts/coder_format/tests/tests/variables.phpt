<?php
TEST: Variables

--INPUT--
// No change:
function node_view($node, $teaser = FALSE, $page = FALSE, $links = TRUE) {
  $node = (object)$node;

  $node = node_build_content($node, $teaser, $page);

  if ($links) {
    $node->links = module_invoke_all('link', 'node', $node, $teaser);
    drupal_alter('link', $node->links, $node);
  }
}

function node_content_form($node, $form_state) {
  $type = node_get_types('type', $node);
  $form = array();

  if ($type->has_title) {
    $form['title'] = array();
  }
}


--EXPECT--
// No change:
function node_view($node, $teaser = FALSE, $page = FALSE, $links = TRUE) {
  $node = (object)$node;

  $node = node_build_content($node, $teaser, $page);

  if ($links) {
    $node->links = module_invoke_all('link', 'node', $node, $teaser);
    drupal_alter('link', $node->links, $node);
  }
}

function node_content_form($node, $form_state) {
  $type = node_get_types('type', $node);
  $form = array();

  if ($type->has_title) {
    $form['title'] = array();
  }
}

