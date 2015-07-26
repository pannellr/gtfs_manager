<?php

function gtfs_manager_stop_time_form($form, &$form_state, $args) {

	$stop_time = $args['stop_time'];
	$stop = $args['stop'];
	$trip = $args['trip'];

	$form['trip_info']['#markup'] = '<h2>Trip: ' . t($trip->route_long_name) . '</h2>';
	$form['trip_stops']['#markup'] = active_stops_table($trip->tid);
	$form['add_stop']['#markup'] = l(t('Add stop'), 'stop/add');

  	//Set the id ot identify this as the stop edit form
  	$form['#id'] = 'stop-time-form';

  	//save the stop for later
  	$form['#stop_time'] = $stop_time;
  	$form_state['stop_time'] = $stop_time;

  	$form['gtfs_manager_tid'] = array(
    	'#type' => 'hidden',
    	'#value' => $trip->tid,
  	);

	// $form['#tree'] = TRUE;
 //  	$form['stops_fieldset'] = array(
 //    	'#type' => 'fieldset',
 //    	'#title' => t('List of stops associated with this trip')
 //  	);

    $form['gtfs_manager_sid'] = array(
	    '#type' => 'select',
	    '#title' => t('Select a stop'),
	    '#options' => fetch_stop_options(),
	    '#required' => TRUE,
	    '#default_value' => $stop_time->sid,
  	);

	//tetfield for trip distance
	$form['gtfs_manager_shape_dist_traveled'] = array(
		'#type' => 'textfield',
		'#title' => t('Distance from previous stop in kilometers'),
		'#description' => t('Up to 3 decimals of accuracy.'),
		'#size' => 24,
		'#default_value' => $stop_time->shape_dist_traveled,
	);

	//timepicker for stop arrival time
	$form['gtfs_manager_stop_arrival_time'] = array(
		'#type' => 'textfield',
		'#title' => t('Arrival Time for this stop'),
		'#description' => t('The arrival time'),
		'#attributes' => array('class' => array('form-time')),
		'#size' => 20,
		'#required' => TRUE,
		'#default_value' => $stop_time->arrival_time,
	);

  	//timepicker for stop arrival time
	$form['gtfs_manager_stop_departure_time'] = array(
		'#type' => 'textfield',
		'#title' => t('Departure Time for this stop'),
		'#description' => t('The departure time'),
		'#attributes' => array('class' => array('form-time')),
		'#size' =>20,
		'#required' => TRUE,
		'#default_value' => $stop_time->departure_time,
	);

	//tetfield for stop sequence number
	$form['gtfs_manager_stop_sequence'] = array(
		'#type' => 'textfield',
		'#title' => t('Sequence number for this stop'),
		'#title' => t('Sequence'),
		'#required' => TRUE,
		'#default_value' => $stop_time->stop_sequence,
	);

	$form['buttons'] = array();
	$form['buttons']['#wieght'] = 100;
	$form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save stop time'),
    '#submit' => array('gtfs_manager_stop_time_form_submit'),

   
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    // '#ajax' => array(
    //   'callback' => 'gtfs_manager_stop_time_form_callback',
    //   'wrapper' => 'names-fieldset-wrapper',
    // ),
  );

	   //Add a delete button if user is editting
  	if (!empty($stop_time->tid) && !empty($stop_time->sid)) {
    	$form['buttons']['delete'] = array(
      	'#type' => 'submit',
      	'#value' => t('Delete'),
      	'#weight' => 15,
      	'#submit' => array('gtfs_manager_stop_time_form_delete_submit'),
    	);
  	}
    

  return $form;

}

function gtfs_manager_stop_time_form_submit($form, &$form_state) {
	global $user;

   $stop_time = &$form_state['stop_time'];

  // if this is a new record
  if (empty($agency->created_by)) {
    $stop_time->created_by = $user->name;
  }

  $stop_time->updated_by = $user->name;
  $stop_time->sid = $form_state['values']['gtfs_manager_sid'];
  $stop_time->tid = $form_state['values']['gtfs_manager_tid'];
  $stop_time->stop_sequence = $form_state['values']['gtfs_manager_stop_sequence'];
  $stop_time->arrival_time = $form_state['values']['gtfs_manager_stop_arrival_time'];
  $stop_time->departure_time = $form_state['values']['gtfs_manager_stop_departure_time'];
  $stop_time->shape_dist_traveled = $form_state['values']['gtfs_manager_shape_dist_traveled'];

  //Save
  stop_time_save($stop_time);

  drupal_set_message(t('Stop time saved'));

  //$form_state['redirect'] = 'agencies';

}

