SELECT
	a.airline_name,
	monday_flights,
	sunday_flights
FROM
	airline_codes a,
	(
		SELECT
			f.airline_code,
			f.weekday_id,
			count(*) AS monday_flights
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NULL
		GROUP BY
			f.airline_code,
			f.weekday_id
		HAVING
			f.weekday_id = 1) M,
	(
		SELECT
			f.airline_code,
			f.weekday_id,
			count(*) AS sunday_flights
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NULL
		GROUP BY
			f.airline_code,
			f.weekday_id
		HAVING
			f.weekday_id = 7) S
WHERE
	M.airline_code = a.airline_code
	AND M.airline_code = S.airline_code
ORDER BY
	airline_name