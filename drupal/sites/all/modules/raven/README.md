Raven: Sentry Integration
=========================

Raven module integrates the
[Sentry PHP](https://github.com/getsentry/sentry-php) and
[@sentry/browser](https://github.com/getsentry/sentry-javascript) SDKs for
[Sentry](https://sentry.io/) into Drupal.

[Sentry](https://sentry.io/) is a realtime application monitoring and error
tracking platform. It specializes in monitoring errors and extracting all the
information needed to do a proper post-mortem without any of the hassle of the
standard user feedback loop.


## Features

This module logs errors in a few ways:

* Register error handler for fatal errors
* Handle watchdog messages
* Record Sentry breadcrumbs for watchdog messages
* Handle JavaScript exceptions via @sentry/browser.
* Send Content Security Policy (CSP) reports to Sentry (if
  [Security Kit module](https://www.drupal.org/project/seckit) is installed).

You can choose which errors you want to catch by enabling desired error
handlers and selecting error levels.

If desired, the SENTRY_DSN, SENTRY_ENVIRONMENT and SENTRY_RELEASE environment
variables can be used to configure this module, overriding the corresponding
settings at admin/config/development/logging.


## Installation for Drupal 7

1. Choose a module to autoload your site's composer dependencies; for example,
   [Composer Autoloader](https://www.drupal.org/project/composer_autoloader)
   module may work well for you. Or setup the autoloader in your custom code.
2. Install Sentry PHP via composer, for example `composer require drupal/raven`
   to install both this module and Sentry PHP, or
   `composer require sentry/sdk:^3` if you install this module manually.


## Dependencies

* The [Sentry PHP SDK](https://github.com/getsentry/sentry-php) version 3.x
installed by composer. You will also need a module to autoload your composer
dependencies, such as
[Composer Autoloader](https://www.drupal.org/project/composer_autoloader), or
custom code to setup the autoloader.


## Information for developers

You can attach an extra information to error reports (logged in user details,
modules versions, etc). See `raven.api.php` for examples.


## Drush integration

The `drush raven-capture-message` command sends a message to Sentry.


## Known issues

If you have code that generates thousands of PHP notices—for example processing
a large set of data, with one notice for each item—you may find that storing and
sending the errors to Sentry requires a large amount of memory and execution
time, enough to exceed your configured memory_limit and max_execution_time
settings. This could result in a stalled or failed request. The work-around for
this case would be to disable sending PHP notices to Sentry, or, for
long-running processes, to periodically call `raven_flush()`.


## Maintainers

This module is not affiliated with Sentry; it is developed and maintained by
[mfb](https://www.drupal.org/u/mfb).
