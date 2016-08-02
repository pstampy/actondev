<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the Acton theme.
 */

/**
 * Theme indicator. This constant is used to activate Acton Profile.
 */
define('ACTON', 'acton');

// Initialize.
acton_initialize();

/**
 * Initializes the theme.
 */
function acton_initialize() {
  $path = drupal_get_path('theme', 'acton');
  require_once $path . '/includes/acton.common.inc';
  require_once $path . '/includes/acton.settings.inc';
  require_once $path . '/includes/acton.theme.inc';
  require_once $path . '/includes/acton.preprocess.inc';

  // Initialize settings.
  acton_initialize_settings();
}

/**
 * Implements hook_page_alter().
 */
function acton_page_alter(&$page) {
  $current_path = current_path();
  if (module_exists('block') && $current_path == 'admin/structure/block/demo/' . $GLOBALS['theme_key']) {
    // Toggle menu sidebar.
    if (!empty($_GET['hide_sidebar'])) {
      $page['sidebar_first'] = array();
    }

    // Build toggle link.
    $parameters = drupal_get_query_parameters();
    $parameters['hide_sidebar'] = empty($_GET['hide_sidebar']) ? 1 : 0;
    $page['page_top']['toggle_sidebar'] = $page['page_top']['backlink'];
    $page['page_top']['toggle_sidebar']['#title'] = t('Toggle menu sidebar');
    $page['page_top']['toggle_sidebar']['#href'] = $current_path;
    $page['page_top']['toggle_sidebar']['#options']['attributes']['class'][] = 'toggle-sidebar-link';
    $page['page_top']['toggle_sidebar']['#options']['query'] = $parameters;
    drupal_add_css('
      .acton a.block-demo-backlink.toggle-sidebar-link {
        left: auto;
        right: 20px;
      }
    ', 'inline');

    // Layout region demo blocks.
    $page['highlighted']['block_description']['#prefix'] = '<div class="full nopadtop nopadbottom">';
    $page['highlighted']['block_description']['#suffix'] = '</div>';
    $page['header']['block_description']['#prefix'] = '<div class="full nopadbottom">';
    $page['header']['block_description']['#suffix'] = '</div>';
    foreach (array('content_top', 'content', 'content_bottom') as $region) {
      $page[$region]['block_description']['#prefix'] = '<div class="doublewide nopadtop">';
      $page[$region]['block_description']['#suffix'] = '</div>';
    }
    foreach (array('band_first', 'band_second', 'footer') as $region) {
      $page[$region]['block_description']['#prefix'] = '<div class="full nopadtop">';
      $page[$region]['block_description']['#suffix'] = '</div>';
    }
  }
}
