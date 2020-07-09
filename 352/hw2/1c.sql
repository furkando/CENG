alter table employee add constraint salaryCheck check(salary > 36000);

alter table department add constraint nameCheck check("name" LIKE '%' || "location" || '%');