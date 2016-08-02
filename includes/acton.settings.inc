<?php
/**
 * @file
 * Settings-related functions.
 */

/**
 * CSS class for tab links.
 */
define('ACTON_TABS_LINK_CLASS', 'acton-tabs-link');

/**
 * Initializes theme settings.
 */
function acton_initialize_settings() {
  // Save default settings.
  $defaults = acton_default_settings();
  $var = 'theme_' . $GLOBALS['theme_key'] . '_settings';
  $settings = (array) variable_get($var, array());

  // Only detect settings if version is not set or less than default.
  if (!isset($settings['acton_settings_version']) || $settings['acton_settings_version'] < $defaults['acton_settings_version']) {
    $version_old = isset($settings['acton_settings_version']) ? $settings['acton_settings_version'] : NULL;
    $settings['acton_settings_version'] = $defaults['acton_settings_version'];

    foreach ($defaults as $setting_key => $setting_default) {
      // Set default value where missing or different type.
      if (!array_key_exists($setting_key, $settings) || gettype($settings[$setting_key]) != gettype($setting_default)) {
        $settings[$setting_key] = $setting_default;
      }
    }

    if (isset($version_old)) {
      $settings = acton_settings_update($version_old, $settings);
    }
    variable_set($var, $settings);
  }
}

/**
 * Gets default settings.
 */
function acton_default_settings() {
  return array(
    'acton_grid_default_layout' => array(
      'content_top' => 'doublewide',
      'content' => 'doublewide',
      'content_bottom' => 'doublewide',
    ),
    'acton_grid_default_trim' => array(
      'content_top' => array('top' => TRUE),
      'content' => array('top' => TRUE),
      'content_bottom' => array('top' => TRUE),
    ),
    'acton_hide_breadcrumb_path' => '<front>',
    'acton_hide_title_path' => '<front>',
    'acton_menu_style_default' => 'menu-main',
    'acton_menu_block_styles' => array(),
    'acton_menu_highlight_frontpage' => 0,
    'acton_breadcrumb_append_title' => 1,
    'acton_breadcrumb_append_menu_active_title' => 0,
    'acton_tabs_prefix' => array('tabs-'),
    // Mark configuration version.
    'acton_settings_version' => 2.007,
  );
}

/**
 * Resets Acton-specific settings.
 */
function acton_reset_default_settings($theme_key) {
  $var = 'theme_' . $theme_key . '_settings';
  $settings = (array) variable_get($var, array());
  foreach (array_keys(acton_default_settings()) as $key) {
    if (array_key_exists($key, $settings)) {
      unset($settings[$key]);
    }
  }
  variable_set($var, $settings);
}

/**
 * Updates theme settings.
 */
function acton_settings_update($base_version, array $settings) {
  $info = acton_settings_update_info();
  ksort($info);
  foreach ($info as $version => $callback) {
    if ($version > $base_version && function_exists($callback)) {
      $settings = $callback($settings);
    }
  }
  return $settings;
}

/**
 * Defines settings updater callbacks.
 */
function acton_settings_update_info() {
  return array(
    2 => 'acton_settings_update_2',
  );
}

/**
 * Updates menu style settings.
 */
function acton_settings_update_2(array $settings) {
  require_once drupal_get_path('theme', 'acton') . '/includes/acton.common.inc';

  if (array_key_exists('acton_menu_style', $settings)) {
    // Compile menu style settings by block.
    $acton_menu_style = $settings['acton_menu_style'];
    if (!empty($acton_menu_style) && is_array($acton_menu_style)) {
      $block_styles = array();
      $default_style = 'menu-main';
      foreach (block_list('sidebar_first') as $block_id => $block) {
        if (acton_block_is_core_menu($block)) {
          $block_style = array_shift($acton_menu_style);
          // Copy old style class.
          if (!empty($block_style) && in_array($block_style, array('menu-main', 'menu-grey', 'menu-uni'))) {
            $block_styles[$block_id] = $default_style = $block_style;
          }
          // Use default.
          else {
            $block_styles[$block_id] = 'default';
          }
        }
      }
      $settings['acton_menu_block_styles'] = $block_styles;
      $settings['acton_menu_style_default'] = $default_style;
    }

    // Remove legacy setting.
    unset($settings['acton_menu_style']);
  }

  return $settings;
}
