
SELECT
	x.weekday_id,
	w.weekday_name,
	avg(x.total_delay) AS avg_delay
FROM
	weekdays w,
	(
		SELECT
			f.weekday_id,
			f.departure_delay + f.arrival_delay AS total_delay
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NULL
			AND f.origin_city_name = 'San Francisco, CA'
			AND f.dest_city_name LIKE 'Boston, MA'
		GROUP BY
			f.weekday_id,
			total_delay
		ORDER BY
			total_delay) X
WHERE
	x.weekday_id = w.weekday_id
GROUP BY
	x.weekday_id,
	w.weekday_name
ORDER BY
	avg_delay
LIMIT 1