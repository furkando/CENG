INSERT INTO author (name)
SELECT DISTINCT
	field_value
FROM
	field
WHERE
	field_name = 'author';

------------------------------------------------------------------------
INSERT INTO publication (pub_key, title, "year")
SELECT DISTINCT
	f1.pub_key,
	f1.field_value,
	CAST(f2.field_value AS integer)
FROM
	field f1
	LEFT OUTER JOIN field f2 ON f1.pub_key = f2.pub_key
WHERE
	f1.field_name = 'title'
	AND f2.field_name = 'year';

------------------------------------------------------------------------
INSERT INTO article (pub_id, journal, "number", volume, "month")
SELECT DISTINCT
	pu.pub_id,
	f2.field_value,
	f3.field_value,
	f4.field_value,
	f5.field_value
FROM
	publication pu,
	pub p,
	field f1
	LEFT OUTER JOIN field f2 ON f1.pub_key = f2.pub_key
	AND f2.field_name = 'journal'
	LEFT OUTER JOIN field f3 ON f1.pub_key = f3.pub_key
	AND f3.field_name = 'number'
	LEFT OUTER JOIN field f4 ON f1.pub_key = f4.pub_key
	AND f4.field_name = 'volume'
	LEFT OUTER JOIN field f5 ON f1.pub_key = f5.pub_key
	AND f5.field_name = 'month'
WHERE
	f1.pub_key = pu.pub_key
	AND f1.pub_key = p.pub_key
	AND p.pub_type = 'article';

------------------------------------------------------------------------
INSERT INTO book (pub_id, publisher, isbn)
SELECT
	p1.pub_id,
	publisher.field_value,
	isbn.max
FROM (
	SELECT
		f.pub_key,
		f.field_value
	FROM
		field f,
		pub p
	WHERE
		f.pub_key = p.pub_key
		AND p.pub_type = 'book'
		AND f.field_name = 'publisher') AS publisher
	LEFT OUTER JOIN (
	SELECT
		f.pub_key, max(f.field_value)
	FROM
		field f,
		pub p
	WHERE
		f.pub_key = p.pub_key
		AND p.pub_type = 'book'
		AND field_name = 'isbn'
	GROUP BY
		f.pub_key) AS isbn ON publisher.pub_key = isbn.pub_key,
	publication p1
WHERE
	publisher.pub_key = p1.pub_key;

------------------------------------------------------------------------
INSERT INTO incollection (pub_id, book_title, publisher, isbn)
SELECT DISTINCT
	p.pub_id,
	f2.field_value,
	f3.field_value,
	f4.field_value
FROM
	publication p,
	field f1
	LEFT OUTER JOIN field f2 ON f1.pub_key = f2.pub_key
	AND f2.field_name = 'booktitle'
	LEFT OUTER JOIN field f3 ON f1.pub_key = f3.pub_key
	AND f3.field_name = 'publisher'
	LEFT OUTER JOIN field f4 ON f1.pub_key = f4.pub_key
	AND f4.field_name = 'isbn',
	pub p1
WHERE
	f1.pub_key = p.pub_key
	AND f1.pub_key = p1.pub_key
	AND p1.pub_type = 'incollection';

------------------------------------------------------------------------
INSERT INTO inproceedings (pub_id, book_title, editor)
SELECT DISTINCT
	p.pub_id,
	f2.field_value,
	f3.field_value
FROM
	publication p,
	field f1
	LEFT OUTER JOIN field f2 ON f1.pub_key = f2.pub_key
	AND f2.field_name = 'booktitle'
	LEFT OUTER JOIN field f3 ON f1.pub_key = f3.pub_key
	AND f3.field_name = 'editor',
	pub p1
WHERE
	f1.pub_key = p.pub_key
	AND f1.pub_key = p1.pub_key
	AND p1.pub_type = 'inproceedings';

------------------------------------------------------------------------
INSERT INTO authored (author_id, pub_id)
SELECT DISTINCT
	a.author_id,
	p.pub_id
FROM
	field f,
	author a,
	publication p
WHERE
	f.pub_key = p.pub_key
	AND f.field_name = 'author'
	AND f.field_value = a. "name";

------------------------------------------------------------------------