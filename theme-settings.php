<?php
/**
 * @file
 * Theme settings form.
 */

// Load settings file.
include_once 'includes/acton.settings.inc';

/**
 * Implements hook_form_system_theme_settings_alter() function.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function acton_form_system_theme_settings_alter(&$form, &$form_state) {
  form_load_include($form_state, 'inc', 'block', 'block.admin');
  $form_state['build_info']['files']['acton_theme_settings'] = drupal_get_path('theme', 'acton') . '/theme-settings.php';
  require_once drupal_get_path('theme', 'acton') . '/includes/acton.common.inc';

  $form['acton'] = array(
    '#type' => 'fieldset',
    '#title' => t('Acton theme settings'),
  );

  $form['acton']['acton_path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path-related settings'),
    '#collapsible' => TRUE,
  );
  $form['acton']['acton_path']['acton_hide_title_path'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages to hide title'),
    '#default_value' => !is_null($setting = theme_get_setting('acton_hide_title_path')) ? $setting : '<front>',
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );
  $form['acton']['acton_path']['acton_hide_breadcrumb_path'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages to hide breadcrumb'),
    '#default_value' => !is_null($setting = theme_get_setting('acton_hide_breadcrumb_path')) ? $setting : '<front>',
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  $form['acton']['grid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grid layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if (!module_exists('block')) {
    $form['acton']['grid']['block_info'] = array(
      '#markup' => '<p>' . t('The Block module must be enabled to configure layouts.') . '</p>',
    );
  }
  else {
    $form['acton']['grid']['info'] = array(
      '#markup' => '<p>' . t('Select the default grid layout for blocks in each region.') . '</p>',
    );
    $form['acton']['grid']['acton_grid_default'] = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array('class' => array('acton-region-default-layout-settings')),
      '#attached' => array(
        'css' => array(
          array(
            'data' => '
              .acton-region-default-layout-settings .acton-region-layout{clear:both;}
              .acton-region-default-layout-settings .acton-region-layout .form-item{float:left; min-width:12em; margin-right:2em;}
              .acton-region-default-layout-settings .acton-region-layout .form-type-checkbox{margin-top:2em;}
            ',
            'type' => 'inline',
          ),
        ),
      ),
    );
    $layout_settings = theme_get_setting('acton_grid_default_layout');
    $layout_settings = is_array($layout_settings) ? $layout_settings : array();
    $trim_settings = theme_get_setting('acton_grid_default_trim');
    $trim_settings = is_array($trim_settings) ? $trim_settings : array();
    $info = system_get_info('theme', $GLOBALS['theme_key']) + array('regions' => array());
    $skipped_regions = array('help', 'sidebar_first', 'page_top', 'page_bottom');
    $doublewide_regions = array('content_top', 'content', 'content_bottom');
    foreach ($info['regions'] as $region_name => $region_title) {
      // Skip menu sidebar.
      if (in_array($region_name, $skipped_regions)) {
        continue;
      }

      // Add options for regions.
      $region_width = in_array($region_name, $doublewide_regions) ? 'doublewide' : 'full';
      $form['acton']['grid']['acton_grid_default'][$region_name] = array(
        '#theme_wrappers' => array('container'),
        '#attributes' => array('class' => array('acton-region-layout')),
      );
      $layout_settings += array($region_name => '');
      $form['acton']['grid']['acton_grid_default'][$region_name]['layout'] = array(
        '#type' => 'select',
        '#title' => check_plain($region_title),
        '#default_value' => $layout_settings[$region_name],
        '#options' => array('' => t('- None -')) + acton_get_grid_layout_options($region_width),
        '#parents' => array('acton_grid_default_layout', $region_name),
      );
      $trim_settings += array($region_name => array());
      $trim_settings[$region_name] += array('top' => FALSE, 'bottom' => FALSE);
      $form['acton']['grid']['acton_grid_default'][$region_name]['trim_top'] = array(
        '#type' => 'checkbox',
        '#title' => t('Remove top margin'),
        '#default_value' => $trim_settings[$region_name]['top'],
        '#parents' => array('acton_grid_default_trim', $region_name, 'top'),
      );
      $form['acton']['grid']['acton_grid_default'][$region_name]['trim_bottom'] = array(
        '#type' => 'checkbox',
        '#title' => t('Remove bottom margin'),
        '#default_value' => $trim_settings[$region_name]['bottom'],
        '#parents' => array('acton_grid_default_trim', $region_name, 'bottom'),
      );
    }
  }

  $form['acton']['acton_menu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu & breadcrumb settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['acton']['acton_menu']['styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu blocks'),
  );
  if (!module_exists('block')) {
    $form['acton']['acton_menu']['styles']['block_info'] = array(
      '#markup' => '<p>' . t('The Block module must be enabled to configure menu blocks.') . '</p>',
    );
  }
  else {
    $form['acton']['acton_menu']['styles']['#description'] = t('Select blocks to show as menu and configure menu display style.');

    // List sidebar blocks.
    $form['acton']['acton_menu']['styles']['acton_menu_blocks'] = array(
      '#type' => 'vertical_tabs',
    );
    $form['acton']['acton_menu']['styles']['acton_menu_block_styles'] = array(
      '#parents' => array('acton_menu_block_styles'),
      '#tree' => TRUE,
      '#element_validate' => array('acton_menu_clean_block_settings'),
    );
    $styles = theme_get_setting('acton_menu_block_styles');
    $menu_style_options = array(
      'menu-main' => t('College-specific (menu-main)'),
      'menu-grey' => t('Grey (menu-grey)'),
      'menu-uni' => t('Platinum (menu-uni)'),
    );
    foreach (block_admin_display_prepare_blocks($GLOBALS['theme_key']) as $block_data) {
      if ($block_data['region'] == 'sidebar_first') {
        $block = block_load($block_data['module'], $block_data['delta']);
        $block_id = $block_data['module'] . '_' . $block_data['delta'];
        $is_core_menu = acton_block_is_core_menu($block);
        $form['acton']['acton_menu']['styles']['acton_menu_block_styles'][$block_id] = array(
          '#type' => 'fieldset',
          '#title' => $block_data['info'],
          '#group' => 'acton_menu_blocks',
        );
        $form['acton']['acton_menu']['styles']['acton_menu_block_styles'][$block_id]['enabled'] = array(
          '#type' => 'checkbox',
          '#title' => t('Display as menu'),
          '#default_value' => $is_core_menu ? TRUE : isset($styles[$block_id]),
          '#disabled' => $is_core_menu,
        );
        $form['acton']['acton_menu']['styles']['acton_menu_block_styles'][$block_id]['style'] = array(
          '#type' => 'select',
          '#title' => t('Menu style'),
          '#options' => array('default' => t('Default')) + $menu_style_options,
          '#default_value' => isset($styles[$block_id]) ? $styles[$block_id] : 'default',
          '#states' => array(
            'visible' => array(
              ':checkbox[name="acton_menu_block_styles[' . $block_id . '][enabled]"]' => array('checked' => TRUE),
            ),
          ),
        );
      }
    }
    $default_menu_style = theme_get_setting('acton_menu_style_default');
    $form['acton']['acton_menu']['styles']['acton_menu_style_default'] = array(
      '#type' => 'select',
      '#title' => t('Default menu style'),
      '#options' => $menu_style_options,
      '#default_value' => $default_menu_style,
    );
  }
  $form['acton']['acton_menu']['acton_menu_highlight_frontpage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Highlight "Home" if no active link'),
    '#default_value' => !is_null($setting = theme_get_setting('acton_menu_highlight_frontpage')) ? $setting : 0,
    '#description' => t('Select to highlight the "Home" menu link, if it exists, where a page has no active menu item.'),
  );
  $form['acton']['acton_menu']['acton_breadcrumb_append_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append page title to breadcrumb'),
    '#default_value' => !is_null($setting = theme_get_setting('acton_breadcrumb_append_title')) ? $setting : 0,
    '#description' => t('Select to append the title of the current page to the breadcrumb, if the breadcrumb is not empty.'),
  );
  $form['acton']['acton_menu']['acton_breadcrumb_append_menu_active_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use menu link for page title'),
    '#default_value' => theme_get_setting('acton_breadcrumb_append_menu_active_title'),
    '#description' => t('Select to append the menu link title to the breadcrumb instead of the default page title, whenever an active menu link is available.'),
    '#states' => array(
      'visible' => array(
        ':checkbox[name="acton_breadcrumb_append_title"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['acton']['acton_tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dynamic tabs settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['acton']['acton_tabs']['acton_tabs_prefix'] = array(
    '#type' => 'textarea',
    '#title' => t('Tab hash prefixes'),
    '#default_value' => implode("\n", !is_null($setting = theme_get_setting('acton_tabs_prefix')) ? $setting : array('tabs-')),
    '#description' => '<p>' . t("Specify prefixes for tab links. Acton will scan all links on a page and look for link paths with a fragment (i.e. URL segment starting with '#') to process them. These processed links, upon landing on the target page, will prevent the browser from scrolling to the tab pane.") . '</p>' .
      '<p>' . t('Alternatively, the links may specify the class !link_class to force processing the link path (with a fragment).', array('!link_class' => '<code>' . ACTON_TABS_LINK_CLASS . '</code>')) . '</p>',
    '#element_validate' => array('acton_form_element_split_lines'),
  );

  $form['acton']['acton_reset_settings'] = array(
    '#type' => 'submit',
    '#theme_key' => $form_state['build_info']['args'][0],
    '#value' => t('Reset Acton theme settings'),
    '#submit' => array('acton_form_system_theme_settings_reset'),
    '#limit_validation_errors' => array(),
  );
}

/**
 * Cleans menu block style settings.
 */
function acton_menu_clean_block_settings($element, &$form_state) {
  $value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  $styles = array();
  foreach ($value as $block_id => $data) {
    if (!empty($data['enabled'])) {
      $styles[$block_id] = $data['style'];
    }
  }
  drupal_array_set_nested_value($form_state['values'], $element['#parents'], $styles);
}

/**
 * Transforms text area submission into array.
 */
function acton_form_element_split_lines($element, &$form_state) {
  $value = $element['#value'];
  $array_value = array_filter(preg_split('/(\r|\n)+/', $value));
  drupal_array_set_nested_value($form_state['values'], $element['#parents'], $array_value);
}

/**
 * Resets Acton-specific settings.
 */
function acton_form_system_theme_settings_reset($form, $form_state) {
  if (isset($form_state['triggering_element']['#theme_key'])) {
    acton_reset_default_settings($form_state['triggering_element']['#theme_key']);
    drupal_set_message(t('Acton settings have been reset.'));
  }
}
