<?php

/**
 * @file
 * Administrative page callbacks for the Product Pricing UI module.
 */


/**
 * Builds the product sell price calculation Rules Overview page.
 */
function commerce_product_pricing_ui_sell_price_rules() {
  RulesPluginUI::$basePath = 'admin/commerce/config/product-pricing/rules';
  $options = array('show plugin' => FALSE);

  $content['enabled']['title']['#markup'] = '<h3>' . t('Enabled sell price calculation rules') . '</h3>';

  $conditions = array(
    'event' => 'commerce_product_calculate_sell_price',
    'plugin' => 'reaction rule',
    'active' => TRUE,
  );
  $content['enabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['enabled']['rules']['#empty'] = t('There are no active sell price calculation rules.');

  $content['disabled']['title']['#markup'] = '<h3>' . t('Disabled sell price calculation rules') . '</h3>';

  $conditions['active'] = FALSE;
  $content['disabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['disabled']['rules']['#empty'] = t('There are no disabled sell price calculation rules.');

  // Store the function name in the content array to make it easy to alter the
  // contents of this page.
  $content['#page_callback'] = 'commerce_product_pricing_ui_sell_price_rules';

  return $content;
}

/**
 * Displays the settings form for managing product sell price pre-calculation.
 */
function commerce_product_pre_calculation_settings_form($form, &$form_state) {
  // Count the number of rows in the price pre-calculation table.
  $query = db_select('commerce_calculated_price')
    ->fields('commerce_calculated_price', array('created'))
    ->condition('module', 'commerce_product_pricing')
    ->condition('entity_type', 'commerce_product')
    ->condition('field_name', 'commerce_price');

  $count_query = clone($query);
  $count = $count_query->countQuery()->execute()->fetchField();

  // If there are rows in the table or price pre-calculation is enabled, show
  // the management fieldset with its action buttons.
  // TODO This variable was probably removed in Backdrop without replacement.
  if ($count > 0 || variable_get('commerce_product_sell_price_pre_calculation', 'disabled') != 'disabled') {
    // Build a description for the fieldset indicating how many rows are in the
    // table and when they were last processed.
    $description = format_plural($count,
      'There is 1 product sell price in the calculated price table.',
      'There are @count product sell prices in the calculated price table.');

    // If there are prices, add the timestamp for the last calculation.
    if ($count > 0) {
      $description .= ' ' . t('The last calculation occured on @date.', array('@date' => format_date($query->execute()->fetchField(), 'short')));
    }

    $form['database'] = array(
      '#type' => 'fieldset',
      '#title' => t('Manage calculated prices'),
      '#description' => '<p>' . $description . '</p>',
    );

    if ($count > 0) {
      $form['database']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete product sell prices'),
      );
    }
    else {
      $form['database']['batch_calculate'] = array(
        '#type' => 'submit',
        '#value' => t('Batch calculate prices now'),
      );
    }
  }
  // Retrieve the current Product pricing callback currently configured.
  $product_pricing_callback = config_get('commerce_product_pricing.settings', 'commerce_product_pricing_callback');

  // TODO This variable was probably removed in Backdrop without replacement.
  $form['commerce_product_sell_price_pre_calculation'] = array(
    '#type' => 'radios',
    '#title' => t('Sell price pre-calculation method'),
    '#description' => t('If pre-calculation is disabled, code that integrates calculated prices into queries and price displays will ignore any existing calculated prices.'),
    '#options' => array(
      'disabled' => t('Disabled'),
      'manual_batch' => t('Manual batch calculation'),
      // TODO: Support automated re-calculation when Rules or Products that have
      // pre-calculated prices are updated.
      // 'automated_batch' => t('Manual batch pre-calculation with automated updates'),
    ),
    '#default_value' => variable_get('commerce_product_sell_price_pre_calculation', 'disabled'),
    '#disabled' => $product_pricing_callback != 'rules_invoke_event',
  );

  // If price pre-calculation is enabled, prevent options to bypass rules.
  // TODO This variable was probably removed in Backdrop without replacement.
  if (variable_get('commerce_product_sell_price_pre_calculation', 'disabled') != 'disabled') {
    // Build an options list of all valid pricing rules and keep track of those
    // that are invalid.
    $options = array();
    $invalid = array();

    $event = rules_get_cache('event_commerce_product_calculate_sell_price');

    foreach ($event as $rule) {
      if (commerce_product_valid_pre_calculation_rule($rule, TRUE)) {
        $options[$rule->name] = t('@label [@name]', array('@label' => $rule->label(), '@name' => $rule->name));
      }
      else {
        $invalid[$rule->name] = t('@label [@name]', array('@label' => $rule->label(), '@name' => $rule->name));
      }
    }

    if (!empty($options)) {
      // TODO This variable was probably removed in Backdrop without replacement.
      $form['commerce_product_sell_price_pre_calculation_rules_bypass'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Bypass these rules when pre-calculating sell prices'),
        '#description' => t('When pre-calculating sell prices, all valid rules will be included in calculating the variations of every price unless specified here to be bypassed.'),
        '#options' => $options,
        '#default_value' => variable_get('commerce_product_sell_price_pre_calculation_rules_bypass', array()),
      );
    }

    if (!empty($invalid)) {
      $form['invalid_rules'] = array(
        '#type' => 'fieldset',
        '#title' => t('Invalid rules for pre-calculation'),
        '#description' => t('The rules listed below are invalid for sell price pre-calculation either because they do not have any conditions (and therefore do not require pre-calculation), they reference product data in a condition data selector, or they reference non-product data in an action data selector.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['invalid_rules']['list'] = array(
        '#markup' => theme('item_list', array('items' => $invalid)),
      );
    }
  }

  $form['commerce_product_pricing_callback'] = array(
    '#type' => 'radios',
    '#title' => t('Price calculation process'),
    '#description' => t('You can only allow modules to directly engage in product pricing when pre-calculation is disabled.'),
    '#options' => array(
      'rules_invoke_event' => t('Evaluate pricing rules in order'),
      'rules_invoke_all' => t('Evaluate module based pricing routines then invoke pricing rules in order'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="commerce_product_sell_price_pre_calculation"]' => array('value' => 'disabled'),
      ),
    ),
    '#default_value' => $product_pricing_callback,
  );
  // If the Product pricing callback configured is set to "module_invoke_all"
  // disable the element and update the description.
  if ($product_pricing_callback == 'module_invoke_all') {
    $form['commerce_product_pricing_callback']['#options']['module_invoke_all'] = t('Only invoke module based pricing routines');
    $form['commerce_product_pricing_callback']['#description'] .= ' ' . t("This value has been set in your site's codebase and cannot be changed via this form.");
    $form['commerce_product_pricing_callback']['#disabled'] = TRUE;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit callback: process the product sell price pre-calculation form.
 */
function commerce_product_pre_calculation_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Save variables on the form regardless of the button pressed.
  // TODO This variable was probably removed in Backdrop without replacement.
  variable_set('commerce_product_sell_price_pre_calculation', $values['commerce_product_sell_price_pre_calculation']);

  if (!empty($values['commerce_product_sell_price_pre_calculation_rules_bypass'])) {
    // TODO This variable was probably removed in Backdrop without replacement.
    variable_set('commerce_product_sell_price_pre_calculation_rules_bypass', $values['commerce_product_sell_price_pre_calculation_rules_bypass']);
  }
  if (!empty($values['commerce_product_pricing_callback'])) {
    if ($values['commerce_product_sell_price_pre_calculation'] == 'disabled') {
      config_set('commerce_product_pricing.settings', 'commerce_product_pricing_callback', $values['commerce_product_pricing_callback']);
    }
    else {
      // If the pre calculation is enabled, revert the Product pricing process
      // to default.
      config_clear('commerce_product_pricing.settings', 'commerce_product_pricing_callback');
    }
  }

  // React to the management buttons if they were used to submit the form.

  // If the batch calculation button was pressed, setup the batch now.
  if (!empty($values['batch_calculate']) && $values['op'] == $values['batch_calculate']) {
    commerce_product_batch_pre_calculate_sell_prices();
  }

  // TODO: Expand the API to allow for deletion of pre-calculated prices and
  // get this query the heck out of here.
  if (!empty($values['delete']) && $values['op'] == $values['delete']) {
    db_delete('commerce_calculated_price')
      ->condition('module', 'commerce_product_pricing')
      ->condition('entity_type', 'commerce_product')
      ->condition('field_name', 'commerce_price')
      ->execute();
  }
}
