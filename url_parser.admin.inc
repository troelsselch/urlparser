<?php

function url_parser_admin_page() {
  $html = t('<h2>Defined keys</h2>');
  $html .= _url_parser_get_rendered_table();
  
  $html .= t('<h2>Add new key</h2>');
  $form = drupal_get_form('url_parser_admin_form');
  $html .= drupal_render($form);
  
  //Display data
  $html .= t('<h2>Data for keys</h2>');
  $html .= _url_parser_get_rendered_table_data();

  return $html;
}

function _url_parser_get_rendered_table() {
  $header = array(
    array(
      'data' => t('Host'),
    ),
    array(
      'data' => t('Key'),
    ),
    array(
      'data' => t('Description'),
    ),
    array(
      'data' => t('Operations'),
    ),
  );

  $records = db_select('url_parser_keys','upk')
    ->fields('upk')
    ->execute();
  
  $rows = array();
  foreach ($records as $record) {
    $delete_link =  l(t('Delete'),'admin/config/url_parser/delete/'.$record->host.'/'.$record->kay);
    $rows[] = array(
      'data' => array(
        $record->host,
        $record->kay,
        $record->description,
        $delete_link,
      ),
    );
  }

  $variables = array(
    'header' => $header,
    'rows' => $rows,
  );

  return theme('table',$variables);
}

function _url_parser_get_rendered_table_data() {
  //Todo: Make values column contain one value per row and make key column span all values
  $header = array(
    array(
      'data' => t('Host'),
    ),
    array(
      'data' => t('Key'),
    ),
    array(
      'data' => t('Values'),
    ),
  );

  $records = db_select('url_parser_data','upd')
    ->fields('upd')
    ->execute();

  $rows = array();
  foreach ($records as $record) {
    $rows[] = array(
      'data' => array(
        $record->host,
        $record->kay,
        $record->value,
      ),
    );
  }

  $variables = array(
    'header' => $header,
    'rows' => $rows,
  );

  return theme('table',$variables);
}

function url_parser_admin_form($form, &$form_state) {
  $form = array();

  $form['url_parser_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#required' => TRUE,
    '#description' => t('The host to which the key belongs.'),
  );
  $form['url_parser_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key)'),
    '#required' => TRUE,
    '#description' => t('The key part of a possible key/value pair in the url parameter list.'),
  );
  $form['url_parser_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Key description'),
    '#required' => TRUE,
    '#description' => t('Description of the key'),
  );
  $form['url_parser_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add key definition'),
  );

  return $form;
}

function url_parser_admin_form_submit($form, &$form_state) {
  $host = $form_state['values']['url_parser_host'];
  $key = $form_state['values']['url_parser_key'];
  $description = $form_state['values']['url_parser_description'];

  try {
    $query = db_insert('url_parser_keys')->fields(array('host','kay','description'));
    $query->values(array(
      'host' => $host,
      'kay' => $key,
      'description' => $description,
    ));
    $query->execute();
    
    drupal_set_message(t('The key was added.'));
  } catch(PDOException $e) {
    if(strpos($e->getMessage(),'Error: SQLSTATE[23000]') !== FALSE) {//Integrety contrainst (record already exists)
      drupal_set_message(t('Error: %message', array('%message' => $e->getMessage())), 'error');
    } else {
      drupal_set_message(t('The key @key is already defined for the host $host',array('@key'=>$key,'@host',$host)),'error');
    }
  }
}

function url_parser_admin_page_delete($form, &$form_state, $host, $key) {
  $form['url_parser_key'] = array(
    '#type' => 'value',
    '#value' => $key,
  );
  $form['url_parser_host'] = array(
    '#type' => 'value',
    '#value' => $host,
  );
  $confirm_form = confirm_form($form,
    t('Are you sure you want to delete @key for @host?',array('@key'=>$key,'@host'=>$host)),
    'admin/config/url_parser',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
    
  return $confirm_form;
}

function url_parser_admin_page_delete_submit($form, &$form_state) {
  $key = $form_state['values']['url_parser_key'];
  $host = $form_state['values']['url_parser_host'];
  
  $num_deleted = db_delete('url_parser_keys')
    ->condition('kay',$key,'=')
    ->condition('host',$host,'=')
    ->execute();
  if($num_deleted === 0) {
    drupal_set_message(t('Key @key did not exist.',array('@key'=>$key)),'warning');
  } else {
    drupal_set_message(t('Key @key has been deleted.',array('@key'=>$key)));
  }
  
  $form_state['redirect'] = 'admin/config/url_parser';
}

function url_parser_admin_load_config() {
  $form = drupal_get_form('url_parser_admin_load_config_form');
  $output = drupal_render($form);
  return $output;
}

function url_parser_admin_load_config_form($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('Upload a file containing the values for a host.'),
  );
  $form['inifile'] = array(
    '#type' => 'file',
    '#name' => 'files[inifile]',
    '#title' => t('INI file.'),
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Parse'),
  );
  return $form;}

function url_parser_admin_load_config_form_submit($form, &$form_state) {
  $validators = array(
   'file_validate_extensions' => array('ini'),
  );

  // file_save_upload() only saves the file temporarily.
  $file = file_save_upload('inifile', $validators);
  $config = parse_ini_file($file->uri);
  
  foreach ($config['keys'] as $key => $description) {
    try {
      $query = db_insert('url_parser_keys')
        ->fields(array('host','kay','description'))
        ->values(array(
          'host' => $config['host'],
          'kay' => $key,
          'description' => $description,
        ))
        ->execute();
        drupal_set_message(t('The new configuration for @key on @host has been saved.',array('@key'=>$key,'@host'=>$config['host'])));
    } catch(PDOException $e) {
      if(strpos($e->getMessage(),'Error: SQLSTATE[23000]') !== FALSE) {//Integrety contrainst (record already exists)
      //TODO: refactor this piece of copy/paste code to match here.
        drupal_set_message(t('Error: %message', array('%message' => $e->getMessage())), 'error');
      } else {
        drupal_set_message(t('The key @key is already defined for the host @host',array('@key'=>$key,'@host'=>$config['host'])),'error');
      }
    }
  }  
}
