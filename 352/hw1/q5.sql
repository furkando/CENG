SELECT
	concat(cast(f. "day" AS VARCHAR(100)), '/', CAST(f. "month" AS VARCHAR(100)), '/', cast(f. "year" AS VARCHAR(100))) AS flight_date,
	f.plane_tail_number,
	f.arrival_time,
	b.departure_time,
	f.origin_city_name,
	f.dest_city_name AS stop_city_name,
	b.dest_city_name,
	f.flight_time + f.taxi_out_time + b.taxi_in_time + b.flight_time AS total_time,
	f.flight_distance + b.flight_distance AS total_distance
FROM
	flight_reports f,
	(
		SELECT
			*
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NULL
			AND f.dest_city_name = 'Boston, MA') b
WHERE
	f.cancellation_reason IS NULL
	AND f.origin_city_name = 'Seattle, WA'
	AND f.dest_city_name = b.origin_city_name
	AND f.plane_tail_number = b.plane_tail_number
	AND f. "day" = b. "day"
	AND f. "month" = b. "month"
	AND f. "year" = b. "year"
	AND f. weekday_id = b. weekday_id
	AND f.arrival_time < b.departure_time
ORDER BY
	total_time, total_distance, plane_tail_number, stop_city_name