<?php
/**
 * @file
 * Region template with a CSS id.
 */
?>
<?php if ($content): ?>
  <div id="<?php print $id; ?>" class="<?php print $classes; ?>">
    <?php print $content; ?>
  </div><!-- /.region -->
<?php endif; ?>
