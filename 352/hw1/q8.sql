SELECT DISTINCT
	f1.plane_tail_number,
	f1.airline_code,
	f2.airline_code
FROM
	flight_reports f1,
	flight_reports f2
WHERE
	f1.plane_tail_number = f2.plane_tail_number
	AND f1.airline_code <> f2.airline_code
	and(f1. "year" < f2. "year"
		or(f1. "year" = f2. "year"
			AND f1. "month" < f2. "month")
		or(f1. "year" = f2. "year"
			AND f1. "month" = f2. "month"
			AND f1. "day" < f2. "day"))
ORDER BY
	f1.plane_tail_number