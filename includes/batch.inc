<?php

/**
 * @file
 * Batch for disk image files that only changes how the binaries are processed.
 */

/**
 * {@inheritdoc}
 */
class IslandoraDiskImageBatch extends IslandoraScanBatch {

  /**
   * Get the name of the class to instantiate for the batch operations.
   */
  protected static function getObjectClass() {
    return "IslandoraDiskImageBatchObject";
  }

}

/**
 * Disk image batch object that differs minutely from the parent.
 */
class IslandoraDiskImageBatchObject extends IslandoraScanBatchObject {

  /**
   * {@inheritdoc}
   */
  public function batchProcess() {
    // XXX: This is virtually identical to the parent batchProcess beyond the
    // check for the OBJ datastream as fiwalk requires filenames to be present
    // during the derivative creation process.
    $this->label = $this->getTitle();
    $this->getMods();
    $this->getDc();

    if (!isset($this['OBJ'])) {
      $other = array_diff_key($this->objectInfo, array_flip($this->objectInfoDatastreamExclusions));
      foreach ($other as $obj) {
        list($dsid, $mimetype) = static::determineDSIDAndMimetype($obj->filename);
        $obj_datastream = $this->constructDatastream($dsid);
        $obj_datastream->mimetype = $mimetype;
        $obj_datastream->label = $dsid === 'OBJ' ? $obj->filename : "$dsid Datastream";
        $obj_datastream->setContentFromFile($obj->uri, FALSE);

        $this->ingestDatastream($obj_datastream);
      }
    }
    return ISLANDORA_BATCH_STATE__DONE;
  }

}
