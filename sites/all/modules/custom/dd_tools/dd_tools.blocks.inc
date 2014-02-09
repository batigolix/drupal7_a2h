<?php

/**
 * Implements hook block info
 * Defines custom blocks
 */
function dd_tools_block_info() {
  $blocks['slideshow'] = array(
      'info' => t('DD tools: Slideshow'),
      'cache' => DRUPAL_CACHE_GLOBAL,
  );
  return $blocks;
}

/**
 * Defines title and content of custom blocks
 * Defines which function is called for block. Functions can be found in
 * carcan_awar_blocks.inc
 */
function dd_tools_block_view($delta = '') {
  $blocks = array();
  switch ($delta) {
    case 'slideshow':
      $blocks['content'] = dd_tools_slideshow();
      break;
  }
  return $blocks;
}
function dd_tools_slideshow() {
  $items = variable_get('slideshow_items', 5);
  $query = db_select('node', 'n');
  $query->condition('n.promote', 1);
  $query->range(0, $items);

  $query->condition('n.status', 1);
  $query->condition('n.type', 'object');
  $query->fields('n', array('nid', 'title'));

//  $query->addField('fdfc', 'entity_id', 'entid');
  $results = $query->execute();

  foreach ($results as $result) {
    $nids[] = $result->nid;
  }
  $nodes = node_load_multiple($nids);
  $items = array();
  foreach ($nodes as $node) {
    $node_view = node_view($node, 'hero_teaser');
    $items[] = array('data' => drupal_render($node_view));
  }
    $lib_path = libraries_get_path('responsiveslides');
  drupal_add_js($lib_path . '/responsiveslides.min.js');
  $rs_js = 'jQuery(function() {jQuery(".rslides").responsiveSlides({auto: true, timeout: 3000, speed: 500, pager: true, pauseControls: true, navContainer: ""});});';
  drupal_add_js($rs_js, 'inline');
  return theme('item_list', array('items' => $items, 'title' => NULL, 'type' => 'ul', 'attributes' => array('class' => array('rslides'))));
}
/**
 * Implements hook_block_configure().
 */
function dd_tools_block_configure($delta = '') {
  $form = array();
  if ($delta == 'slideshow') {
    $form['slideshow_items'] = array(
      '#type' => 'select',
      '#title' => t('Number of items'),
      '#options' => array(2,5,10),
      '#description' => t('This number of items will be shown in the slideshow block'),
      '#default_value' => variable_get('slideshow_items', 5),
    );
  }
  return $form;
}
/**
 * Implements hook_block_save().
 */
function dd_tools_block_save($delta = '', $edit = array()) {
  if ($delta == 'slideshow') {
    variable_set('slideshow_items', $edit['slideshow_items']);
  }
}