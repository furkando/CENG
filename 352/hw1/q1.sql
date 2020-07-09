SELECT DISTINCT
	a.airline_name,
	f.airline_code,
	AVG(f.departure_delay)
FROM
	flight_reports f,
	airline_codes a
WHERE
	f.airline_code = a.airline_code
	AND f.cancellation_reason IS NULL
GROUP BY
	a.airline_name,
	f.airline_code
ORDER BY
	AVG(f.departure_delay),
	a.airline_name