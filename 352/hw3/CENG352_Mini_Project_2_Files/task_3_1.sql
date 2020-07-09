SELECT
	a. "name" as author_name,
	pub_count
FROM (
	SELECT
		ad.author_id,
		COUNT(pub_id) AS pub_count
	FROM
		authored ad
	GROUP BY
		ad.author_id
	HAVING
		COUNT(ad.pub_id) >= 150
		AND COUNT(ad.pub_id) < 200
	ORDER BY
		COUNT(ad.pub_id)) AS pub_count,
	author a
WHERE
	a.author_id = pub_count.author_id
ORDER BY
	pub_count,
	a. "name"