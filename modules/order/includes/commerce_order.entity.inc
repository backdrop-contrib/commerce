<?php 

/**
 * Defines the CommerceOrder Entity.
 */
class CommerceOrder extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_order');
  }

  public function id() {
    return $this->id;
  }

  public function entityType() {
    return 'commerce_order';
  }

  public function label() {
    return $this->name;
  }

  public function uri() {
    if (empty($this->id)) {
      return [];
    }
    return [
      'path' => 'commerce_order/' . $this->id
    ];
  }

}
