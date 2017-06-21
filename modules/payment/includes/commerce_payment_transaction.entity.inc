<?php

/**
 * @file
 * @todo.
 */

/**
 * Class CommercePaymentTransaction
 */
class CommercePaymentTransaction extends CommerceEntity {

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
   * @var
   */
  public $transaction_id;

  /**
   * @var
   */
  public $order_id;
  /**
   * @var
   */
  public $payment_method;
  /**
   * @var
   */
  public $instance_id;
  /**
   * @var
   */
  public $remote_id;
  /**
   * @var
   */
  public $message;
  /**
   * @var array
   */
  public $message_variables = array();
  /**
   * @var
   */
  public $amount;
  /**
   * @var
   */
  public $currency_code;
  /**
   * @var
   */
  public $remote_status;
  /**
   * @var
   */
  public $payload;

  /**
   * {@inheritdoc}
   */
  public function getEntityType() {
    return 'commerce_payment_transaction';
  }

  /**
   * @return mixed
   */
  public function getTransactionId() {
    return $this->transaction_id;
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
  public function getOrderId() {
    return $this->order_id;
  }

  /**
   * @return EntityDrupalWrapper
   * @throws \EntityMetadataWrapperException
   */
  public function getOrder() {
    return $this->wrapper()->get('order');
  }

  /**
   * Returns payment method structure array.
   *
   * @return mixed
   */
  public function getPaymentMethod() {
    return commerce_payment_method_load($this->payment_method);
  }

  /**
   * @param mixed $payment_method
   */
  public function setPaymentMethod($payment_method) {
    $this->payment_method = $payment_method;
  }

  /**
   * @return mixed
   */
  public function getInstanceId() {
    return $this->instance_id;
  }

  /**
   * @return mixed
   */
  public function getRemoteId() {
    return $this->remote_id;
  }

  /**
   * @param mixed $remote_id
   */
  public function setRemoteId($remote_id) {
    $this->remote_id = $remote_id;
  }

  /**
   * @return mixed
   */
  public function getMessage() {
    return t($this->message, $this->getMessageVariables());
  }

  /**
   * @param mixed $message
   */
  public function setMessage($message) {
    $this->message = $message;
  }

  /**
   * @return mixed
   */
  public function getMessageVariables() {
    return $this->message_variables;
  }

  /**
   * @param mixed $message_variables
   */
  public function setMessageVariables(array $message_variables) {
    $this->message_variables = $message_variables;
  }

  /**
   * @return mixed
   */
  public function getAmount() {
    return $this->amount;
  }

  /**
   * @param mixed $amount
   */
  public function setAmount($amount) {
    $this->amount = $amount;
  }

  /**
   * @return mixed
   */
  public function getCurrencyCode() {
    return $this->currency_code;
  }

  /**
   * @param mixed $currency_code
   */
  public function setCurrencyCode($currency_code) {
    $this->currency_code = $currency_code;
  }

  /**
   * @return mixed
   */
  public function getRemoteStatus() {
    return $this->remote_status;
  }

  /**
   * @param mixed $remote_status
   */
  public function setRemoteStatus($remote_status) {
    $this->remote_status = $remote_status;
  }

  /**
   * @return mixed
   */
  public function getPayload() {
    return $this->payload;
  }

  /**
   * @param mixed $payload
   */
  public function setPayload($payload) {
    $this->payload = $payload;
  }

  /**
   * @return mixed
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * {@inheritdoc}
   *
   * @throws CommerceException
   */
  public function setStatus($status) {
    if (!in_array($status, array(COMMERCE_PAYMENT_STATUS_FAILURE, COMMERCE_PAYMENT_STATUS_PENDING, COMMERCE_PAYMENT_STATUS_SUCCESS))) {
      throw new CommerceException(t('Invalid payment transaction status'));
    }
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
}
