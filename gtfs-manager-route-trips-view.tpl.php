<?php
/**
 * @file template for route stops
 *
 *
 */
?>
 <?php print implode(' | ', $variables['links']); ?>
<h3>Route: <?php print $variables['route']->route_long_name; ?></h3>
<?php if (!empty($variables['trips'])) { ?>
<table class="route-trips-view">
  <thead>
    <tr>
      <th>Calendar Begins</th>
      <th>Calendar Ends</th>
      <th>Days</th>
      <th>Options</th>
    </tr>
  </thead>
  <tbody>
<?php foreach ($variables['trips'] as $trip) { ?>
  <tr class="trip-row <?php print $trip->tid; ?>">
     <td><?php print $trip->start_date; ?></td>
     <td><?php print $trip->end_date; ?></td>
     <td><?php print $trip->active_days; ?></td>
     <td><?php print implode('<br />', $trip->links); ?></td>
  </tr>
<?php } ?>
<?php } else { ?>
  <p>No trips have been added yet.</p>
<?php } ?>
 </tbody>
</table>
