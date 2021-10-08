<?php

/**
 * @file
 * Contains the administration pages for Raven.
 */

/**
 * Adds Raven settings form to form.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The form state.
 */
function raven_settings_form(array &$form, array &$form_state) {
  $form['raven'] = [
    '#type' => 'fieldset',
    '#title' => t('Sentry'),
    '#collapsible' => TRUE,
  ];

  $form['raven']['client'] = [
    '#type' => 'fieldset',
    '#title' => t('Browser'),
    '#collapsible' => TRUE,
  ];

  $form['raven']['client']['raven_public_dsn'] = [
    '#type' => 'textfield',
    '#title' => t('Sentry DSN'),
    '#description' => t('Sentry DSN credentials for current site. This setting can be overridden with the SENTRY_DSN environment variable.'),
    '#default_value' => variable_get('raven_public_dsn', ''),
  ];

  $form['raven']['client']['raven_js_enabled'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable Sentry JavaScript handler'),
    '#description' => t('Check to send client-side JavaScript exceptions to Sentry (if user has the <a target="_blank" href="@url"><em>send JavaScript errors to Sentry</em> permission</a>).', [
      '@url' => url('admin/people/permissions', ['fragment' => 'module-raven']),
    ]),
    '#default_value' => variable_get('raven_js_enabled', FALSE),
  ];

  $form['raven']['client']['raven_js_traces_sample_rate'] = [
    '#type' => 'textfield',
    '#title' => t('Browser performance tracing sample rate'),
    '#description' => t('Set the browser performance tracing sample rate, or set to 0 to disable performance tracing.'),
    '#default_value' => variable_get('raven_js_traces_sample_rate', NULL),
    '#element_validate' => ['element_validate_number'],
  ];

  $form['raven']['client']['raven_auto_session_tracking'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable auto session tracking'),
    '#description' => t('Check to monitor release health by sending a session event to Sentry for each page load; only active if a release is specified below or via the SENTRY_RELEASE environment variable.'),
    '#default_value' => variable_get('raven_auto_session_tracking', FALSE),
  ];

  $form['raven']['client']['raven_send_client_reports'] = [
    '#type' => 'checkbox',
    '#title' => t('Send client reports'),
    '#description' => t('Send client report (e.g. number of discarded events), if any, when tab is hidden or closed.'),
    '#default_value' => variable_get('raven_send_client_reports', FALSE),
  ];

  $form['raven']['client']['raven_set_report_uri'] = [
    '#type' => 'checkbox',
    '#title' => t('Send security header reports to Sentry'),
    '#description' => t('Check this and install <a href="@url">Security Kit module</a> to send CSP and CT reports to Sentry.', ['@url' => module_exists('seckit') ? url('admin/config/system/seckit') : 'https://www.drupal.org/project/seckit']),
    '#default_value' => variable_get('raven_set_report_uri', FALSE),
    '#disabled' => !module_exists('seckit'),
  ];

  $form['raven']['raven_enabled'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable Sentry PHP handler'),
    '#description' => t('Check to send events in server-side PHP code to Sentry.'),
    '#default_value' => variable_get('raven_enabled', FALSE),
  ];

  $form['raven']['php'] = [
    '#type' => 'fieldset',
    '#title' => t('PHP'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('raven_enabled', FALSE),
    '#states' => [
      'collapsed' => [
        ':input[name="raven_enabled"]' => ['unchecked' => TRUE],
      ],
    ],
  ];

  $form['raven']['php']['raven_dsn'] = [
    '#type' => 'textfield',
    '#title' => t('Sentry DSN'),
    '#description' => t('Sentry DSN credentials for current site. This setting can be overridden with the SENTRY_DSN environment variable.'),
    '#default_value' => variable_get('raven_dsn', ''),
  ];

  $form['raven']['php']['raven_fatal_error_handler'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable fatal error handler'),
    '#description' => t('Check to handle fatal PHP errors.'),
    '#default_value' => variable_get('raven_fatal_error_handler', TRUE),
  ];

  foreach (watchdog_severity_levels() as $level => $label) {
    $watchdog_levels[$level + 1] = $label;
  }
  $form['raven']['php']['raven_watchdog_levels'] = [
    '#type' => 'checkboxes',
    '#title' => t('Message levels'),
    '#description' => t('Check message levels to handle.'),
    '#default_value' => variable_get('raven_watchdog_levels', []),
    '#options' => $watchdog_levels,
  ];

  $form['raven']['php']['raven_ignored_types'] = [
    '#type' => 'textarea',
    '#title' => t('Ignored types'),
    '#description' => t('A list of log event types for which messages should not be sent to Sentry (one type per line). Commonly-configured types include <em>access denied</em> for 403 errors and <em>page not found</em> for 404 errors.'),
    '#default_value' => variable_get('raven_ignored_types', ''),
  ];

  $form['raven']['php']['raven_stack'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable stacktraces'),
    '#description' => t('Enable it to add stacktraces to reports.'),
    '#default_value' => variable_get('raven_stack', TRUE),
  ];

  $form['raven']['php']['raven_trace'] = [
    '#type' => 'checkbox',
    '#title' => t('Reflection tracing in stacktraces'),
    '#description' => t('Check to enable reflection tracing (function calling arguments) in stacktraces. Warning: This setting allows sensitive data to be logged by Sentry! To enable for exception stacktraces, PHP configuration flag zend.exception_ignore_args must be disabled.'),
    '#default_value' => variable_get('raven_trace', FALSE),
  ];

  $form['raven']['php']['raven_send_user_data'] = [
    '#type' => 'checkbox',
    '#title' => t('Send user data to Sentry'),
    '#description' => t('Check to send user email and username to Sentry with each event. Warning: User data can still be sent to Sentry even when this setting is disabled, for example as part of a log message or request body. Custom code is required to scrub personally-identifying information from events before they are sent.'),
    '#default_value' => variable_get('raven_send_user_data', FALSE),
  ];

  $form['raven']['php']['raven_send_request_body'] = [
    '#type' => 'checkbox',
    '#title' => t('Send request body to Sentry'),
    '#description' => t('Check to send the request body (POST data) to Sentry. Warning: This setting allows sensitive data to be logged by Sentry!'),
    '#default_value' => variable_get('raven_send_request_body', FALSE),
  ];

  $form['raven']['php']['raven_rate_limit'] = [
    '#type' => 'textfield',
    '#title' => t('Rate limit'),
    '#description' => t('Maximum log events sent to Sentry per-request or per-execution. To disable the limit, set to zero. You may need to set a limit if you have buggy code which generates a large number of log messages.'),
    '#default_value' => variable_get('raven_rate_limit', 0),
    '#element_validate' => ['element_validate_integer'],
  ];

  $form['raven']['raven_environment'] = [
    '#type' => 'textfield',
    '#title' => t('Environment'),
    '#description' => t('The environment in which this site is running. This setting can be overridden with the SENTRY_ENVIRONMENT environment variable.'),
    '#default_value' => variable_get('raven_environment', ''),
  ];

  $form['raven']['raven_release'] = [
    '#type' => 'textfield',
    '#title' => t('Release'),
    '#description' => t('The release this site is running (could be a version or commit hash). This setting can be overridden with the SENTRY_RELEASE environment variable.'),
    '#default_value' => variable_get('raven_release', ''),
  ];
}
