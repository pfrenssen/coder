<?php
TEST: Multiline arrays

-- INPUT --
$var = array(
  'install_page' => array(
    'arguments' => array(
      'content' => NULL),
  ),
);

$array = array(
  'foo' => 'bar',
  2 => $foo,
  0x000000 => 'asdf',
  "CRIVENS" => 3,
  $bar => 'asdf',
);

$deep = array(
  'foo' => array(
    'croon' => 'asdf',
    'f' => 'a'
  ),
  'barasdfsadf' => array(
    'asdfasdfasdf' => 'd',
    'fsd' => 23
  )
);

drupal_add_link(array('rel' => 'alternate',
                      'type' => 'application/rss+xml',
                      'title' => $title,
                      'href' => $url));

function node_theme() {
  return array(
    'node' => array(
      'arguments' => array('node' => NULL, 'teaser' => FALSE, 'page' => FALSE),
      'template' => 'node',
    ),
    'node_list' => array(
      'arguments' => array('items' => NULL, 'title' => NULL),
    ),
    'node_search_admin' => array(
      'arguments' => array('form' => NULL),
    ),
    'node_filter_form' => array(
      'arguments' => array('form' => NULL),
      'file' => 'node.admin.inc',
    ),
    'node_filters' => array(
      'arguments' => array('form' => NULL),
      'file' => 'node.admin.inc'
    ),
    'node_admin_nodes' => array(
      'arguments' => array('form' => NULL),
      'file' => 'node.admin.inc',
    ),
    'node_add_list' => array('arguments' => array('content' => NULL),
      'file' => 'node.pages.inc',
    ),
    'node_form' => array(
      'arguments' => array('form' => NULL),
      'file' => 'node.pages.inc',
    ),
    'node_preview' => array('arguments' => array('node' => NULL), 'file' => 'node.pages.inc'),
    'node_log_message' => array(
      'arguments' => array('log' => NULL),
    ),
    'node_submitted' => array(
      'arguments' => array('node' => NULL),
    ),
  );
}

-- EXPECT --
$var = array(
  'install_page' => array(
    'arguments' => array(
      'content' => NULL,
    ),
  ),
);

$array = array(
  'foo' => 'bar',
  2 => $foo,
  0x000000 => 'asdf',
  "CRIVENS" => 3,
  $bar => 'asdf',
);

$deep = array(
  'foo' => array(
    'croon' => 'asdf',
    'f' => 'a',
  ),
  'barasdfsadf' => array(
    'asdfasdfasdf' => 'd',
    'fsd' => 23,
  ),
);

drupal_add_link(array('rel' => 'alternate',
    'type' => 'application/rss+xml',
    'title' => $title,
    'href' => $url,
  ));
function node_theme() {
  return array(
    'node' => array(
      'arguments' => array('node' => NULL, 'teaser' => FALSE, 'page' => FALSE),
      'template' => 'node',
    ),
    'node_list' => array(
      'arguments' => array('items' => NULL, 'title' => NULL),
    ),
    'node_search_admin' => array(
      'arguments' => array('form' => NULL),
    ),
    'node_filter_form' => array(
      'arguments' => array('form' => NULL),
      'file' => 'node.admin.inc',
    ),
    'node_filters' => array(
      'arguments' => array('form' => NULL),
      'file' => 'node.admin.inc',
    ),
    'node_admin_nodes' => array(
      'arguments' => array('form' => NULL),
      'file' => 'node.admin.inc',
    ),
    'node_add_list' => array('arguments' => array('content' => NULL),
      'file' => 'node.pages.inc',
    ),
    'node_form' => array(
      'arguments' => array('form' => NULL),
      'file' => 'node.pages.inc',
    ),
    'node_preview' => array('arguments' => array('node' => NULL), 'file' => 'node.pages.inc'),
    'node_log_message' => array(
      'arguments' => array('log' => NULL),
    ),
    'node_submitted' => array(
      'arguments' => array('node' => NULL),
    ),
  );
}

