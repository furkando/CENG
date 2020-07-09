SELECT
	b.airline_name,
	(CAST(c.cnt AS float) / CAST(d.cnt AS float)) * 100 AS percentage
FROM
	airline_codes b,
	(
		SELECT
			f.airline_code,
			count(*) AS cnt
		FROM
			flight_reports f
		WHERE
			f.origin_city_name = 'Boston, MA'
		GROUP BY
			f.airline_code) D,
	(
		SELECT
			f.airline_code,
			count(*) AS cnt
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NOT NULL
			AND f.origin_city_name = 'Boston, MA'
		GROUP BY
			f.airline_code) C
WHERE
	c.airline_code = b.airline_code
	AND c.airline_code = d.airline_code
	AND(CAST(c.cnt AS float) / CAST(d.cnt AS float)) * 100 > 10
ORDER BY
	percentage DESC