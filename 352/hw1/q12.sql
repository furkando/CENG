SELECT
	a. "year",
	a.airline_code,
	boston_flight_count,
	(cast(boston_flight_count AS float) / cast(alll AS float)) * 100 AS boston_flight_percentage
FROM (
	SELECT
		f. "year",
		f.airline_code,
		count(*) AS alll
	FROM
		flight_reports f
	WHERE
		f.cancellation_reason IS NULL
	GROUP BY
		f. "year",
		f.airline_code) A,
	(
		SELECT
			f. "year",
			f.airline_code,
			count(*) AS boston_flight_count
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NULL
			AND f.dest_city_name = 'Boston, MA'
		GROUP BY
			f. "year",
			f.airline_code) B
WHERE
	a. "year" = b. "year"
	AND a.airline_code = b.airline_code
	and(cast(boston_flight_count AS float) / cast(alll AS float)) * 100 > 1
ORDER BY
	a. "year",
	a.airline_code