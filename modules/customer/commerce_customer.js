(function($) {

Backdrop.behaviors.customerFieldsetSummaries = {
  attach: function (context, settings) {
    $('fieldset#edit-user', context).backdropSetSummary(function (context) {
      var name = $('#edit-name').val() || Backdrop.settings.anonymous;

      return Backdrop.t('Owned by @name', { '@name': name });
    });

    $('fieldset#edit-profile-status', context).backdropSetSummary(function (context) {
      return ($('input[@name=status]:checked').val() == 0) ?
        Backdrop.t('Disabled') :
        Backdrop.t('Active');
    });
  }
};

})(jQuery);
