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
function ding_user_consent_form($form, &$form_state) {
  form_load_include($form_state, 'inc', 'ding_user_consent', 'ding_user_consent.pages');
  global $user;
  if (!empty($form['#user']))
    $account = $form['#user'];
  else
    $account = $user;
  
  $consents = ding_provider_invoke('user_consent', 'info');
  $consent = ding_provider_invoke('user_consent', 'get_consent', $account);
  foreach ($consents as $id => $values) {
    if (!$account->login) {
      $title = variable_get($id . '_title_first_time', $values['title']);
      $description = variable_get($id . '_description_first_time', $values['description']);
    }
    else {
      $title = variable_get($id . '_title', $values['title']);
      $description = variable_get($id . '_description', $values['description']);
    }
    $form['content'][$id] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($title),
      '#description' => $description['value'],
      '#default_value' => !empty($consent[$id]) ? $consent[$id] : 0,
    );
  }
  // This is where we diverge from the regular login form. We're not
  // specifying any special validators, but relying on the normal behaviour.

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Accept'));

  return $form;
}

/**
 * Form submit function.
 */
function ding_user_consent_form_submit($form, &$form_state) {
  global $user;
  if (!empty($form['#user'])) {
    $account = $form['#user'];
  } else {
    $account = $user;
  }
  $consents = ding_provider_invoke('user_consent', 'info');
  $consent = ding_provider_invoke('user_consent', 'get_consent', $user);
  $changed = array();
  foreach ($consents as $id => $values) {
    if ($form_state['input'][$id] <> $consent[$id]) {
      if ($form_state['input'][$id]) {
        $result = ding_provider_invoke('user_consent', 'add', $user, $id, $consent);
      }
      else {
        $result = ding_provider_invoke('user_consent', 'remove', $user, $id, $consent);
      }
      $changed[$id] = ($form_state['input'][$id]) ? TRUE : FALSE;
    }
    if ($form_state['input'][$id] == 0 && $values['required']) {
      $title = variable_get($id . '_title', $values['title']);
      $description = variable_get($id . '_description', $values['description']);
      $description = (is_array($description)) ? $description['value'] : $description;
      $commands[] = ajax_command_ding_popup('ding_user_consent', $title, $description, array('resubmit' => FALSE));
      print ajax_render($commands);
      exit;
    }
  }
  module_invoke_all('ding_user_consent_changed', $changed);
  if (empty($account->data['consent']) || !isset($account->data['consent'])) {
    $su = clone $account;
    $auth_res['user']['data'] = array('consent' => 1);
    ding_user_update_user($auth_res, $su);
  }
  $form_state['redirect'] = 'user/' . $account->uid;
}
