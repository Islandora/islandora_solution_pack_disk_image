<?php

/**
 * @file
 * Handles the uploading of the disk image files.
 */

/**
 * Defines a file upload form for uploading the disk image files.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return string
 *   The drupal form definition.
 */
function islandora_disk_image_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('disk_image');
  $form['disk_image_file'] = array(
    '#title' => t('Disk Image'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select disk image to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['disk_image_file']) ? $form_state['values']['disk_image_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      // Assume its specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['upload_disk_image_image'] = array(
    '#title' => t('Upload a disk_image_image?'),
    '#type' => 'checkbox',
  );
  $form['disk_image_image'] = array(
    '#title' => t('Image of Disk Image'),
    '#type' => 'managed_file',
    '#description' => t('Select disk_image_image to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>DISK_IMAGE_IMAGE.</b>', array('@size' => $upload_size)),
    '#default_value' => isset($form_state['values']['disk_image_image']) ? $form_state['values']['disk_image_image'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('png'),
      // Assume its specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="upload_disk_image_image"]' => array('checked' => TRUE),
      ),
    ),
  );
  return $form;
}

/**
 * Validation for the islandora_disk_image upload form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_disk_image_upload_form_validate(array $form, array &$form_state) {
  if ($form_state['values']['upload_disk_image_image']) {
    if ($form_state['values']['disk_image_image'] == 0) {
      form_error($form['disk_image_image'], t('No disk_image_image uploaded!'));
    }
  }
}

/**
 * Submit handler, adds uploaded file to ingest object.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_disk_image_upload_form_submit(array $form, array &$form_state) {
  $objects = &islandora_ingest_form_get_objects($form_state);
  foreach ($objects as $object) {
    if (empty($object['OBJ'])) {
      $disk_image_ds = $object->constructDatastream('OBJ', 'M');
      $object->ingestDatastream($disk_image_ds);
    }
    else {
      $disk_image_ds = $object['OBJ'];
    }
    $disk_image_file = file_load($form_state['values']['disk_image_file']);
    $disk_image_ds->setContentFromFile($disk_image_file->uri, FALSE);
    $disk_image_ds->label = $disk_image_file->filename;
    $disk_image_ds->mimetype = $disk_image_file->filemime;
  }
  // See if there's an attached DISK_IMAGE_IMAGE to process.
  if ($form_state['values']['upload_disk_image_image']) {
    if (empty($object['DISK_IMAGE_IMAGE'])) {
      $disk_image_image_ds = $object->constructDatastream('DISK_IMAGE_IMAGE', 'M');
      $object->ingestDatastream($disk_image_image_ds);
    }
    else {
      $disk_image_image_ds = $object['DISK_IMAGE_IMAGE'];
    }
    $disk_image_image_file = file_load($form_state['values']['disk_image_image']);
    $disk_image_image_ds->setContentFromFile($disk_image_image_file->uri, FALSE);
    $disk_image_image_ds->label = $disk_image_image_file->filename;
    $disk_image_image_ds->mimetype = $disk_image_image_file->filemime;
  }
}
