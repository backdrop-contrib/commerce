<?php

/**
 * @file
 * The controller for the product entity containing the CRUD operations.
 */

/**
 * The controller class for products contains methods for the product CRUD
 * operations.
 *
 * Mainly relies on the EntityAPIController class provided by the Entity
 * module, just overrides specific features.
 */
class CommerceProductEntityController extends BackdropCommerceEntityController implements EntityPlusControllerRevisionableInterface {

  public $is_active_revision = TRUE;

  /**
   * Create a default product.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return
   *   A product object with all default fields initialized.
   */
  public function create(array $values = []) {
    $values += [
      'product_id' => NULL,
      'is_new' => TRUE,
      'sku' => '',
      'revision_id' => NULL,
      'title' => '',
      'uid' => '',
      'status' => 1,
      'created' => '',
      'changed' => '',
      'is_active_revision' => TRUE,
    ];

    return parent::create($values);
  }

  /**
   * Implements Backdrop\entity\EntityInterface::isActiveRevision().
   */
  public function isActiveRevision() {
    return $this->is_active_revision;
  }

  /**
   * Implements Backdrop\entity\EntityInterface::setActiveRevision().
   */
  public function setIsActiveRevision() {
    $this->is_active_revision = TRUE;
  }

  /**
   * Saves a product.
   *
   * @param $entity
   *   The full product object to save.
   * @param $transaction
   *   An optional transaction object.
   *
   * @return
   *   SAVED_NEW or SAVED_UPDATED depending on the operation performed.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    global $user;

    // Hardcode the changed time.
    $entity->changed = REQUEST_TIME;

    if (empty($entity->{$this->idKey}) || !empty($entity->is_new)) {
      // Set the creation timestamp if not set, for new entities.
      if (empty($entity->created)) {
        $entity->created = REQUEST_TIME;
      }
    }
    else {
      // Otherwise if the product is not new but comes from an entity_create()
      // or similar function call that initializes the created timestamp and uid
      // value to empty strings, unset them to prevent destroying existing data
      // in those properties on update.
      if ($entity->created === '') {
        unset($entity->created);
      }
      if ($entity->uid === '') {
        unset($entity->uid);
      }
    }

    $entity->revision_timestamp = REQUEST_TIME;
    $entity->revision_uid = $user->uid;

    // Determine if we will be inserting a new product.
    $entity->is_new = empty($entity->product_id);

    if ($entity->is_new || !empty($entity->revision)) {
      // When inserting either a new product or revision, $entity->log must be set
      // because {commerce_product_revision}.log is a text column and therefore
      // cannot have a default value. However, it might not be set at this
      // point, so we ensure that it is at least an empty string in that case.
      if (!isset($entity->log)) {
        $entity->log = '';
      }
    }
    elseif (empty($entity->log)) {
      // If we are updating an existing product without adding a new revision,
      // we need to make sure $entity->log is unset whenever it is empty. As
      // long as $entity->log is unset, drupal_write_record() will not attempt
      // to update the existing database column when re-saving the revision.
      unset($entity->log);
    }

    // Remove price components from any price fields attached to the product.
    // Default price components should instead be rebuilt each load using
    // hook_field_attach_load().
    foreach (field_info_instances('commerce_product', $entity->type) as $field_name => $instance) {
      // Load the instance's field data.

      $field = field_info_field($instance['field_name']);

      // If the instance is a price field with data on this product...
      if ($field['type'] == 'commerce_price' && !empty($entity->{$field_name})) {
        // Remove the price components from every price value.
        foreach ($entity->{$field_name} as $langcode => &$items) {
          foreach ($items as $delta => &$item) {
            if (!empty($item['data'])) {
              $item['data']['components'] = [];
            }
          }
        }
      }
    }

    // Reset load by SKU static cache.
    backdrop_static_reset('commerce_product_load_by_sku');

    return parent::save($entity, $transaction);
  }

  /**
   * Unserializes the data property of loaded products.
   */
  public function attachLoad(&$queried_products, $revision_id = FALSE) {
    foreach ($queried_products as $product_id => &$product) {
      $product->data = unserialize($product->data);
    }

    // Call the default attachLoad() method. This will add fields and call
    // hook_commerce_product_load().
    parent::attachLoad($queried_products, $revision_id);
  }

  /**
   * Deletes multiple products by ID.
   *
   * @param $product_ids
   *   An array of product IDs to delete.
   * @param $transaction
   *   An optional transaction object.
   *
   * @return
   *   TRUE on success, FALSE otherwise.
   */
  public function delete($product_ids, DatabaseTransaction $transaction = NULL) {
    if (!empty($product_ids)) {
      $products = $this->load($product_ids, []);

      // Ensure the products can actually be deleted.
      foreach ((array) $products as $product_id => $product) {
        if (!commerce_product_can_delete($product)) {
          unset($products[$product_id]);
        }
      }

      // If none of the specified products can be deleted, return FALSE.
      if (empty($products)) {
        return FALSE;
      }

      parent::delete(array_keys($products), $transaction);
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function deleteRevision($revision_id) {
    if ($revisions = commerce_product_load_multiple([], ['revision_id' => $revision_id])) {
      $revision = $revisions ? reset($revisions) : FALSE;
      // Prevent deleting the current revision.
      if (!$revision || commerce_product_is_latest_revision($revision)) {
        return FALSE;
      }

      // Delete product revision.
      db_delete('commerce_product_revision')
        ->condition('product_id', $revision->product_id)
        ->condition('revision_id', $revision->revision_id)
        ->execute();

      $this->invoke('commerce_product_revision_delete', $revision);
      field_attach_delete_revision('commerce_product', $revision);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Builds a structured array representing the entity's content.
   *
   * The content built for the entity will vary depending on the $view_mode
   * parameter.
   *
   * @param $entity
   *   An entity object.
   * @param $view_mode
   *   View mode, e.g. 'full', 'teaser'...
   * @param $langcode
   *   (optional) A language code to use for rendering. Defaults to the global
   *   content language of the current request.
   *
   * @return
   *   The renderable array.
   */
  public function buildContent(EntityInterface $product, $view_mode = 'full', $langcode = NULL, $content = []) {
    // Prepare a reusable array representing the CSS file to attach to the view.
    $attached = [
      'css' => [backdrop_get_path('module', 'commerce_product') . '/theme/commerce_product.theme.css'],
    ];

    // Add the default fields inherent to the product entity.
    $content['sku'] = [
      '#markup' => theme('commerce_product_sku', [
        'sku' => $product->sku,
        'label' => t('SKU:'),
        'product' => $product,
      ]),
      '#attached' => $attached,
    ];
    $content['title'] = [
      '#markup' => theme('commerce_product_title', [
        'title' => $product->title,
        'label' => t('Title:'),
        'product' => $product,
      ]),
      '#attached' => $attached,
    ];
    $content['status'] = [
      '#markup' => theme('commerce_product_status', [
        'status' => $product->status,
        'label' => t('Status:'),
        'product' => $product,
      ]),
      '#attached' => $attached,
    ];

    return parent::buildContent($product, $view_mode, $langcode, $content);
  }

}
