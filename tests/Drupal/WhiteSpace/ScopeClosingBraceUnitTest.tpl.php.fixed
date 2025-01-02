<?php

/**
 * @file
 * Template error test.
 *
 * This sniff should not run on template files, since the closing brace can be
 * used with content on the line before. We just have this test case to
 * demonstrate there are no fatal errors when running the fixer.
 */

?>
<table <?php if ($classes) { print 'class="'. $classes . '" '; } ?><?php print $attributes; ?>>
   <?php if (!empty($title) || !empty($caption)) : ?>
     <caption><?php print $caption . $title; ?></caption>
  <?php endif; ?>
  <?php if (!empty($header)) : ?>
    <thead>
      <tr>
        <?php foreach ($header as $field => $label): ?>
          <th <?php if ($header_classes[$field]) { print 'class="'. $header_classes[$field] . '" '; } ?> scope="col">
            <?php print $label; ?>
          </th>
        <?php endforeach; ?>
      </tr>
    </thead>
  <?php endif; ?>
  <tbody>
    <?php foreach ($rows as $row_count => $row): ?>
      <tr <?php if ($row_classes[$row_count]) { print 'class="' . implode(' ', $row_classes[$row_count]) .'"';  } ?>>
        <?php foreach ($row as $field => $content): ?>
          <td <?php if ($field_classes[$field][$row_count]) { print 'class="'. $field_classes[$field][$row_count] . '" '; } ?><?php print drupal_attributes($field_attributes[$field][$row_count]); ?>>
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
      <tr <?php if ($row_classes[$row_count]) { print 'class="details ' . implode(' ', $row_classes[$row_count]) .'"';  } ?>>
          <td colspan="<?php print count($row); ?>">
            <?php print $descriptions[$row_count]; ?>
          </td>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>
