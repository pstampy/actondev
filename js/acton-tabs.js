(function($) {

Drupal.behaviors.acton_tabs = {
  attach: function (context, settings) {
    $('.ui-tabs', context).once('acton-tabs', function () {
      $(this)
        // Wrap tabs bar inside div.pagetabs-nav.
        .find('ul.ui-tabs-nav').each(function () {
          // Wrap only if not already in div.pagetabs-nav.
          if ($(this).parentsUntil('.ui-tabs', '.pagetabs-nav').length == 0) {
            $(this).wrap('<div class="clearfix"></div>').wrap('<div class="pagetabs-nav"></div>');
          }
        })
        .end()
        // Strip jQuery UI widget styling.
        .removeClass('ui-widget ui-widget-content')
        .find('.ui-widget-header').removeClass('ui-widget-header').end()
        .find('.ui-widget-content').removeClass('ui-widget-content').end()
        // Add/remove ANU style class on tab select.
        .find('.ui-tabs-selected a').each(Drupal.behaviors.acton_tabs.selectTab).end()
        .bind('tabsselect', function(event, ui) {
          Drupal.behaviors.acton_tabs.selectTab(ui.index, $(ui.tab));
          return true;
        });
      // Process tabs for onhashchange.
      Drupal.actonHistoryTabs($(this).attr('id'));
    });
  },
  selectTab: function (index, tab) {
    $(tab).addClass('pagetabs-select')
      .closest('li').siblings().find('a').removeClass('pagetabs-select');
  }
};

Drupal.actonHistoryTabs = function (tabsId) {
  // Change hash on tab select.
  $('#' + tabsId).bind('tabsselect', function(event, ui) {
    if (!$(ui.tab).closest('.ui-tabs').data('actonHistoryTabsPreventHashChange')) {
      var tabId = ui.tab.hash.replace('#', '')
      window.location.hash = '#' + actonTabsHashPrefix + tabId;
    }
  });
  // Switch tab on hash change.
  if (!$(window).data('actonHistoryTabs')) {
    $(window).data('actonHistoryTabs', true);
    $(window).bind('hashchange', function () {
      if (window.location.hash.length > 0) {
        Drupal.actonNavigateToPrefixedTab(window.location.hash);
      }
      else {
        // Revert all tabsets to the default tab.
        $('.ui-tabs').each(function () {
          $(this).data('actonHistoryTabsPreventHashChange', true);
          $(this).tabs('select', 0);
          $(this).data('actonHistoryTabsPreventHashChange', false);
        });
      }
    });
  }
};

var actonTabsHashPrefix = 'acton-tabs-link--';
var locationBase = window.location.href.split('#')[0];
var locationHash = window.location.hash.replace('#', '');
var actonTabsHash = (locationHash.indexOf(actonTabsHashPrefix) > -1 && locationHash.length > actonTabsHashPrefix.length)
  ? locationHash.substr(actonTabsHashPrefix.length)
  : null;

Drupal.behaviors.acton_tabs_nav = {
  attach: function (context, settings) {
    // Switch to tab.
    var switchToTab = function (tabs) {
      if (actonTabsHash != null && !tabs.data('acton_tabs_nav')) {
        tabs.tabs();
        Drupal.actonNavigateToTab('#' + actonTabsHash, tabs);
        tabs.data('acton_tabs_nav', true);
      }
    };

    // Switch to tabs.
    $('.ui-tabs', context).each(function (index, element) {
      var tabs = $(element);
      // Defer until after other event handlers have been run.
      $(window).load(function () {
        switchToTab(tabs);
      });
    });
  
    // Transform links.
    $('#body a', context).once('acton-tabs-link', function (index, element) {
      var link = $(element);
      // Check link href for tab hash.
      var url = element.href;
      var hashIndex = url.indexOf('#');
      if (hashIndex > -1) {
        var linkBase = url.substr(0, hashIndex);
        var linkHash = url.substr(hashIndex + 1);
        // Ensure this is not an internal link.
        if (linkBase === locationBase) {
          return;
        }
        // Check if tabs link class does not match.
        if (!settings.acton_tabs_link_class || !link.hasClass(settings.acton_tabs_link_class)) {
          // Check if tabs hash prefixes are not defined.
          if (!settings.acton_tabs_prefix) {
            return;
          }
          // Check if hash prefixes do not match.
          else {
            var matched = false;
            for (var i = 0; i < settings.acton_tabs_prefix.length; i ++) {
              var prefix = settings.acton_tabs_prefix[i];
              if (linkHash.indexOf(prefix) === 0) {
                matched = true;
                break;
              }
            }
            if (!matched) {
              return;
            }
          }
        }
        // Transform link hash.
        url = linkBase + '#' + actonTabsHashPrefix + linkHash;
        link.attr('href', url);
      }
    });
  }
};

Drupal.actonNavigateToPrefixedTab = function (hash) {
  var locationFragment = window.location.hash.replace('#', '');
  if (locationFragment.indexOf(actonTabsHashPrefix) > -1) {
    var tabSelector = '#' + locationFragment.substr(actonTabsHashPrefix.length);
    $(tabSelector).closest('.ui-tabs').each(function (index, element) {
      Drupal.actonNavigateToTab(tabSelector, $(element));
    });
  }
};

Drupal.actonNavigateToTab = function (tabSelector, tabs) {
  if ($(tabSelector).length > 0) {
    tabs.tabs('select', tabSelector);
  }
};

})(jQuery);