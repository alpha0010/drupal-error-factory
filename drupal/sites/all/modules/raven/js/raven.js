/**
 * @file
 * Configures @sentry/browser with the options and user context.
 */

(function (Drupal, Sentry) {

  'use strict';

  Drupal.settings.raven.options.integrations.push(new Sentry.Integrations.BrowserTracing());

  // Additional Sentry configuration can be applied by modifying
  // Drupal.settings.raven.options in custom PHP or JavaScript. Use the latter
  // for Sentry callback functions; library weight can be used to ensure your
  // custom settings are added before this file executes.
  Sentry.init(Drupal.settings.raven.options);

  Sentry.setUser(Drupal.settings.raven.user);

})(Drupal, window.Sentry);
