TEST: Tags
FULL: 1

--INPUT--
<?php
// $Id$

function ugly_foo() {
  $bar = bar();
  ?>
  <div class="foo">
    <?php print $bar; ?>
  </div>
  <?php
  return $baz;
}

--INPUT--
<?php
// $Id$

?>
<a href="<?php print $link; ?>"><?php print $title; ?></a>

