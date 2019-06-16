<?php

/**
 * @file
 * Implementation of Drush hooks.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_disk_image_drush_command() {
  $commands = array();

  $commands['islandora_disk_image_preprocess'] = array(
    'aliases' => array('idip'),
    'description' => 'Preprocess assets by scanning either a directory or a ZIP archive.',
    'drupal dependencies' => array(
      'islandora_disk_image',
      'islandora_batch',
    ),
    'examples' => array(
      'drush -v --user=admin --uri=https://my.cool.site islandora_disk_image_preprocess--parent=my:pid --parent_relationship_pred=isMemberOfCollection --type=directory --scan_target=/tmp/batch_ingest',
    ),
    'options' => array(
      'type' => array(
        'description' => "Either 'directory' or 'zip'. The zip importer is unstable with large files (~2GB). Consider alternative methods such as unzipping your Zip file and using the `--directory` option.",
        'required' => TRUE,
      ),
      'parent' => array(
        'description' => 'The collection to which the generated items should be added. Defaults to the root Islandora repository PID.',
        'value' => 'optional',
      ),
      'parent_relationship_uri' => array(
        'description' => 'The namespace URI of the relationship to the parent. Defaults to "info:fedora/fedora-system:def/relations-external#".',
        'value' => 'optional',
      ),
      'parent_relationship_pred' => array(
        'description' => 'The predicate of the relationship to the parent. Defaults to "isMemberOfCollection".',
        'value' => 'optional',
      ),
      'namespace' => array(
        'description' => 'Namespace of objects to create. Defaults to namespace specified in Fedora configuration.',
        'value' => 'optional',
      ),
      'zip_encoding' => array(
        'description' => 'The encoding of filenames contained in ZIP archives:Only relevant with --scan_target=zip. Defaults to the native encoding being used by PHP.',
        'value' => 'optional',
      ),
      'scan_target' => array(
        'description' => 'The target to directory or zip file to scan. Requires the full path to your archive from root directory. e.g. /var/www/drupal/sites/archive.zip',
        'required' => TRUE,
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $commands;
}

/**
 * Validates parameters for the disk image preprocess.
 */
function drush_islandora_disk_image_preprocess_validate() {
  module_load_include('inc', 'islandora_batch', 'includes/utilities');
  $encoding_check = islandora_batch_scan_check_encoding(drupal_strtolower(drush_get_option('type')), drush_get_option('zip_encoding'));
  if ($encoding_check !== NULL) {
    drush_set_error('INVALID ENCODING', $encoding_check);
  }
  $path = drush_get_option('scan_target') ? drush_get_option('scan_target') : drush_get_option('target');
  $target_checks = islandora_batch_scan_check_target(drupal_strtolower(drush_get_option('type')), $path);
  if (count($target_checks)) {
    foreach ($target_checks as $target_check) {
      drush_set_error($target_check['error'], NULL, $target_check['label']);
    }
  }
  $parent = drush_get_option('parent', variable_get('islandora_repository_pid', 'islandora:root'));
  if (!islandora_object_load($parent)) {
    drush_set_error('INVALID PARENT', NULL, dt('"!parent" does not exist or is not accessible. ', array('!parent' => $parent)));
  }
}

/**
 * Implements hook_islandora_batch_scan_preprocess().
 *
 * Builds a preprocessor, and passes it off to a preprocessor handler.
 */
function drush_islandora_disk_image_preprocess() {
  // XXX: Due to how Drush bootstrapping works, the connection may be created
  // without credentials (when your site's front page is
  // 'islandora/object/some:object', for example). Resetting to ensure a new
  // connection gets created should fix it.
  drupal_static_reset('islandora_get_tuque_connection');
  $connection = islandora_get_tuque_connection();
  $parameters = array(
    'type' => drush_get_option('type'),
    'target' => drush_get_option('scan_target'),
    'parent' => drush_get_option('parent', variable_get('islandora_repository_pid', 'islandora:root')),
    'parent_relationship_uri' => drush_get_option('parent_relationship_uri', 'info:fedora/fedora-system:def/relations-external#'),
    'parent_relationship_pred' => drush_get_option('parent_relationship_pred', 'isMemberOfCollection'),
    'namespace' => drush_get_option('namespace'),
    'zip_encoding' => drush_get_option('zip_encoding'),
    'content_models' => array('islandora:sp_disk_image'),
  );
  $preprocessor = new IslandoraDiskImageBatch($connection, $parameters);
  // Pass the preprocessor off to run.
  islandora_batch_handle_preprocessor($preprocessor);
  drush_log(t("SetId: @s", array('@s' => $preprocessor->getSetId())), "ok");
}
