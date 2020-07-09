SELECT
	a. "name",
	collab_count
FROM
	author a,
	( SELECT DISTINCT
			auid,
			count(DISTINCT ad.author_id) AS collab_count
		FROM (
			SELECT
				a.author_id AS auid,
				ad.pub_id AS puid
			FROM
				author a,
				authored ad
			WHERE
				a.author_id = ad.author_id) AS auth, authored ad
		WHERE
			puid = ad.pub_id
			AND auid <> ad.author_id
		GROUP BY
			auid) AS colloborator
WHERE
	colloborator.auid = a.author_id
ORDER BY
	collab_count DESC, a. "name"
LIMIT 1000