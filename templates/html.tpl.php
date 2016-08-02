<?php
/**
 * @file
 * Implementation to display a full HTML page.
 *
 * Available variables, in addition to default variables for this template:
 * - $doctype: Document type declaration.
 * - $html_attributes: String of basic attributes for the <html> tag.
 * - $html_full_attributes: String of attributes for the augmented <html> tag.
 * - $acton_head: Renderable array for page metadata tags in header.
 * - $anu_head: Renderable array for remotely provided style resources.
 */
?>
<?php print $doctype; ?>

<!--[if lt IE 7 ]><html class="ie6"<?php print $html_attributes; ?>><![endif]-->
<!--[if IE 7 ]><html class="ie7"<?php print $html_attributes; ?>><![endif]-->
<!--[if IE 8 ]><html class="ie8"<?php print $html_attributes; ?>><![endif]-->
<!--[if IE 9 ]><html class="ie9"<?php print $html_attributes; ?>><![endif]-->
<!--[if (gt IE 9)|!(IE)]><!--><html<?php print $html_full_attributes; ?>><!--<![endif]-->
<head<?php if ($grddl_profile): ?> profile="<?php print $grddl_profile; ?>"<?php endif; ?>>
  <title><?php print $head_title; ?></title>
  <?php print render($acton_head); ?>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print render($anu_head); ?>
  <?php print $scripts; ?>
</head>
<body<?php print $attributes; ?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
