ALTER TABLE article
	ADD CONSTRAINT FK_Pub_id FOREIGN KEY (pub_id) REFERENCES publication (pub_id);

ALTER TABLE book
	ADD CONSTRAINT FK_Pub_id FOREIGN KEY (pub_id) REFERENCES publication (pub_id);

ALTER TABLE incollection
	ADD CONSTRAINT FK_Pub_id FOREIGN KEY (pub_id) REFERENCES publication (pub_id);

ALTER TABLE inproceedings
	ADD CONSTRAINT FK_Pub_id FOREIGN KEY (pub_id) REFERENCES publication (pub_id);

ALTER TABLE authored
	ADD CONSTRAINT FK_Pub_id FOREIGN KEY (pub_id) REFERENCES publication (pub_id),
		ADD CONSTRAINT FK_Author_id FOREIGN KEY (author_id) REFERENCES author (author_id);

CREATE INDEX auth_pub ON authored (author_id, pub_id);
