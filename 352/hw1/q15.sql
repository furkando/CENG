SELECT
	b.airport_desc
FROM
	airport_ids b,
	(
		SELECT
			f1.dest_airport_id,
			count(*) AS dest
		FROM
			flight_reports f1
		GROUP BY
			f1.dest_airport_id) X,
	(
		SELECT
			f1.origin_airport_id,
			count(*) AS origi
		FROM
			flight_reports f1
		GROUP BY
			f1.origin_airport_id) Y
WHERE
	x.dest_airport_id = y.origin_airport_id
	AND x.dest_airport_id = b.airport_id
ORDER BY
	dest + origi DESC,
	b.airport_desc
LIMIT 5