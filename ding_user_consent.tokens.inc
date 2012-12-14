<?php

/**
 * @file
 * Ding user pages.
 */

function ding_user_consent_token_info() {
  $type = array(
    'name' => t('Ding user consent'), 
    'description' => t('Tokens related to user consent.'), 
  );

  // Core tokens for nodes.
  $consent['consent'] = array(
    'name' => t("User consent status"), 
    'description' => t("The states of the current users consent settings."),
  );

  return array(
    'types' => array('ding_user_consent' => $type), 
    'tokens' => array('ding_user_consent' => $consent),
  );
}

function ding_user_consent_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'ding_user_consent') {

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the node.
        case 'accept':
          $form_state = array('hide_description' => TRUE);
          $form = drupal_build_form('ding_user_consent_quick_accept', $form_state);
          $consent_form = drupal_render($form);
          
          $replacements[$original] = $consent_form;
          break;
        case 'consent':
          $form_state = array();
          $form = drupal_build_form('ding_user_consent_quick_accept', $form_state);
          $consent_form = drupal_render($form);
          
          $replacements[$original] = $consent_form;
          break;
      }
    }
  }

  return $replacements;
}