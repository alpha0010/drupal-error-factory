<?php

/**
 * @file
 * Raven module drush integration.
 */

use Sentry\Severity;

/**
 * Implements hook_drush_command().
 *
 * @see drush_parse_command()
 */
function raven_drush_command() {
  $items['raven-capture-message'] = [
    'description' => dt('Sends a test message to Sentry.'),
    'drupal dependencies' => ['raven'],
    'arguments' => [
      'message' => dt('The message text.'),
    ],
    'options' => [
      'level' => dt('The message level (debug, info, warning, error, fatal).'),
    ],
  ];
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function raven_drush_help($section) {
  switch ($section) {
    case 'drush:raven-capture-message':
      return dt('Sends a test message to Sentry.') . PHP_EOL . PHP_EOL .
        dt('Because messages are sent to Sentry asynchronously, there is no guarantee that the message was actually delivered successfully.');
  }
}

/**
 * Sends a test message to Sentry.
 */
function drush_raven_capture_message($message = 'Test message from Drush.') {
  $client = raven_get_client();
  if (!$client) {
    return drush_set_error('RAVEN_CLIENT', dt('Raven client not available.'));
  }
  $level = drush_get_option('level', 'info');
  $id = \Sentry\captureMessage($message, new Severity($level));
  if (!$id) {
    return drush_set_error('RAVEN_SEND', dt('Send failed.'));
  }
  drush_log(dt('Message sent as event !id.', ['!id' => $id]), 'success');
}
