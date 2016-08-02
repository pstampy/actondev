<?php
/**
 * @file
 * Contains infrequently used theme registry build functions.
 */

/**
 * Implements hook_theme().
 */
function acton_theme(&$existing, $type, $theme, $path) {
  $hooks = array();
  $hooks['acton_page_requisite_default'] = array(
    'variables' => array('part' => NULL),
    'template' => 'acton-page-requisite-default',
    'path' => $path . '/templates/default',
  );
  $hooks += drupal_find_theme_templates($hooks, '.tpl.php', $path . '/templates/default');

  $hooks['acton_alert_bar_default'] = array(
    'variables' => array(),
    'template' => 'acton-alert-bar-default',
    'path' => $path . '/templates/default',
  );

  return $hooks;
}

/**
 * Implements hook_preprocess_acton_page_requisite_default().
 */
function acton_preprocess_acton_page_requisite_default(&$variables) {
  if (!empty($variables['part'])) {
    $variables['theme_hook_suggestions'][] = 'acton_page_requisite_default';
    $variables['theme_hook_suggestions'][] = 'acton_page_requisite_default__' . $variables['part'];
  }
}
