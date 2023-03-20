CREATE OR REPLACE FUNCTION check_brigadier_is_worker()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM (employee e
    JOIN employee_category_type ect ON ect.id = e.employee_type)
        JOIN employee_category ec on ec.id = ect.employee_category
    WHERE e.id = NEW.id AND ec.name = 'worker'
  ) THEN
    RAISE EXCEPTION 'Brigadier should have employee_type = worker';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER brigadier_is_worker
BEFORE INSERT OR UPDATE ON brigadier
FOR EACH ROW
EXECUTE FUNCTION check_brigadier_is_worker();


CREATE OR REPLACE FUNCTION check_brigade_employee_is_worker()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM (employee e
    JOIN employee_category_type ect ON ect.id = e.employee_type)
        JOIN employee_category ec on ec.id = ect.employee_category
    WHERE e.id = NEW.worker AND ec.name = 'worker'
  ) THEN
    RAISE EXCEPTION 'Brigade employee should have employee_type = worker';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER brigadier_is_worker
BEFORE INSERT OR UPDATE ON worker_brigade
FOR EACH ROW
EXECUTE FUNCTION check_brigade_employee_is_worker();


CREATE OR REPLACE FUNCTION check_department_chief_is_engineer()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM (employee e
    JOIN employee_category_type ect ON ect.id = e.employee_type)
        JOIN employee_category ec on ec.id = ect.employee_category
    WHERE e.id = NEW.id AND ec.name = 'engineering_staff'
  ) THEN
    RAISE EXCEPTION 'Department chief should have employee_type = engineering_staff';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER department_chief_is_engineer
BEFORE INSERT OR UPDATE ON department_chief
FOR EACH ROW
EXECUTE FUNCTION check_department_chief_is_engineer();


CREATE OR REPLACE FUNCTION check_department_region_chief_is_engineer()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM (employee e
    JOIN employee_category_type ect ON ect.id = e.employee_type)
        JOIN employee_category ec on ec.id = ect.employee_category
    WHERE e.id = NEW.id AND ec.name = 'engineering_staff'
  ) THEN
    RAISE EXCEPTION 'Department region chief should have employee_type = engineering_staff';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER department_region_chief_is_engineer
BEFORE INSERT OR UPDATE ON department_region_chief
FOR EACH ROW
EXECUTE FUNCTION check_department_region_chief_is_engineer();


CREATE OR REPLACE FUNCTION check_master_is_engineer()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM (employee e
    JOIN employee_category_type ect ON ect.id = e.employee_type)
        JOIN employee_category ec on ec.id = ect.employee_category
    WHERE e.id = NEW.id AND ec.name = 'engineering_staff'
  ) THEN
    RAISE EXCEPTION 'Master should have employee_type = engineering_staff';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER master_is_engineer
BEFORE INSERT OR UPDATE ON master
FOR EACH ROW
EXECUTE FUNCTION check_master_is_engineer();



CREATE OR REPLACE FUNCTION check_laboratory_employee_is_tester()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM (employee e
    JOIN employee_category_type ect ON ect.id = e.employee_type)
        JOIN employee_category ec on ec.id = ect.employee_category
    WHERE e.id = NEW.id AND ec.name = 'laboratory_tester'
  ) THEN
    RAISE EXCEPTION 'Laboratory employee is tester should have employee_type = tester';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER laboratory_employee_is_tester
BEFORE INSERT OR UPDATE ON laboratory_employee
FOR EACH ROW
EXECUTE FUNCTION check_laboratory_employee_is_tester();


CREATE OR REPLACE FUNCTION check_employee_has_attribute()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM (employee e
    JOIN employee_category_type ect ON ect.id = e.employee_type)
    WHERE e.id = NEW.employee AND e.employee_type = NEW.attribute
  ) THEN
    RAISE EXCEPTION 'Employee has no such attribute';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER employee_attribute
BEFORE INSERT OR UPDATE ON employee_property
FOR EACH ROW
EXECUTE FUNCTION check_employee_has_attribute();


CREATE OR REPLACE FUNCTION check_product_has_attribute()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM product, product_category_type
    WHERE product.id = NEW.product AND
          product.category_type = product_category_type.id AND product_category_type.category =
        (SELECT product_type_attribute.category FROM product_type_attribute WHERE NEW.attribute = product_type_attribute.id)
    ) THEN
    RAISE EXCEPTION 'Product has no such attribute';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER check_employee_attribute
BEFORE INSERT OR UPDATE ON product_property
FOR EACH ROW
EXECUTE FUNCTION check_product_has_attribute();


CREATE OR REPLACE FUNCTION check_status_product_process()
RETURNS TRIGGER AS $$
BEGIN
  IF (NEW.status = 'assembled' AND NEW.release_date IS NULL) OR (NEW.status <> 'assembled' AND NEW.release_date IS NOT NULL) THEN
    RAISE EXCEPTION 'Product process should has release_date if status is assembled';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER status_product_process
BEFORE INSERT OR UPDATE ON product_process
FOR EACH ROW
EXECUTE FUNCTION check_status_product_process();
