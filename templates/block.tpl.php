<?php
/**
 * @file
 * Acton implementation to display block.
 */
?>
<div<?php print $attributes; ?>>
  <?php if ($show_box_header): ?>
     <div class="box-header">
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <div class="box-title"><?php print $title; ?></div>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
    </div>
  <?php endif; ?>

  <div class="<?php print $box_classes; ?>">
    <?php if (!$show_box_header): ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <?php if (in_array('menu-flat', $classes_array)): ?>
          <p<?php print $title_attributes; ?>><?php print $title; ?></p>
        <?php else: ?>
          <h2<?php print $title_attributes; ?>><?php print $title; ?></h2>
        <?php endif; ?>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
    <?php endif; ?>

    <div<?php print $content_attributes; ?>>
      <?php print $content; ?>
    </div>
  </div>
</div><!-- /.block -->
