create table if not exists Author (author_id serial primary key, name text);
create table if not exists Publication (pub_id serial primary key, pub_key text, title text, year int);
create table if not exists Article (pub_id int primary key, journal text, month text, volume text, number text);
create table if not exists Book (pub_id int primary key, publisher text, isbn text);
create table if not exists Incollection (pub_id int primary key, book_title text, publisher text, isbn text);
create table if not exists Inproceedings (pub_id int primary key, book_title text, editor text);
create table if not exists Authored (author_id int, pub_id int);
