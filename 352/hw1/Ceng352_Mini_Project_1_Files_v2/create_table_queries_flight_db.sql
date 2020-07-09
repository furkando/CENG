create table if not exists world_area_codes (
	wac_id int not null,
	wac_name varchar(100) not null,
	primary key (wac_id)
);

create table if not exists weekdays (
	weekday_id int not null,
	weekday_name varchar(10) not null,
	primary key (weekday_id)
);


create table if not exists cancellation_reasons (
	reason_code varchar(1) not null,
	reason_desc varchar(20) not null,
	primary key (reason_code)
);


create table if not exists airport_ids (
	airport_id int not null,
	airport_desc varchar(200) not null,
	primary key (airport_id)
);

create table if not exists airport_codes (
	airport_code varchar(5) not null,
	airport_desc varchar(200) not null,
	primary key (airport_code)
);

create table if not exists airline_codes (
	airline_code varchar(10) not null,
	airline_name varchar(200) not null,
	primary key (airline_code)
);

create table if not exists flight_reports (

	report_id serial not null,
	"year" int,
	"month" int,
	"day" int,
	weekday_id int,
	airline_code varchar(10),
	plane_tail_number varchar(10),
	origin_airport_id int,
	origin_airport_code varchar(5),
	origin_city_name varchar(100),
	origin_wac_id int,
	dest_airport_id int,
	dest_airport_code varchar(5),
	dest_city_name varchar(100),
	dest_wac_id int,
	departure_time varchar(5),
	departure_delay float8,
	taxi_out_time float8,
	wheels_off_time varchar(5),
	wheels_on_time varchar(5),
	taxi_in_time float8,
	arrival_time varchar(5),
	arrival_delay float8,
	is_cancelled float8,
	cancellation_reason varchar(1),
	is_diverted float8,
	flight_time float8,
	flight_distance float8,
	
	primary key (report_id),
	foreign key (weekday_id) references weekdays(weekday_id),
	foreign key (airline_code) references airline_codes(airline_code),
	foreign key (origin_airport_id) references airport_ids(airport_id),
	foreign key (origin_airport_code) references airport_codes(airport_code),
	foreign key (origin_wac_id) references world_area_codes(wac_id),
	foreign key (dest_airport_id) references airport_ids(airport_id),
	foreign key (dest_airport_code) references airport_codes(airport_code),
	foreign key (dest_wac_id) references world_area_codes(wac_id),
	foreign key (cancellation_reason) references cancellation_reasons(reason_code)
);


