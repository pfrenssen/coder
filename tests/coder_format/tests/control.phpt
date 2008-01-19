<?php
TEST: Control structures

--INPUT--
// No change:
function menu_tree_page_data($menu_name = 'navigation') {
  // Check whether the current menu has any links set to be expanded.
  if (in_array($menu_name, $expanded)) {
    // Collect all the links set to be expanded, and then add all of
    // their children to the list as well.
    do {
      $result = db_query("SELECT mlid FROM {menu_links} WHERE menu_name = '%s' AND expanded = 1 AND has_children = 1 AND plid IN (". $placeholders .') AND mlid NOT IN ('. $placeholders .')', array_merge(array($menu_name), $args, $args));
      $num_rows = FALSE;
      while ($item = db_fetch_array($result)) {
        $args[] = $item['mlid'];
        $num_rows = TRUE;
      }
      $placeholders = implode(', ', array_fill(0, count($args), '%d'));
    } while ($num_rows);
  }
}


--EXPECT--
// No change:
function menu_tree_page_data($menu_name = 'navigation') {
  // Check whether the current menu has any links set to be expanded.
  if (in_array($menu_name, $expanded)) {
    // Collect all the links set to be expanded, and then add all of
    // their children to the list as well.
    do {
      $result   = db_query("SELECT mlid FROM {menu_links} WHERE menu_name = '%s' AND expanded = 1 AND has_children = 1 AND plid IN (". $placeholders .') AND mlid NOT IN ('. $placeholders .')', array_merge(array($menu_name), $args, $args));
      $num_rows = FALSE;
      while ($item = db_fetch_array($result)) {
        $args[]   = $item['mlid'];
        $num_rows = TRUE;
      }
      $placeholders = implode(', ', array_fill(0, count($args), '%d'));
    } while ($num_rows);
  }
}

