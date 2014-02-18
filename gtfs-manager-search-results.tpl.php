<?php
/**
 * @file search results page template
 */
?>
<section class="main">
	<div class="container">
		<aside class="left-col">
			<div class="trip-holder">
				<!--<p><small><em>Fare prices are subject to chage.</em></small></p>
   <a href="#" class="change-date prev">Previous day</a> |
  <a href="#" class="change-date next">Next day</a>-->
				<div class="trip-title">
					<ul class="trip-text">
						<li><h5>Your Departing Trip</h5></li>
						<li><h1>
								<i class="icon-map-marker"></i>
								<?php print $from; ?>
								to
								<?php print $to; ?>
							</h1></li>
					</ul>
					<ul class="trip-date">
						<li><i class="icon-calendar-empty"></i> <?php print $date; ?></li>

					</ul>
				</div>
				<div class="table-holder">
					<table class="trip-table departing">
						<thead>
							<tr>
								<th class="center"><i class="icon-ok"></i></th>
								<th>Depart</th>
								<th>Arrive</th>
								<th>Duration</th>
								<th>Cost*</th>
								<th class="center">Transfers</th>
								<th>Features</th>
								<th>Type</th>
								<th>Transporter</th>
								<th class="center">Info</th>
							</tr>
						</thead>
						<tbody>
							<?php $i = 1; ?>
							<?php foreach ($results as $result) { ?>
							<?php $fromaddr = str_replace($from . '- ', '', str_replace($from . ' - ', '', $result->departure_description));
	      	$toaddr = str_replace($to . '- ', '', str_replace($to . ' - ', '', $result->destination_description)); ?>
							<tr>
								<td class="center"><input type="radio" name="departureSel" value="depart-<?php print $i?>"></td>
								<td><?php print $result->departure_departure_time; ?></td>
								<td><?php print $result->destination_arrival_time; ?></td>
								<td><?php print $result->duration . $result->days; ?></td>
								<td><?php print $result->price; ?></td>
								<td class="center"><?php print $result->transfer;?></td>
								<td><?php print $result->features;?></td>
								<td><?php print $result->transportation_type; ?></td>
								<td><?php print $result->departure_agency_name; ?></td>
								<td class="center"><a
									data-expander="#trip-<?php print $i; ?>-details" href="#"><i
										class="icon-chevron-down"></i> </a>
								</td>
							</tr>
							<tr id="trip-<?php print $i; ?>-details" class="expandable">
								<td colspan="9">
									<ul class="trip-breakdown-title departure">
										<li><h1>
												<span>A</span>
											</h1>
										
										<li>
											<h1>
												<?php print $from; ?>
											</h1>
										</li>
									</ul>
									<ul class="trip-breakdown">
										<li>
											<ul>
												<li class="time"><h2>
														<?php print $result->departure_arrival_time; ?>
													</h2></li>
												<li class="location"><h6>
														Depart from
														<?php print $from; ?>
													</h6>
													<p>
														<?php print $fromaddr; ?>
												
												</li>
												<li class="cost"><h6>
														<?php print $result->price; ?>
													</h6></li>
												<li class="transporter"><a target="_blank"
													href="<?php print $result->departure_agency_url; ?>"><?php print $result->departure_agency_name; ?>
														<i class="icon-external-link"></i> </a>
													<p>
														<?php print $result->agency_phone; ?>
												
												</li>
											</ul>
										</li>
										<li>
											<ul>
												<li class="time"><h2>
														<?php print $result->destination_arrival_time; ?>
													</h2></li>
												<li class="location"><h6>
														Arrive at
														<?php print $to; ?>
													</h6>
													<p>
														<?php print $toaddr; ?>
												
												</li>
											</ul>
										</li>
									</ul>
									<ul class="trip-breakdown-title arrival">
										<li><h1>
												<span>B</span>
											</h1></li>
										<li>
											<h1>
												<?php print $to; ?>
											</h1>
										</li>
									</ul>
								</td>
							</tr>
							<?php $i++; ?>
							<?php } ?>
						</tbody>
					</table>
				</div>

				<?php if (!empty($return_results)) { ?>
				<h1>Return Trip</h1>
				<h2>
					<?php print $to; ?>
					to
					<?php print $from; ?>
					-
					<?php print $return_date; ?>
				</h2>
				<a href="#" class="change-date prev return">Previous day</a> | <a
					href="#" class="change-date next return">Next day</a>
				<table>
					<thead>
						<tr>
							<th class="center"><i class="icon-ok"></i></th>
							<th>Depart</th>
							<th>Arrive</th>
							<th>Duration</th>
							<th>Cost*</th>
							<th class="center">Transfers</th>
							<th>Features</th>
							<th>Type</th>
							<th>Transporter</th>
							<th class="center">Info</th>
						</tr>
					</thead>
					<tbody>
						<?php $i = 1; ?>
						<?php foreach ($return_results as $result) { ?>
						<tr>
							<td><input type="radio" name="returnSel" value="return-<?php print $i?>"></td>
							<td><?php print $result->departure_departure_time; ?></td>
							<td><?php print $result->destination_arrival_time; ?></td>
							<td><?php print $result->duration; ?></td>
							<td><?php print $result->price; ?></td>
							<td><?php print $result->transfer;?></td>
							<td><?php print $result->features;?></td>
							<td><?php print $result->transportation_type; ?></td>
							<td><?php print $result->provider; ?></td>
							<td class="center"><a
								data-expander="#trip-<?php print $i; ?>-details" href="#"><i
									class="icon-chevron-down"></i></span>
							
							</td>
						</tr>
						<tr id="trip-<?php print $i; ?>-details" class="expandable">
							<td colspan="9"><?php print $result->more_info; ?></td>
						</tr>
						<?php $i++; ?>
						<?php } ?>
					</tbody>
				</table>

				<?php } ?>
			</div>
		</aside>
		<aside class="right-col">
			<section class="block legend">
				<h3>Feature Legend</h3>
				<ul class="legend-list">
					<li><span><img alt="Bike Friendly"
							src="/sites/all/themes/atlantica/img/features/feature-bike-w.png">
					</span> Ability to carry your bike</li>
					<li><span><img alt="Accessible"
							src="/sites/all/themes/atlantica/img/features/feature-accessible-w.png"></i>
					</span> Accessible</li>
					<li><span><img alt="WiFi"
							src="/sites/all/themes/atlantica/img/features/feature-wifi-w.png">
					</span> Free onboard Wi-Fi</li>
				</ul>
			</section>
			<section class="block cost-outline">
				<h3>Total Cost Outline</h3>
				<ul class="total-cost departure">
					<li>
						<h2>
							Depart <strong>Oct. 21, 2013</strong>
						</h2>
						<h1>Halifax (NS)</h1>
					</li>
					<li>
						<h4>$24.10</h4>
						<h3>9:00am</h3>
					</li>
				</ul>
				<ul class="total-cost return">
					<li>
						<h2>
							Return <strong>Oct. 23, 2013</strong>
						</h2>
						<h1>Truro (NS)</h1>
					</li>
					<li>
						<h4>$24.10</h4>
						<h3>10:30am</h3>
					</li>
				</ul>
				<ul class="total-cost final-cost">
					<li>
						<p>
							<small>HST included.</small>
						</p>
					</li>
					<li>
						<h1>$48.20*</h1>
					</li>
				</ul>
				<p class="disclaimer">
					<small>* Prices are subject to change without notice.</small>
				</p>
				<div id="book-instructions">
					<h6>Booking Instructions</h6>
					<ol>
						<li>Call <a href="#">Maritime Bus</a> at 1-800-465-4655 at least
							48 hours prior to your trip.
						</li>
						<li>Reserve your return trip with <a href="#">VIA Rail</a> with
							their online booking system.
						</li>
						<li>Enjoy the ride!</li>
					</ol>
				</div>
				<a href="#" class="btn book-now" id="instruction-expand"><i
					class="icon-ok"></i> Book Your Trip</a>
			</section>
		</aside>
	</div>
</section>
