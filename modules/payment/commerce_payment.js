;(function($) {

  /**
   * Automatically submit the payment redirect form.
   */
  Backdrop.behaviors.commercePayment = {
    attach: function (context, settings) {
      $('div.payment-redirect-form form', context).submit();
    }
  }
})(jQuery);
