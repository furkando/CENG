CREATE ASSERTION Total CHECK (
	EXISTS (
		SELECT
			e.emp_id
		FROM
			employee e,
			employee_department d
		WHERE
			e.emp_id = d.employee_id));