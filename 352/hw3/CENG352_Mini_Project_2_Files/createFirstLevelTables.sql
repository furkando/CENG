create table if not exists Pub (pub_key text,  pub_type text);
create table if not exists Field (pub_key text, field_index int, field_name text, field_value text);

\copy Pub from './pubFile.csv';
\copy Field from './fieldFile.csv';
