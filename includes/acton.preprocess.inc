<?php
/**
 * @file
 * Core preprocess functions.
 */

/**
 * Consolidates element attributes.
 */
function acton_consolidate_attributes(&$variables) {
  if (isset($variables['classes_array'])) {
    // Consolidate attributes by moving classes into attributes_array.
    if (!empty($variables['attributes_array']['class'])) {
      $variables['classes_array'] = array_unique(array_merge($variables['attributes_array']['class'], $variables['classes_array']));
    }
    $variables['classes'] = implode(' ', $variables['classes_array']);
    $variables['attributes_array']['class'] = $variables['classes_array'];
  }
  if (isset($variables['attributes_array'])) {
    $variables['attributes'] = drupal_attributes($variables['attributes_array']);
  }
}

/**
 * Implements hook_preprocess_html().
 */
function acton_preprocess_html(&$variables) {
  _acton_add_extra_stylesheets();

  // Add 'acton' class for ease of customization.
  $variables['classes_array'][] = 'acton';

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  // Add unique class for each page.
  $path = drupal_get_path_alias($_GET['q']);
  // Add unique class for each website section.
  list($section, ) = explode('/', $path, 2);
  if (arg(0) == 'node') {
    if (arg(1) == 'add') {
      $section = 'node-add';
    }
    elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
      $section = 'node-' . arg(2);
    }
  }
  $variables['classes_array'][] = drupal_html_class('section-' . $section);

  // Further consolidate attributes.
  acton_consolidate_attributes($variables);

  // Fill in default values.
  $variables += array(
    'doctype' => '<!DOCTYPE html>',
    'encoding' => array(),
    'html_attributes_array' => array(),
    'html_extra_attributes_array' => array(),
    'grddl_profile' => '',
    'acton_head' => array(),
    'rdfa_prefix' => '',
  );

  // Add custom HTML encoding.
  if ($variables['encoding']) {
    drupal_add_html_head($variables['encoding'], 'system_meta_content_type');
  }
}

/**
 * Implements hook_process_html().
 */
function acton_process_html(&$variables) {
  // Prepare attributes.
  $variables['html_attributes'] = drupal_attributes($variables['html_attributes_array']);
  $variables['html_full_attributes'] = drupal_attributes(array_merge($variables['html_attributes_array'], $variables['html_extra_attributes_array']));

  // Add default variables.
  if (!isset($variables['anu_head'])) {
    $variables['anu_head'] = array(
      '#theme' => 'acton_page_requisite_default',
      '#part' => 'meta',
    );
  }

  // Force latest supported IE mode.
  drupal_add_http_header('X-UA-Compatible', 'IE=Edge,chrome=1');
}

/**
 * Implements hook_preprocess_page().
 */
function acton_preprocess_page(&$variables) {
  // Apply path settings.
  acton_apply_path_settings();

  // Prepare breadcrumb.
  $variables['breadcrumb'] = array();
  if ($breadcrumb = drupal_get_breadcrumb()) {
    $breadcrumb = acton_prepare_breadcrumb($breadcrumb);
    $variables['breadcrumb'] = array(
      '#theme' => 'breadcrumb',
      '#breadcrumb' => $breadcrumb,
    );
  }

  // Scrub tabs if there are no primary or secondary links.
  if (empty($variables['tabs']['#primary']) && empty($variables['tabs']['#secondary'])) {
    $variables['tabs'] = array();
  }

  // Send tabs prefixes setting to js behaviors.
  $prefix = theme_get_setting('acton_tabs_prefix');
  drupal_add_js(array('acton_tabs_prefix' => $prefix), 'setting');
  drupal_add_js(array('acton_tabs_link_class' => ACTON_TABS_LINK_CLASS), 'setting');

  // Prepare main page content without Block module.
  if (!module_exists('block') && !empty($variables['page']['content']['system_main'])) {
    $variables['page']['content']['system_main']['#prefix'] = '<div class="full nomargintop">';
    $variables['page']['content']['system_main']['#suffix'] = '</div>';
  }

  // Prepare blocks in content regions without sidebar.
  if (!$variables['page']['sidebar_first']) {
    foreach (array('content_top', 'content', 'content_bottom') as $region) {
      if ($variables['page'][$region]) {
        foreach ($variables['page'][$region] as $block_key => $block) {
          if ($block_key[0] != '#') {
            $variables['page'][$region][$block_key]['#acton_grid_extend_full'] = TRUE;
          }
        }
      }
    }
  }

  // Add default page parts.
  if (empty($variables['anu_body_header'])) {
    $variables['anu_body_header'] = array(
      '#theme' => 'acton_page_requisite_default',
      '#part' => 'banner',
    );
  }
  if (empty($variables['anu_body_footer'])) {
    $variables['anu_body_footer'] = array(
      '#theme' => 'acton_page_requisite_default',
      '#part' => 'footer',
    );
  }

  // Render error alert message to advise user to install acton_profile.
  if (!module_exists('acton_profile')) {
    $variables['alert_bar'] = array(
      '#theme' => 'acton_alert_bar_default',
    );
  }

  // Further consolidate attributes.
  acton_consolidate_attributes($variables);

  // Fill in default values.
  $variables += array(
    'alert_bar' => array(),
    'site_footer' => array(),
  );
}

