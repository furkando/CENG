CREATE TABLE Employee (
	emp_id INT NOT NULL,
	name VARCHAR(128),
	surname VARCHAR(128),
	salary INT,
	gender VARCHAR(128),
	PRIMARY KEY (emp_id));

CREATE TABLE Department (
	dept_id INT NOT NULL,
	manager_id INT DEFAULT 101,
	name VARCHAR(128),
	LOCATION VARCHAR(128),
	PRIMARY KEY (dept_id),
	FOREIGN KEY (manager_id) REFERENCES employee(emp_id) ON DELETE SET DEFAULT
);

CREATE TABLE employee_department (
	dept_id INT,
	employee_id INT,
	FOREIGN KEY (dept_id) REFERENCES department (dept_id),
	FOREIGN KEY (employee_id) REFERENCES employee (emp_id) ON DELETE CASCADE
);

CREATE TABLE employee_report (
	subordinate_id INT,
	supervisor_id INT,
	FOREIGN KEY (subordinate_id) REFERENCES employee (emp_id),
	FOREIGN KEY (supervisor_id) REFERENCES employee (emp_id)
);

CREATE TABLE project (
	project_id INT NOT NULL,
	dept_id INT NOT NULL,
	budget INT,
	state VARCHAR(128),
	due_date TIMESTAMP,
	PRIMARY KEY (project_id),
	FOREIGN KEY (dept_id) REFERENCES department (dept_id)
);

