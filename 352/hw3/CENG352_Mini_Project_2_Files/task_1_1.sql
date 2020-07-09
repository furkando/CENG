SELECT
	p.pub_type,
	count(*) AS count
FROM
	pub p
GROUP BY
	p.pub_type
ORDER BY
	count(*)
	DESC