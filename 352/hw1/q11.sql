SELECT
	a.airline_name,
	z. "year",
	total_flight AS total_flight_number,
	cancelled_flights
FROM
	airline_codes a,
	(
		SELECT
			f. "year",
			f.airline_code,
			count(*) AS total_flight
		FROM
			flight_reports f
		GROUP BY
			f. "year",
			f.airline_code) X,
	(
		SELECT
			Y.airline_code,
			Y. "year",
			count(*) AS total_day
		FROM (
			SELECT
				f. "year",
				f. "month",
				f. "day",
				f.airline_code
			FROM
				flight_reports f
			GROUP BY
				f. "year",
				f. "month",
				f. "day",
				f.airline_code) Y
		GROUP BY
			Y.airline_code,
			Y. "year") Z,
	(
		SELECT
			f. "year",
			f.airline_code,
			count(*) AS cancelled_flights
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NOT NULL
		GROUP BY
			f. "year",
			f.airline_code) T
WHERE
	Z.airline_code = X.airline_code
	AND T.airline_code = X.airline_code
	AND T. "year" = X. "year"
	AND Z. "year" = X. "year"
	and(cast(total_flight AS float) / cast(total_day AS float)) > 2000
	AND T.airline_code <> 'OO'
	AND T.airline_code = a.airline_code
ORDER BY
	a.airline_name