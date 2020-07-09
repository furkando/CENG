SELECT
	groupped.field_name
FROM ( SELECT DISTINCT
		together.field_name,
		together.pub_type
	FROM (
		SELECT
			*
		FROM
			field f,
			pub p
		WHERE
			p.pub_key = f.pub_key) AS together
	GROUP BY
		together.field_name,
		together.pub_type) AS groupped
GROUP BY
	groupped.field_name
HAVING
	count(*) = (
		SELECT
			max(counter)
		FROM (
			SELECT
				count(*) AS counter
			FROM ( SELECT DISTINCT
					together.field_name,
					together.pub_type
				FROM (
					SELECT
						*
					FROM
						field f,
						pub p
					WHERE
						p.pub_key = f.pub_key) AS together
				GROUP BY
					together.field_name,
					together.pub_type) AS groupped
			GROUP BY
				groupped.field_name) AS cntr)
	ORDER BY
		field_name