<?php 

/**
 * Defines the CommerceLineItem Entity
 */
class CommerceLineItem extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_line_item');
  }

  public function id() {
    return $this->id;
  }

  public function entityType() {
    return 'commerce_line_item';
  }

  public function label() {
    return $this->name;
  }

  public function uri() {
    if (empty($this->id)) {
      return [];
    }
    return [
      'path' => 'commerce_line_item/' . $this->id
    ];
  }

}
