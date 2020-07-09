SELECT
	au. "name",
	count(*)
FROM
	author au,
	article ar,
	authored aud
WHERE
	au.author_id = aud.author_id
	AND ar.pub_id = aud.pub_id
	AND ar.journal LIKE '%IEEE%'
GROUP BY
	au.author_id,
	au. "name"
ORDER BY
	count(*)
	DESC,
	au. "name"
LIMIT 50