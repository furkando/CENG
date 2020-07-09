-- I am creating this temporary table for task 3-4 since i am able to find every ten year in one query like (40-50,50-60) so i have wirte different queries for each year and its decades ant add it to the TEMPORARY table

CREATE TEMPORARY TABLE decades (
	decade text,
	total int
);

INSERT INTO decades (decade, total)
SELECT
	((p. "year") / 10) * 10 || '-' || ((p. "year") / 10) * 10 + 10 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940
GROUP BY
	(p. "year") / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 1) / 10) * 10 + 1 || '-' || ((p. "year" - 1) / 10) * 10 + 10 + 1 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 1
GROUP BY
	(p. "year" - 1) / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 2) / 10) * 10 + 2 || '-' || ((p. "year" - 2) / 10) * 10 + 10 + 2 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 2
GROUP BY
	(p. "year" - 2) / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 3) / 10) * 10 + 3 || '-' || ((p. "year" - 3) / 10) * 10 + 10 + 3 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 3
GROUP BY
	(p. "year" - 3) / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 4) / 10) * 10 + 4 || '-' || ((p. "year" - 4) / 10) * 10 + 10 + 4 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 4
GROUP BY
	(p. "year" - 4) / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 5) / 10) * 10 + 5 || '-' || ((p. "year" - 5) / 10) * 10 + 10 + 5 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 5
GROUP BY
	(p. "year" - 5) / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 6) / 10) * 10 + 6 || '-' || ((p. "year" - 6) / 10) * 10 + 10 + 6 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 6
GROUP BY
	(p. "year" - 6) / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 7) / 10) * 10 + 7 || '-' || ((p. "year" - 7) / 10) * 10 + 10 + 7 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 7
GROUP BY
	(p. "year" - 7) / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 8) / 10) * 10 + 8 || '-' || ((p. "year" - 8) / 10) * 10 + 10 + 8 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 8
GROUP BY
	(p. "year" - 8) / 10;

INSERT INTO decades (decade, total)
SELECT
	((p. "year" - 9) / 10) * 10 + 9 || '-' || ((p. "year" - 9) / 10) * 10 + 10 + 9 AS DECADE,
	count(*) AS TOTAL
FROM
	publication p
WHERE
	p. "year" >= 1940 + 9
GROUP BY
	(p. "year" - 9) / 10;