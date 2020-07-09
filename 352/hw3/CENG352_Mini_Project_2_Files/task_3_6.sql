SELECT
	t. "year",
	a. "name",
	t.counter AS count
FROM (
	SELECT
		by_year. "year",
		max(by_year.counter) AS maxer
	FROM (
		SELECT
			pd. "year",
			ad.author_id,
			count(*) AS counter
		FROM
			authored ad,
			publication pd
		WHERE
			ad.pub_id = pd.pub_id
			AND pd. "year" >= 1940
			AND pd. "year" <= 1990
		GROUP BY
			pd. "year",
			ad.author_id) by_year
	GROUP BY
		by_year. "year") m
	JOIN (
		SELECT
			pd. "year",
			ad.author_id,
			count(*) AS counter
		FROM
			authored ad,
			publication pd
		WHERE
			ad.pub_id = pd.pub_id
			AND pd. "year" >= 1940
			AND pd. "year" <= 1990
		GROUP BY
			pd. "year",
			ad.author_id) t ON m. "year" = t. "year"
	AND m.maxer = t.counter,
	author a
WHERE
	a.author_id = t.author_id;