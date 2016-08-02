<?php
/**
 * @file
 */
?>

<?php print render($alert_bar); ?>
<?php print render($anu_body_header); ?>

<?php if ($page['highlighted']): ?>
  <div id="feat-wrap">
    <div id="feat">
      <?php print render($page['highlighted']); ?>
    </div>
  </div><!-- /#feat-wrap -->
<?php endif; ?>

<?php print render($page['header']); ?>

<?php if ($sidebar_exists = !empty($page['sidebar_first']) || !empty($page['sidebar_content'])): ?>
  <?php print render($page['sidebar_first']); ?>
  <?php print render($page['sidebar_content']); ?>
  <div id="content" class="clearfix">
<?php endif; ?>

<div id="content-main" class="margintop clearfix">

<?php if ($messages || $breadcrumb || $title_prefix || $title || $title_suffix || $tabs || $action_links): ?>
  <div id="content-header" class="<?php print($sidebar_exists ? 'doublewide' : 'full'); ?> nomargintop nomarginbottom">
    <?php if ($messages): ?>
      <div class="marginbottom">
        <?php print $messages; ?>
      </div>
    <?php endif; ?>
    <?php if ($breadcrumb): ?>
      <div id="breadcrumb">
        <?php print render($breadcrumb); ?>
      </div>
    <?php endif; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h1 class="title" id="page-title"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if ($tabs): ?>
      <div class="tabs">
        <?php print render($tabs); ?>
      </div>
    <?php endif; ?>
    <?php if ($action_links): ?>
      <ul class="action-links">
        <?php print render($action_links); ?>
      </ul>
    <?php endif; ?>
  </div><!-- /#content-header -->
<?php endif; ?>

<?php if ($page['help']): ?>
  <div class="<?php print($sidebar_exists ? 'doublewide' : 'full'); ?>  nomargintop nomarginbottom">
    <?php print render($page['help']); ?>
  </div>
<?php endif; ?>

<?php print render($page['content_top']); ?>
<?php print render($page['content']); ?>
<?php if ($feed_icons): ?>
  <div class="<?php print($sidebar_exists ? 'doublewide' : 'full'); ?> nomargintop">
    <?php print $feed_icons; ?>
  </div>
<?php endif; ?>
<?php print render($page['content_bottom']); ?>

</div><!-- /#content-main -->

<?php if ($sidebar_exists): ?>
  </div><!-- /#content -->
<?php endif; ?>

<?php print render($page['band_first']); ?>
<?php print render($page['band_second']); ?>
<?php print render($page['footer']); ?>

<?php print render($site_footer); ?>

<?php print render($anu_body_footer); ?>
