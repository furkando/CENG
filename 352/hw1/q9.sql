SELECT
	f.plane_tail_number,
	avg(f.flight_distance / f.flight_time) AS avgspeed
FROM
	flight_reports f
WHERE
	f.cancellation_reason IS NULL
	AND f. "year" = 2016
	AND f. "month" = 1
	AND(f.weekday_id = 6
		OR f.weekday_id = 7)
	AND NOT EXISTS (
		SELECT
			*
		FROM
			flight_reports fl
		WHERE
			f.plane_tail_number = fl.plane_tail_number
			AND fl. "year" = 2016
			AND fl. "month" = 1
			and(fl.weekday_id < 6))
	GROUP BY
		f.plane_tail_number
	ORDER BY
		avgspeed DESC