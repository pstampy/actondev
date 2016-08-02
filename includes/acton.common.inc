<?php
/**
 * @file
 * Theme utility functions file.
 */

/**
 * Determines whether a block is a system or core menu.
 */
function acton_block_is_core_menu($block) {
  $system_menus = menu_list_system_menus();
  $menus = menu_get_menus(FALSE);
  return $block->module == 'system' && isset($system_menus[$block->delta]) ||
    $block->module == 'menu' && isset($menus[$block->delta]);
}

/**
 * Builds a list of grid class options for a given max grid width.
 */
function acton_get_grid_layout_options($region_width = 'doublewide') {
  $grids = array_fill_keys(array(
    'narrow',
    'doublenarrow',
    'wide',
    'doublewide',
    'full',
    'one-third',
    'two-third',
  ), FALSE);

  // Detect grid layouts to include.
  switch ($region_width) {
    case 'full':
      $grids['full'] = TRUE;
    case 'two-third':
      $grids['two-third'] = TRUE;
    case 'one-third':
      $grids['one-third'] = TRUE;
      // Stop if only thirds are supported.
      if ($region_width == 'one-third' || $region_width == 'two-third') {
        break;
      }
    case 'doublewide':
      $grids['doublewide'] = TRUE;
    case 'doublenarrow':
      $grids['doublenarrow'] = TRUE;
    case 'wide':
      $grids['wide'] = TRUE;
    case 'narrow':
      $grids['narrow'] = TRUE;
  }

  // Allow modules and subthemes a chance to modify options.
  $options = drupal_map_assoc(array_keys(array_filter($grids)));
  drupal_alter('acton_grid_layout_options', $options, $region_width);

  return $options;
}
