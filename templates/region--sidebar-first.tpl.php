<?php
/**
 * @file
 * Region template for sidebar containing menus and optionally a search box.
 */
?>
<?php if ($content): ?>
<!-- START SIDEBAR -->
<!-- noindex -->
<?php if ($responsive_menu): ?>
  <?php print render($responsive_menu); ?>
<?php endif; ?>
<div id="menu" class="<?php print $classes; ?>">
  <div class="narrow">
    <?php if ($search_box):
      print render($search_box);
    endif; ?>
    <?php print $content; ?>
  </div>
</div>
<!-- endnoindex -->
<!-- END SIDEBAR -->
<?php endif; ?>
