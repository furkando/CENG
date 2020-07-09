CREATE TRIGGER BudgetProject
	AFTER UPDATE budget ON project
	FOR EACH ROW
	WHEN (OLD.budget > NEW.budget)
	UPDATE project
	SET "state" = Unsuccessful
	WHERE
		project_id = OLD.project_id