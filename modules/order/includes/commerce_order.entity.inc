<?php 

/**
 * Defines the CommerceOrder Entity.
 */
class CommerceOrder extends Entity {

  public $langcode = LANGUAGE_NONE;

  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_order');
  }

  public function id() {
    return $this->order_id;
  }

  public function entityType() {
    return 'commerce_order';
  }

  public function label() {
    return t('Order ') . $this->id();
  }

  public function bundle() {
    return $this->type;
  }

  public function uri() {
    if (empty($this->id)) {
      return array();
    }
    return [
      'path' => 'commerce_order/' . $this->id,
      'options' => [],
    ];
  }

}
