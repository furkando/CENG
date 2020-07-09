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
	AND ar.journal = 'IEEE Trans. Wireless Communications'
	AND au.author_id NOT in(
		SELECT
			aud.author_id FROM article ar, authored aud
		WHERE
			ar.pub_id = aud.pub_id
			AND ar.journal = 'IEEE Wireless Commun. Letters')
GROUP BY
	au.author_id, au. "name"
HAVING
	count(*) >= 10
ORDER BY
	count(*)
	DESC,
	au. "name"