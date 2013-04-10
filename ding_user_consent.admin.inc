<?php

/**
 * @file
 * Ding user pages.
 */

/**
 * Form builder, user authentication.
 *
 * This copies the regular user login form on purpose. By making it
 * look like the regular login form, modules wanting to make changes
 * can just form_alter them both.
 *
 * @ingroup forms
 */
function ding_user_consent_admin_form($form, &$form_state) {
  $consents = ding_provider_invoke('user_consent', 'info');
  foreach ($consents as $id => $values) {
    $form[$id] = array(
      '#type' => 'fieldset', 
      '#title' => t('Consent'),
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE,
    );
    
    // Consent description link
    $form[$id][$id . '_description_link'] = array(
      '#type' => 'textfield', 
      '#title' => t('Link to consent description'),
      '#description' => t('Write a link to the page containing a description of user consent. Use wither the format node/$nid or a path alias.'), 
      '#default_value' => variable_get($id . '_description_link', ''),
    );
    // First time message
    $form[$id]['first_time'] = array(
      '#type' => 'fieldset', 
      '#title' => t('First time'),
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE,
    );
    $form[$id]['first_time'][$id . '_title_first_time'] = array(
      '#type' => 'textfield',
      '#title' => t('First login message title'),
      '#description' => t('Write a title for the message which a first time user will receive'),
      '#default_value' => variable_get($id . '_title_first_time', ''),
    );
    $description = variable_get($id . '_description_first_time', '');
    $description = (is_array($description)) ? $description['value'] : $description;
    $form[$id]['first_time'][$id . '_description_first_time'] = array(
      '#type' => 'text_format',
      '#title' => t('First login message description'),
      '#description' => t('Write a message for a first time user'),
      '#default_value' => $description,
      '#format' => 'ding_wysiwyg',
    );
    
    // User consent request message
    $form[$id]['request'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Request'),
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE,
    );
    $form[$id]['request'][$id . '_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Consent title'),
      '#description' => t('Write a title for the message which the user will see when requested to give consent'),
      '#default_value' => variable_get($id . '_title', $values['title']),
    );
    $description = variable_get($id . '_description', $values['description']);
    $description = (is_array($description)) ? $description['value'] : $description;
    $form[$id]['request'][$id . '_description'] = array(
      '#type' => 'text_format',
      '#title' => t('Consent description'),
      '#description' => t('Write a message for the user when requested to give consent'),
      '#default_value' => $description,
      '#format' => 'ding_wysiwyg',
    );
    // Token message
    $form[$id]['token'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Token'),
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE,
    );
    $description = variable_get($id . '_description_token', $values['description']);
    $description = (is_array($description)) ? $description['value'] : $description;
    $form[$id]['token'][$id . '_description_token'] = array(
      '#type' => 'text_format',
      '#title' => t('Consent description'),
      '#description' => t('Write a message to display in the form which is inserted into a text area'),
      '#default_value' => $description,
      '#format' => 'ding_wysiwyg',
    );
    $description = variable_get($id . '_description_token_given', $values['description']);
    $description = (is_array($description)) ? $description['value'] : $description;
    $form[$id]['token'][$id . '_description_token_given'] = array(
      '#type' => 'text_format',
      '#title' => t('Consent description'),
      '#description' => t('Write a message to display in the form which is inserted into a text area but the user has already given consent'),
      '#default_value' => $description,
      '#format' => 'ding_wysiwyg',
    );
  }

  return system_settings_form($form);
}
