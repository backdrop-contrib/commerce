
(function ($) {

  Backdrop.behaviors.orderFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-order-status', context).backdropSetSummary(function (context) {
      // If the status has been changed, indicate the original status.
      if ($('#edit-status').val() != $('#edit-status-original').val()) {
        return Backdrop.t('From @title', { '@title' : Backdrop.settings.status_titles[$('#edit-status-original').val()] }) + '<br />' + Backdrop.t('To @title', { '@title' : Backdrop.settings.status_titles[$('#edit-status').val()] });
      }
      else {
        return Backdrop.settings.status_titles[$('#edit-status').val()];
      }
    });

    $('fieldset#edit-user', context).backdropSetSummary(function (context) {
      var name = $('#edit-name').val() || Backdrop.settings.anonymous,
        mail = $('#edit-mail').val();
      return mail ?
        Backdrop.t('Owned by @name', { '@name' : name }) + '<br />' + mail :
        Backdrop.t('Owned by @name', { '@name': name });
    });

    $('fieldset#edit-order-history', context).backdropSetSummary(function (context) {
      var summary = $('#edit-created', context).val() ?
        Backdrop.t('Created @date', { '@date' : $('#edit-created').val() }) :
        Backdrop.t('New order');

      if ($('#edit-placed', context).val()) {
        summary += '<br />' + Backdrop.t('Placed @date', { '@date' : $('#edit-placed').val() });
      }

      // Add the changed date to the summary if it's different from the created.
      if ($('#edit-created', context).val() != $('#edit-changed', context).val()) {
        summary += '<br />' + Backdrop.t('Updated @date', { '@date' : $('#edit-changed').val() });
      }

      return summary;
    });
  }
};

})(jQuery);
