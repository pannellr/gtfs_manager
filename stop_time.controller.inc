<?php

class StopTimeController extends DrupalDefaultEntityController {

  /**
   * create empty StopTime object
   *
   * @param $sid the stop id
   * @param $tid the trip id
   *
   * @return the empty Route object
   */
  public function create($sid = '', $tid = '') {
    return (object) array(
      'sid' => $sid,
      'tid' => $tid,
      'arrival_time' => '',
      'departure_time' => '',
      'stop_sequence' => '',
      'shape_dist_traveled' => '',
      'created_at' => '',
      'updated_at' => '',
      'created_by' => '',
      'updated_by' => '',
    );
  }

  public function save($stop_time) {
    $transaction = db_transaction();
    $stop_time->is_new = TRUE;

    try {
        global $user;
        //determine if stop time is new
        //if (!empty($stop_time->sid) || !empty($stop_time->tid)) {
        //  $stop_time->is_new = FALSE;
        //} 

      //set timestamps
      if ($stop_time->is_new) {
        $stop_time->created_at = REQUEST_TIME;
      }

      $stop_time->updated_at = REQUEST_TIME;


      // if this is a new stop time
      if ($stop_time->is_new) {
	     // Save the route

        drupal_write_record('stop_times', $stop_time);


      } else {
      	// update the stop time
      	$ids = db_update('stop_times')
      	  ->fields(array(
      	    'arrival_time' => $stop_time->arrival_time,
      	    'departure_time' => $stop_time->departure_time,
      	    'stop_sequence' => $stop_time->stop_sequence,
      	    'shape_dist_traveled' => $stop_time->shape_dist_traveled,
      	    'created_at' => $stop_time->created_at,
      	    'updated_at' => $stop_time->updated_at,
      	    'created_by' => $stop_time->created_by,
      	    'updated_by' => $stop_time->updated_by
      	  ))
      	  ->condition('tid', $stop_time->tid, '=')
      	  ->condition('sid', $stop_time->sid, '=')
      	  ->execute();
      }

      // Save fields

      // Clear internal properties
      unset($stop_time->is_new);

      // Ignore slave server temporarily to give time for insert/update
      db_ignore_slave();

      return $stop_time;

    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('stop_time', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function delete($ids) {

    $transaction = db_transaction();

    if (!empty($ids)) {
      try {
	foreach ($ids as $sid_tid) {
	  db_delete('stop_times')
	    ->condition('sid', $sid_tid['sid'], '=')
	    ->condition('tid', $sid_tid['tid'], '=')
	    ->execute();
	}

	db_ignore_slave();
      } catch (Exception $e) {
	$transaction->rollback();
	watchdog_exception('stop_time', $e, NULL, WATCHDOG_ERROR);
	return FALSE;
      }

      //Clear the page, block, and route caches
      cache_clear_all();
      $this->resetCache();
    }
    return TRUE;
  }

function getStopTimes($ids) {
    $query = 'select * from {stop_times} st where st.sid = :sid and st.tid = :tid';
    $result = db_query($query, $ids[0]);
    $stop_times = $result->fetchObject();
    return $stop_times;
  }


}