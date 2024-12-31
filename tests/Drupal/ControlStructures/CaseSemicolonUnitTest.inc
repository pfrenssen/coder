<?php

switch ($key) {
  case 'field_name':
    $checked_value = $field_storage->getName();
    break;

  case 'field_id':
  case 'field_storage_uuid':
    $checked_value = $field_storage->uuid();
    break;

  case 'uuid';
    $checked_value = $field->uuid();
    break;

  case 'deleted';
    $checked_value = $field->isDeleted();
    break;

  default:
    $checked_value = $field->get($key);
    break;
}
