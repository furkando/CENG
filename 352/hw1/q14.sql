SELECT
	Y. "year",
	w.weekday_name,
	c.reason_desc AS reason,
	Y.number_of_cancellations
FROM
	cancellation_reasons c,
	weekdays w,
	(
		SELECT
			X. "year",
			X.weekday_id,
			max(cnt) AS number_of_cancellations
		FROM (
			SELECT
				f. "year",
				f.weekday_id,
				f.cancellation_reason,
				count(*) AS cnt
			FROM
				flight_reports f
			WHERE
				f.cancellation_reason IS NOT NULL
			GROUP BY
				f. "year",
				f.weekday_id,
				f.cancellation_reason
			ORDER BY
				f. "year",
				f.weekday_id,
				count(*)
				DESC) X
		GROUP BY
			X. "year",
			X.weekday_id) Y,
	(
		SELECT
			f. "year",
			f.weekday_id,
			f.cancellation_reason,
			count(*) AS cnt
		FROM
			flight_reports f
		WHERE
			f.cancellation_reason IS NOT NULL
		GROUP BY
			f. "year",
			f.weekday_id,
			f.cancellation_reason
		ORDER BY
			f. "year",
			f.weekday_id,
			count(*)
			DESC) Z
WHERE
	Y.number_of_cancellations = z.cnt
	AND Y. "year" = Z. "year"
	AND Y.weekday_id = z.weekday_id
	AND c.reason_code = Z.cancellation_reason
	AND z.weekday_id = w.weekday_id
ORDER BY
	Y. "year",
	Y.weekday_id