<?php

/**
 * @file
 * Page callbacks and form builder functions for administering orders.
 */


/**
 * Form callback: edit the global order settings.
 */
function commerce_order_settings_form($form, &$form_state) {
  $config = config('commerce_order.settings');
  $form['commerce_order_help_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Order creation help text'),
    '#description' => t('Supply an optional help message to be displayed above the order add form.'),
    '#default_value' => $config->get('commerce_order_help_text'),
    '#weight' => -10,
  );
  $form['commerce_order_auto_revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revisions when orders are updated by default.'),
    '#description' => t('This default may be overridden on the order edit form but will always be respected for other order status updates.'),
    '#default_value' => $config->get('commerce_order_auto_revision'),
    '#weight' => 0,
  );
  $form['#submit'][] = 'commerce_order_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Submit handler for the checkout pane settings form's save button.
 */
function commerce_order_settings_form_submit($form, &$form_state) {
  $commerce_order_help_text = $form_state['values']['commerce_order_help_text'];
  $commerce_order_auto_revision = $form_state['values']['commerce_order_auto_revision'];
  $commerce_checkout_permit_multiple_completions = $form_state['values']['commerce_checkout_permit_multiple_completions'];
  $commerce_order_simulate_checkout_link = $form_state['values']['commerce_order_simulate_checkout_link'];

  config_set('commerce_order.settings', 'commerce_order_help_text', $commerce_order_help_text);
  config_set('commerce_order.settings', 'commerce_order_auto_revision', $commerce_order_auto_revision);
  config_set('commerce_checkout.settings', 'commerce_checkout_permit_multiple_completions', $commerce_checkout_permit_multiple_completions);
  config_set('commerce_checkout.settings', 'commerce_order_simulate_checkout_link', $commerce_order_simulate_checkout_link);
}

/**
 * Form callback wrapper: create or edit an order.
 *
 * @param $order
 *   The order object to edit through the form.
 * @param $account
 *   For new orders, the customer's user account.
 *
 * @see commerce_order_order_form()
 */
function commerce_order_ui_order_form_wrapper($order = NULL, $account = NULL) {
  if (empty($order)) {
    $order = commerce_order_new();
  }

  // Set the page title and a default customer if necessary.
  if (empty($order->order_id)) {
    backdrop_set_title(t('Create an order'));

    if (!empty($account)) {
      $order->uid = $account->uid;
    }
  }

  // Include the forms file from the Order module.
  module_load_include('inc', 'commerce_order', 'includes/commerce_order.forms');
  return backdrop_get_form('commerce_order_ui_order_form', $order);
}

/**
 * Form callback wrapper: confirmation form for deleting an order.
 *
 * @param $order
 *   The order object to delete through the form.
 *
 * @see commerce_order_order_delete_form()
 */
function commerce_order_ui_order_delete_form_wrapper($order) {
  // Include the forms file from the Order module.
  module_load_include('inc', 'commerce_order', 'includes/commerce_order.forms');
  return backdrop_get_form('commerce_order_ui_order_delete_form', $order);
}
