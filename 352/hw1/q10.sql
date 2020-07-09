SELECT
	b.airline_name,
	count(*) AS flight_count
FROM
	flight_reports fl,
	airline_codes b
WHERE
	fl.airline_code = b.airline_code
	AND fl.dest_city_name LIKE '%TX'
	AND fl.plane_tail_number IS NOT NULL
	AND NOT EXISTS (
		SELECT
			*
		FROM
			flight_reports f
		WHERE
			fl.plane_tail_number = f.plane_tail_number
			AND f.dest_city_name NOT LIKE '%TX')
	GROUP BY
		b.airline_name
	ORDER BY
		b.airline_name