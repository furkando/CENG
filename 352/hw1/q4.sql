SELECT
	a.airline_name
FROM
	airline_codes a,
	(
		SELECT
			x.airline_code
		FROM (
			SELECT
				fl.dest_city_name,
				fl.airline_code
			FROM
				flight_reports fl
			WHERE
				fl.dest_city_name IN('Boston, MA', 'New York, NY', 'Portland, ME', 'Washington, DC', 'Philadelphia, PA')
				AND fl. "year" >= '2018'
				AND fl.year <= '2019'
				AND fl.cancellation_reason IS NULL
			GROUP BY
				fl.airline_code,
				fl.dest_city_name) X
		GROUP BY
			x.airline_code
		HAVING
			count(*) = 5) xi
WHERE
	a.airline_code = xi.airline_code
	AND xi.airline_code NOT in(
		SELECT
			x.airline_code FROM (
				SELECT
					fl.dest_city_name, fl.airline_code FROM flight_reports fl
				WHERE
					fl.dest_city_name IN('Boston, MA', 'New York, NY', 'Portland, ME', 'Washington, DC', 'Philadelphia, PA')
					AND fl. "year" < '2018'
					AND fl.cancellation_reason IS NULL
				GROUP BY
					fl.airline_code, fl.dest_city_name) X
			GROUP BY
				x.airline_code
			HAVING
				count(*) = 5)
ORDER BY
	a.airline_name