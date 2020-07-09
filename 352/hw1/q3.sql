SELECT
	o.plane_tail_number,
	o. "year",
	(o.a / o.b) AS daily_avg
FROM (
	SELECT
		x.plane_tail_number,
		x. "year",
		sum(x.toplam) AS a,
		count(*) AS b
	FROM (
		SELECT
			f.plane_tail_number,
			count(*) AS toplam,
			f. "year",
			f. "day",
			f. "month"
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NULL
		GROUP BY
			f. "day",
			f. "month",
			f. "year",
			f.plane_tail_number) X
	GROUP BY
		x.plane_tail_number,
		x. "year") O
WHERE
	o.a / o.b > 5
ORDER BY
	o.plane_tail_number,
	o. "year"