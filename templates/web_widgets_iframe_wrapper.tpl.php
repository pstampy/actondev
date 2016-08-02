<?php
/**
 * @file
 * HTML wrapper code for the iframe'ed code at the external site.
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>    
<?php print $head ?>
    <?php print drupal_get_js(); ?>
    <?php print drupal_get_css(); ?>
    <?php !empty($styles) ? print $styles : '' ?>
    <?php !empty($scripts) ? print $scripts : '' ?>
<link href="https://style.anu.edu.au/_anu/4/images/logos/anu.ico" rel="shortcut icon" type="image/x-icon"/>
<link href="https://style.anu.edu.au/_anu/images/icons/web/anu-app-57.png" rel="apple-touch-icon" sizes="57x57"/>
<link href="https://style.anu.edu.au/_anu/images/icons/web/anu-app-76.png" rel="apple-touch-icon" sizes="76x76"/>
<link href="https://style.anu.edu.au/_anu/images/icons/web/anu-app-120.png" rel="apple-touch-icon" sizes="120x120"/>
<link href="https://style.anu.edu.au/_anu/images/icons/web/anu-app-152.png" rel="apple-touch-icon" sizes="152x152"/>
<link href="https://style.anu.edu.au/_anu/images/icons/web/anu-app-180.png" rel="apple-touch-icon" sizes="180x180"/>
<link href="https://style.anu.edu.au/_anu/4/style/anu-common.css" rel="stylesheet" type="text/css" media="screen"/>
<link href="https://style.anu.edu.au/_anu/4/style/anu-college.css" rel="stylesheet" type="text/css" media="screen"/>
<link href="https://style.anu.edu.au/_anu/4/style/anu-features.css" rel="stylesheet" type="text/css" media="screen"/>
<link href="https://style.anu.edu.au/_anu/4/style/anu-cecs.css" rel="stylesheet" type="text/css" media="screen"/>
<!--[if !IE]>--><link href="https://style.anu.edu.au/_anu/4/style/anu-fluid.css" rel="stylesheet" type="text/css" media="screen"/><!--<![endif]-->
<link href="https://style.anu.edu.au/_anu/4/style/anu-print.css" rel="stylesheet" type="text/css" media="print"/>
<style>
body {
 background-color: #FFFFFF !important;
}
</style>
<title><?php print $title ?></title>
  </head>
  <body id="widget">
    <?php if ($title): ?><h1 class="page-title"><?php print $title ?></h1><?php endif; ?>
    <?php print $content ?>
  </body>
</html>
