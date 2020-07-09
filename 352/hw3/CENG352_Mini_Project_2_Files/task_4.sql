CREATE TABLE activeauthors (
	name text UNIQUE
);

INSERT INTO activeauthors (name)
SELECT DISTINCT
	a. "name"
FROM
	author a,
	authored ad,
	publication p
WHERE
	a.author_id = ad.author_id
	AND p.pub_id = ad.pub_id
	AND p. "year" >= 2018;

CREATE OR REPLACE FUNCTION public.add_active_author ()
	RETURNS TRIGGER
	LANGUAGE plpgsql
	AS $function$
BEGIN
	IF EXISTS (
		SELECT
			1
		FROM
			publication p
		WHERE
			p.pub_id = NEW.pub_id
			AND p. "year" >= 2018)
		AND NOT EXISTS (
			SELECT
				1
			FROM
				activeauthors aa,
				author a
			WHERE
				a.author_id = NEW.author_id
				AND aa. "name" = a. "name") THEN
			INSERT INTO activeauthors (name)
		SELECT
			a.name
		FROM
			author a
		WHERE
			a.author_id = NEW.author_id;
	RETURN NEW;
END IF;
	RETURN NEW;
END;
$function$;

CREATE TRIGGER active_author
	AFTER INSERT ON authored
	FOR EACH ROW
	EXECUTE PROCEDURE add_active_author ();