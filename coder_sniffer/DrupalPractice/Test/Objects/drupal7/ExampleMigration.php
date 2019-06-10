<?php

/**
 * A class from Drupal 7 that should not throw errors.
 */
class ExampleMigration extends ExampleMigrateBase {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Import users from the CSV.');
    $columns = [
      ['uuid', t('UUID')],
      ['name', t('Username')],
      ['pass', t('User password')],
      ['email', t('User email')],
      ['role', t('User role')],
      ['site', t('Site')],
      ['first_name', t('First name')],
      ['last_name', t('Last name')],
      ['phone', t('Phone')],
      ['status', t('Status')],
    ];
  }

}
