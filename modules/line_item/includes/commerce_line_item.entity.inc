<?php

/**
 * @file
 * Contains CommerceLineItem class.
 */

/**
 * Class CommerceLineItem
 */
class CommerceLineItem extends CommerceEntity {

  /**
   * @var string
   */
  public $type;

  /**
   * @var
   */
  public $line_item_id;

  /**
   * @var
   */
  public $order_id;

  /**
   * @var
   */
  public $line_item_label;

  /**
   * @var
   */
  public $quantity;

  /**
   * {@inheritdoc}
   */
  public function getEntityType() {
    return 'commerce_line_item';
  }

  /**
   * @return mixed
   */
  public function getLineItemId() {
    return $this->line_item_id;
  }

  /**
   * @return mixed
   */
  public function getOrderId() {
    return $this->order_id;
  }

  /**
   * @param mixed $order_id
   */
  public function setOrderId($order_id) {
    $this->order_id = $order_id;
  }

  /**
   * @return EntityDrupalWrapper
   *
   * @throws \EntityMetadataWrapperException
   */
  public function getOrder() {
    return $this->wrapper()->get('order');
  }

  /**
   * @return mixed
   */
  public function getLineItemLabel() {
    return $this->line_item_label;
  }

  /**
   * @param mixed $line_item_label
   */
  public function setLineItemLabel($line_item_label) {
    $this->line_item_label = $line_item_label;
  }

  /**
   * @return mixed
   */
  public function getQuantity() {
    return $this->quantity;
  }

  /**
   * @param mixed $quantity
   */
  public function setQuantity($quantity) {
    $this->quantity = $quantity;
  }

  /**
   * @return mixed
   */
  public function getType() {
    return $this->type;
  }

  /**
   * @param mixed $type
   */
  public function setType($type) {
    $this->type = $type;
  }

}
