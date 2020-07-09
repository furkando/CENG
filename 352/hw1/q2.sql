SELECT
	a.airport_code,
	a.airport_desc,
	COUNT(f.cancellation_reason)
FROM
	flight_reports f,
	airport_codes a
WHERE
	f.origin_airport_code = a.airport_code
	AND f.cancellation_reason = 'D'
GROUP BY
	a.airport_code
ORDER BY
	COUNT(f.cancellation_reason)
	DESC,
	a.airport_code