/**
 * Applies path-related theme settings.
 */
function acton_apply_path_settings() {
  $current_path = current_path();

  // Match paths.
  foreach (array('acton_hide_title_path', 'acton_hide_breadcrumb_path') as $var) {
    $paths = theme_get_setting($var);
    // Check if current path fails to match.
    if (!drupal_match_path($current_path, $paths)) {
      // Lazy lookup aliased path.
      if (!isset($aliased_path)) {
        if ($current_path === $aliased_path = drupal_get_path_alias($current_path)) {
          // Flag normal path as not available.
          $aliased_path = FALSE;
        }
      }
      // Check if aliased path fails to match, where exists.
      if ($aliased_path === FALSE || !drupal_match_path($aliased_path, $paths)) {
        // Lazy lookup normal path.
        if (!isset($normal_path)) {
          if ($current_path === $normal_path = drupal_get_normal_path($current_path)) {
            // Flag normal path as not available.
            $normal_path = FALSE;
          }
        }
        // Check if normal path fails to match, where exists.
        if ($normal_path === FALSE || !drupal_match_path($normal_path, $paths)) {
          // Fail path matching.
          continue;
        }
      }
    }
    // Perform action.
    switch ($var) {
      // Hide page title.
      case 'acton_hide_title_path':
        drupal_set_title('');
        break;
      // Hide breadcrumb.
      case 'acton_hide_breadcrumb_path':
        drupal_set_breadcrumb(array());
        break;
    }
  }
}

/**
 * Prepares breadcrumb as used in the theme.
 */
