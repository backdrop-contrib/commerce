<?php 

/**
 * Defines the CommerceCustomer Entity.
 */
class CommerceCustomerProfile extends Entity {

  public $langcode = LANGUAGE_NONE;

  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_customer_profile');
  }

  public function id() {
    return $this->profile_id;
  }

  public function entityType() {
    return 'commerce_customer_profile';
  }

  public function label() {
    return "Customer Profile (uid {$this->uid})";
  }

  public function bundle() {
    return $this->type;
  }

  public function uri() {
    if (empty($this->id)) {
      return array();
    }
    return [
      'path' => 'commerce_customer_profile/' . $this->id,
      'options' => [],
    ];
  }

}