function gtfs_manager_stop_time_form_delete_submit($form, &$form_state) {
	$destination = array();
  	if (isset($_GET['destination'])) {
    	$destinaltion = drupal_get_destination();
    	unset($_GET['destination']);
  	}

  	$stop_time = $form['#stop_time'];
  	$form_state['redirect'] = array(
  		'trip/' 
  		. $stop_time->tid 
  		. '/stop/' 
  		. $stop_time->sid
  		. '/delete', 
  		array('query' => $destination)
  	);
}

function stop_delete_confirm($form, &$form_state, $trip, $stop) {
  $form['#stop'] = $stop;
  $form['#trip'] = $trip;
  $form['sid'] = array('#type' => 'value', '#value' => $stop->sid);
  $form['tid'] = array('#type' => 'value', '#value' => $stop->tid);
  return confirm_form(
  	$form,
    t('Are you sure you want to remove the stop: %name from this trip?',
      array('%name' => $stop->stop_name)),
    'stop/' . $stop->sid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function stop_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $stop = stop_load($form_state['values']['sid']);
    stop_delete($stop);
    update_route_sequence($stop->rid, $stop->stop_sequence, 'delete');
  }

  watchdog('stop', 'Stop at @name deleted.', array('@name' => $stop->stop_name));

  drupal_set_message(t('The stop at %name has been deleted.',
    array('%name' => $stop->stop_name)
  ));

  $form_state['redirect'] = 'route/' . $stop->rid . '/stops';
}

function fetch_stop_options() {   
	$options = array();

	$stops = entity_load('stop', FALSE);

  	if ($stops) {
    	foreach ($stops as $stop) {
      		$options[$stop->sid] = t($stop->stop_name);
    	}
  	} else {
   		$options[0] = 'Please add stops';
	}
   return $options;
 }

 //builds table for form markup
 function active_stops_table($tid){

 	$thead = '<thead><tr><th>Name</th><th>Sequence</th><th>Arrives</th><th>Departs</th><th>Options</th>';

 	$args = array(':tid' => $tid);

 	$query = 'select  s.sid '
 			.  ' , t.tid '
 			.  ' , s.stop_name '
 			.  ' , st.stop_sequence '
 			.  ' , st.arrival_time '
 			.  ' , st.departure_time '
 		 	.  ' from '
      		.  ' {trips} t '
      		.  ' join {routes} r on r.rid = t.rid '
      		.  ' left join {stop_times} st on st.tid = t.tid '
      		.  ' join {stops} s on s.sid = st.sid'
      		.  ' join {calendar} c on c.service_id = t.service_id '
      		.  ' where t.tid = :tid' 
      		.  ' order by st.stop_sequence';

 	$result = db_query($query, $args);

 	if ($result->rowCount() > 0) {

	 	$rows = array();
		while ($stop = $result->fetchAssoc()){
			$sid = $stop['sid'];
			$tid = $stop['tid'];

			unset($stop['tid']);
			unset($stop['sid']);

			$stop['options'] = '<a href="/trip/' 
				. $tid 
				. '/stop_time/' 
				. $sid 
				. '/delete/">Remove</a><br />' 
				. '<a href="/trip/' 
				. $tid 
				. '/stop_time/' 
				. $sid 
				. '">Edit</a>';

			$row = '<td id="stop-sid-' . $sid . '"">' . join($stop, '</td><td>') . '</td>';
			$rows[] = $row; 
		}

		$table = '<table>'
			. $thead
			. '<tbody>'
			. '<tr>'
			. join($rows, '</tr><tr>')
			. '</tr>'
			. '</tbody>'
			. '</table>';
	} else {
		$table = '<p>No stops have been added to this trip</p>';
	}			

	return $table;
 }

 function stop_time_page_edit($trip, $stop) {
  drupal_set_title(t('<em>Edit stop time</em>'), PASS_THROUGH);
  $stop_time = stop_time_load(array('tid' => $trip->tid, 'sid' => $stop->sid));
  $args = array(
  	'stop_time' => $stop_time,
  	'trip' => $trip,
  	'stop' => $trip
  );
  return drupal_get_form('gtfs_manager_stop_time_form', $args);
}