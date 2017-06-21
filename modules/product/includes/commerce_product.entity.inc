<?php

/**
 * @file
 * @todo.
 */

/**
 * Class CommerceProduct
 */
class CommerceProduct extends CommerceEntity {

  /**
   * @var mixed
   */
  public $status;

  /**
   * @var int
   */
  public $uid;

  /**
   * @var
   */
  public $language;

  /**
   * @var int
   */
  public $revision_id;

    /**
     * @var string
     */
  public $type;

  /**
   * @var
   */
  public $product_id;

  /**
   * @var
   */
  public $sku;

  /**
   * @var
   */
  public $title;

  /**
   * {@inheritdoc}
   */
  public function getEntityType() {
    return 'commerce_product';
  }

  /**
   * @return mixed
   */
  public function getProductId() {
    return $this->product_id;
  }

  /**
   * @return mixed
   */
  public function getSku() {
    return $this->sku;
  }

  /**
   * @param mixed $sku
   */
  public function setSku($sku) {
    $this->sku = $sku;
  }

  /**
   * @return mixed
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * @param mixed $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * @return EntityDrupalWrapper
   * @throws \EntityMetadataWrapperException
   */
  public function getUser() {
    return $this->wrapper()->get('user');
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
  public function getLanguage() {
    return $this->language;
  }

  /**
   * @param mixed $language
   */
  public function setLanguage($language) {
    $this->language = $language;
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
