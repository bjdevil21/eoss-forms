<?php

/**
 * Gets the webform component entry provided a component uuid.
 *
 * @param string $uuid
 * @param bool $reset
 * @return array
 *   An array of submissions stubs (having only the nid and cid attributes)
 *   keyed by UUID. An empty array is returned if no matching submission is
 *   found.
 */
function eoss_webforms_followup_uuid_entry($machine_name, $reset = FALSE) {

  // Get the submissions from static cache.
  $components = &drupal_static(__FUNCTION__);

  // See if we need to reset or if it is statically cached.
  if ($reset || empty($components[$machine_name])) {

    // Get the nid and sid for this uuid.
    $result = db_select('webform_component', 'wc')
      ->fields('s', array('cid', 'nid'))
      ->condition('wc.machine_name', $machine_name)
      ->execute();

    // Make sure we have a result.
    if ($row = $result->fetchObject()) {
      // Add the single result to the static cache.
      $components[$machine_name] = $row;
    }
  }

  // Return the component.
  return !empty($components[$machine_name]) ? $components[$machine_name] : array();
}

/**
 * Loads a webform based on the unique machine_name of the CID.
 * @param machine_name - nid-cid formatted, unique ID generated by Webform Features
 * (webform_features)
 */
function eoss_webforms_followup_uuid_webform($machine_name, $reset = FALSE) {
  $webform = null;

  // Get the submission entry.
  if ($components = eoss_webforms_followup_uuid_entry($machine_name, $reset)) {

    // Translate this to a webform node.
    $webform = node_load($components->nid, NULL, $reset);
  }

  // Return the webform node.
  return $webform;
}
