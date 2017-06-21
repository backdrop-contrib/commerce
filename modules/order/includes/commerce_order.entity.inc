<?php

/**
 * @file
 * @todo.
 */

/**
 * Class CommerceOrder
 */
class CommerceOrder extends CommerceEntity {

  /**
   * @var mixed
   */
  public $status;

  /**
   * @var int
   */
  public $uid;

  /**
   * @var int
   */
  public $revision_id;

  /**
   * @var string
   */
  public $type;

  /**
   * The internal numeric ID of the order.
   *
   * @var string
   */
  public $order_id;

  /**
   * The order number displayed to the customer.
   *
   * @var string
   */
  public $order_number;

  /**
   * The state of the order derived from its status.
   *
   * @var string
   */
  public $state;

  /**
   * The IP address that created this order.
   *
   * @var string
   */
  public $hostname;

  /**
   * The e-mail address associated with this order.
   *
   * @var string
   */
  public $mail;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values);
  }

  /**
   * {@inheritdoc}
   */
  public function getEntityType() {
    return 'commerce_order';
  }

  /**
   * @return string
   */
  public function getOrderId() {
    return $this->order_id;
  }

  /**
   * Returns the order number.
   *
   * @return string
   */
  public function getOrderNumber() {
    return $this->order_number;
  }

  /**
   * Sets the order number.
   *
   * @param $string
   *
   * @return $this
   */
  public function setOrderNumber($string) {
    $this->order_number = $string;
    return $this;
  }
  
  /**
   * @return mixed
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * Sets the order status.
   *
   * @param string $status
   *
   * @return $this
   * @throws CommerceException
   */
  public function setStatus($status) {
    if (commerce_order_status_load($status)) {
      $this->status = $status;
      return $this;
    }
    else {
      throw new CommerceException(t('Invalid order status'));
    }
  }

  /**
   * Returns the order's state.
   *
   * @return string
   */
  public function getState() {
    return $this->state;
  }

  /**
   * @return string
   */
  public function getHostname() {
    return $this->hostname;
  }

  /**
   * Wrapper for user entity that owns the order.
   *
   * @return EntityDrupalWrapper
   *
   * @throws \EntityMetadataWrapperException
   */
  public function getOwner() {
    return $this->wrapper()->get('owner');
  }

  /**
   * @return string
   */
  public function getMail() {
    return $this->mail;
  }

  /**
   * Set the e-mail used for the order.
   *
   * @param string $mail
   * @return $this
   */
  public function setMail($mail) {
    $this->mail = $mail;
    return $this;
  }

  /**
   * @return string
   */
  public function getMailUsername() {
    return $this->wrapper()->get('mail_username');
  }

  /**
   * Returns line item entities from order.
   *
   * @return EntityStructureWrapper|EntityListWrapper
   * @throws \EntityMetadataWrapperException
   */
  public function getLineItems() {
    return $this->wrapper()->get('commerce_line_items');
  }

  /**
   * Sets line item reference field to array of line items.
   *
   * @param array $values
   * @return $this
   */
  public function setLineItems(array $values) {
    $this->wrapperGet('commerce_line_items')->set($values);
    return $this;
  }

  /**
   * Adds a line item to the order.
   *
   * @param $lineItem
   *
   * @return $this
   * @throws \CommerceException
   */
  public function addLineItem($lineItem) {
    $this->wrapper->{'commerce_line_items'}[] = $this->resolveEntity($lineItem);
    return $this;
  }

  /**
   * Returns order price structure.
   *
   * @param string $type
   * @return EntityStructureWrapper
   * @throws \EntityMetadataWrapperException
   */
  public function getOrderTotal($type = NULL) {
    /** @var EntityStructureWrapper $structure */
    $structure = $this->wrapper()->get('commerce_order_total');
    switch ($type) {
      case 'amount':
        return $structure->get('amount');

      case 'amount_decimal':
        return $structure->get('amount_decimal');

      case 'currency_code':
        return $structure->get('currency_code');

      case 'data':
        return $structure->get('data');

      default:
        return $structure;
    }
  }

  /**
   * @return mixed
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * @param mixed $uid
   */
  public function setUid($uid) {
    $this->uid = $uid;
  }

  /**
   * @return mixed
   */
  public function getRevisionId() {
    return $this->revision_id;
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