function acton_prepare_breadcrumb(array $breadcrumb) {
  $current_path = current_path();

  if (!empty($breadcrumb) && theme_get_setting('acton_breadcrumb_append_title')) {
    // Use page title to append to breadcrumb.
    $title = drupal_get_title();
    // React on current menu item visible in breadcrumb.
    $item = menu_get_item();
    $trail = menu_get_active_trail();
    $trail_end = array_pop($trail);
    if ($item['href'] == $current_path && $item['type'] & MENU_VISIBLE_IN_BREADCRUMB) {
      if ($item['type'] & MENU_IS_LOCAL_TASK) {
        // Use non-default tab title.
        if (!($item['type'] & MENU_LINKS_TO_PARENT)) {
          $title = $item['title'];
        }
      }
      // If configured, use menu link title as default.
      elseif ($trail_end['href'] == $current_path && theme_get_setting('acton_breadcrumb_append_menu_active_title')) {
        $title = $trail_end['title'];
      }
    }
    // Append title to breadcrumb.
    $breadcrumb[] = $title;
  }

  return $breadcrumb;
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function acton_preprocess_maintenance_page(&$variables) {
  _acton_add_extra_stylesheets();

  // Set default title.
  if (empty($variables['title'])) {
    $variables['title'] = t('Maintenance');
  }
  // Set default maintenance message.
  if (empty($variables['content'])) {
    $variables['content'] = t('This site is currently under maintenance and will be back shortly. We apologise for any inconvenience.');
  }
}

/**
 * Processes extra style sheets.
 */
function _acton_add_extra_stylesheets() {
  global $theme_info;
  if (!empty($theme_info->info['framework stylesheets'])) {
    $theme_path = drupal_get_path('theme', $theme_info->name);
    foreach ($theme_info->info['framework stylesheets'] as $media => $stylesheets) {
      foreach ($stylesheets as $path) {
        drupal_add_css($theme_path . '/' . $path, array(
          'group' => CSS_SYSTEM,
          'media' => $media,
          'weight' => -1000,
          'every_page' => TRUE,
        ));
      }
    }
  }
}

/**
 * Implements hook_preprocess_region().
 */
function acton_preprocess_region(&$variables) {
  // Add CSS id for region.
  $region_id = $variables['region'];
  if ($region_id == 'content') {
    // Substitute 'content' for 'content_area'.
    $region_id = 'content_area';
  }
  $variables['id'] = drupal_html_id($region_id);

  // Clear content region.
  if (in_array($variables['region'], array('content_top', 'content', 'content_bottom'))) {
    $variables['classes_array'][] = 'clear';
  }

  // Fill in default values.
  if ($region_id == 'sidebar_first') {
    $variables += array(
      'responsive_menu' => array(),
      'search_box' => array(),
    );
  }

  // Further consolidate attributes.
  acton_consolidate_attributes($variables);
}

/**
 * Implements hook_preprocess_node().
 */
function acton_preprocess_node(&$variables) {
  $node = $variables['node'];

  // Make the title into a link and keep it as a renderable array.
  if (!empty($variables['title'])) {
    $variables['title_link'] = array(
      '#theme' => 'link',
      '#path' => 'node/' . $node->nid,
      '#text' => $variables['title'],
      '#options' => array(
        'html' => FALSE,
        'attributes' => array(),
      ),
    );
  }

  // Add some additional node classes.
  $variables['classes_array'][] = 'node-type-' . $node->type;
  if ($variables['page']) {
    $variables['classes_array'][] = 'node-page' . $node->type;
  }
  elseif ($variables['teaser']) {
    $variables['classes_array'][] = 'node-teaser-' . $node->type;
  }
  if ($variables['sticky']) {
    $variables['classes_array'][] = 'sticky';
  }
  if (!$variables['status']) {
    $variables['classes_array'][] = 'node-unpublished';
  }

  // Add classes for the content container.
  $variables['content_attributes_array']['class'][] = 'node-content';
  $variables['content_attributes_array']['class'][] = 'clearfix';

  $variables['attributes_array']['id'] = 'node-' . $node->nid;

  // Further consolidate attributes.
  acton_consolidate_attributes($variables);
}

/**
 * Implements hook_preprocess_comment().
 */
function acton_preprocess_comment(&$variables) {
  $comment = $variables['comment'];

  $variables['attributes_array']['id'] = 'comment-' . $comment->cid;
  $variables['attributes_array']['class'][] = $variables['status'];

  if ($comment->new) {
    $variables['attributes_array']['class'][] = 'comment-new';
  }

  if ($variables['zebra']) {
    $variables['attributes_array']['class'][] = 'comment-' . $variables['zebra'];
  }

  // Further consolidate attributes.
  acton_consolidate_attributes($variables);
}

/**
 * Implements hook_preprocess_block().
 */
function acton_preprocess_block(&$variables) {
  $block = $variables['block'];

  $variables['attributes_array']['id'] = $variables['block_html_id'];
  $variables['title_attributes_array']['class'][] = 'block-title';
  $variables['content_attributes_array']['class'][] = 'content';
  $variables['content_attributes_array']['class'][] = 'block-content';
  $variables['content_attributes_array']['class'][] = 'clearfix';

  $variables['title'] = $block->subject;

  // Apply layout to content blocks.
  $layout_settings = theme_get_setting('acton_grid_default_layout');
  if (empty($block->disable_acton_grid) && !empty($layout_settings[$block->region])) {
    $grid_class = $layout_settings[$block->region];
    if (!empty($variables['elements']['#acton_grid_extend_full']) && $grid_class == 'doublewide') {
      $grid_class = 'full';
    }
    $variables['classes_array'][] = $grid_class;
    // Add configured trim.
    $trim_settings = theme_get_setting('acton_grid_default_trim');
    if (!empty($trim_settings[$block->region]['top'])) {
      $variables['classes_array'][] = 'nomargintop';
    }
    if (!empty($trim_settings[$block->region]['bottom'])) {
      $variables['classes_array'][] = 'nomarginbottom';
    }
  }

  // Process menus.
  acton_menu_preprocess_block($variables);

  // Process ANU Content Styles.
  $anu_content_styles_defaults = array(
    'show_box_header' => FALSE,
    'header_style' => '',
    'box_classes_array' => array(),
  );
  $variables += $anu_content_styles_defaults + array(
    'box_classes' => '',
  );

  // Build box styles.
  if (!empty($variables['anu_content_styles'])) {
    $variables['anu_content_styles'] += $anu_content_styles_defaults;
    $variables['show_box_header'] = $variables['anu_content_styles']['show_box_header'];
    $variables['header_style'] = $variables['anu_content_styles']['header_style'];
    $variables['box_classes_array'] = array_merge($variables['anu_content_styles']['styles'], $variables['box_classes_array']);
    $variables['box_classes'] = implode(' ', $variables['box_classes_array']);
  }

  // Further consolidate attributes.
  acton_consolidate_attributes($variables);

  // Fill in default values.
  $variables += array(
    'show_box_header' => FALSE,
    'box_classes' => '',
  );
}

/**
 * Preprocesses menu blocks.
 */
function acton_menu_preprocess_block(&$variables) {
  $block = $variables['block'];
  if ($block->region == 'sidebar_first') {
    $default_menu_style = theme_get_setting('acton_menu_style_default');
    $block_styles = theme_get_setting('acton_menu_block_styles');
    if (!is_array($block_styles)) {
      $block_styles = array();
    }

    // Match core or system menu.
    if (acton_block_is_core_menu($block)) {
      $menu_block_id = $block->module . '_' . $block->delta;
    }
    // Match custom menu block.
    elseif (isset($block_styles[$block->module . '_' . $block->delta])) {
      $menu_block_id = $block->module . '_' . $block->delta;
    }

    // Apply menu block style.
    if (isset($menu_block_id)) {
      // Determine configured style.
      $menu_style = !empty($block_styles[$menu_block_id]) ? $block_styles[$menu_block_id] : $default_menu_style;
      $menu_style = ($menu_style == 'default') ? $default_menu_style : $menu_style;
      $variables['classes_array'][] = 'menu-flat';
      $variables['classes_array'][] = $menu_style;
      // Mark linked block title for styling overrides.
      if (isset($block->subject_array['#type']) && $block->subject_array['#type'] == 'link') {
        $variables['title_attributes_array']['class'][] = 'linked';
      }
    }
  }
}

/**
 * Implements hook_process_block().
 */
function acton_process_block(&$variables) {
  acton_consolidate_attributes($variables);
}

/**
 * Implements hook_process_region().
 */
function acton_process_region(&$variables) {
  acton_consolidate_attributes($variables);
}

/**
 * Implements hook_preprocess_menu_local_tasks().
 */
function acton_preprocess_menu_local_tasks(&$variables) {
  foreach (array('primary', 'secondary') as $type) {
    // Skip non-array variables.
    if (empty($variables[$type]) || !is_array($variables[$type])) {
      continue;
    }

    // Add appropriate tabs classes.
    $variables[$type]['#attributes']['class'][] = drupal_html_class('tabs-wrapper-' . $type);
    $variables[$type]['#attributes']['class'][] = 'pagetabs-nav';

    // Wrap tabs.
    $variables[$type]['#post_render'][] = 'acton_menu_local_tasks_wrap';

    // Highlight active tabs with the correct class.
    foreach (element_children($variables[$type]) as $i) {
      $link_element = &$variables[$type][$i];
      $link = &$link_element['#link'];
      if (!empty($link_element['#active'])) {
        $link['localized_options']['attributes']['class'][] = 'pagetabs-select';
      }
    }
  }
}

/**
 * Wraps page tabs in div.pagetabs-nav.
 */
function acton_menu_local_tasks_wrap($children, &$elements) {
  $attributes = isset($elements['#attributes']) ? $elements['#attributes'] : array();
  $elements['#prefix'] = '<div' . drupal_attributes($attributes) . '>' . $elements['#prefix'];
  $elements['#suffix'] .= '</div>';
  return $children;
}

/**
 * Add classes to menu links.
 */
function acton_preprocess_menu_link(&$variables) {
  // Set up element references.
  $element = &$variables['element'];
  $link = $element['#original_link'];
  $class = &$element['#attributes']['class'];
  $link_class = &$element['#localized_options']['attributes']['class'];
  if (empty($link_class)) {
    $link_class = array();
  }

  // Add child items chevron indicator.
  if (!empty($link['has_children'])) {
    $link_class[] = 'parent';
    if (!empty($link['in_active_trail'])) {
      $link_class[] = 'trail';
    }
  }

  // Highlight deepest link in trail.
  $trail_register = &drupal_static(__FUNCTION__, array());
  $menu_name = $link['menu_name'];
  if (!empty($link['in_active_trail']) && !isset($trail_register[$menu_name])) {
    // Highlight the link if:
    // - It has no children, or
    // - No children is in active trail.
    $highlight = FALSE;
    if (!empty($element['#below'])) {
      // Highlight if no children is active.
      foreach (element_children($element['#below']) as $mlid) {
        // Only highlight if there are actual child links.
        $highlight = TRUE;
        // Remove highlight if one child link is active.
        if (!empty($element['#below'][$mlid]['#original_link']['in_active_trail'])) {
          $highlight = FALSE;
          break;
        }
      }
    }
    else {
      // Highlight if there are no children.
      $highlight = TRUE;
    }
    // Highlight link.
    if ($highlight) {
      $link_class[] = 'selected';
      // Mark menu as highlighted.
      $trail_register[$menu_name] = TRUE;
    }
  }

  // Highlight front page link.
  if (theme_get_setting('acton_menu_highlight_frontpage') && !in_array('selected', $link_class) &&
    $link['link_path'] == '<front>' && is_null(menu_tree_get_path($link['menu_name']))) {
    $link_class[] = 'selected';
  }

  // Test if menu link is active (emulate l()).
  $path = $link['link_path'];
  if (!in_array('selected', $link_class) &&
    ($path == $_GET['q'] || ($path == '<front>' && drupal_is_front_page())) &&
    (empty($options['language']) || $options['language']->language == $GLOBALS['language_url']->language)) {
    $link_class[] = 'selected';
  }
}

/**
 * Default web style version.
 * @deprecated
 */
define('ACTON_DEFAULT_STYLE_VERSION', 4);
