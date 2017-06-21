<?php

/**
 * @file
 * Contains the CommerceCustomerProfile class.
 */

/**
 * Class CommerceCustomerProfile.
 */
class CommerceCustomerProfile extends CommerceEntity {

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
   * @var int
   */
  public $profile_id;

  /**
   * {@inheritdoc}
   */
  public function getEntityType() {
    return 'commerce_customer_profile';
  }

  /**
   * @return mixed
   */
  public function getProfileId() {
    return $this->profile_id;
  }

  /**
   * @return mixed
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * @param mixed $status
   */
  public function setStatus($status) {
    $this->status = $status;
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
