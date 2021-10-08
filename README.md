To set up:

```
cd docker
docker-compose up
```

Go to http://localhost:5678/?q=admin/config/development/logging
Username and password are both `errors`. Set the Sentry DSN in the PHP block
and save.

It is configured to send everything, so logging in/out, invalid credentials,
404 pages, etc. should all send events. To get an event with a stack, the
easiest way is add `throw new Exception('test');` in `drupal/index.php` right
after `drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);`.
