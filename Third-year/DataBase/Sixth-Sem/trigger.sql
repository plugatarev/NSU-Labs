CREATE OR REPLACE FUNCTION check_brigadier_is_worker()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM employee e, employee_category_type ect, employee_category ec
             WHERE e.id = NEW.id AND
                   e.employee_type = ect.id AND
                   ect.employee_category = ec.id AND
                   ec.name = 'worker'
  ) THEN
    RAISE EXCEPTION 'Brigadier must have employee_type = worker';
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
    SELECT 1 FROM employee e, employee_category_type ect, employee_category ec
             WHERE e.id = NEW.worker AND
                   e.employee_type = ect.id AND
                   ect.employee_category = ec.id AND
                   ec.name = 'worker'
  ) THEN
    RAISE EXCEPTION 'Brigade employee must have employee_type = worker';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER brigade_employee_is_worker
BEFORE INSERT OR UPDATE ON worker_brigade
FOR EACH ROW
EXECUTE FUNCTION check_brigade_employee_is_worker();


CREATE OR REPLACE FUNCTION check_employee_is_engineer_staff()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM employee e, employee_category_type ect, employee_category ec
             WHERE e.id = NEW.id AND
                   e.employee_type = ect.id AND
                   ect.employee_category = ec.id AND
                   ec.name = 'engineering_staff'
  ) THEN
    RAISE EXCEPTION 'Employee must have employee_type = engineering_staff';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER department_chief_is_engineer
BEFORE INSERT OR UPDATE ON department_chief
FOR EACH ROW
EXECUTE FUNCTION check_employee_is_engineer_staff();

CREATE OR REPLACE TRIGGER department_region_chief_is_engineer
BEFORE INSERT OR UPDATE ON department_region_chief
FOR EACH ROW
EXECUTE FUNCTION check_employee_is_engineer_staff();

CREATE OR REPLACE TRIGGER master_is_engineer
BEFORE INSERT OR UPDATE ON master
FOR EACH ROW
EXECUTE FUNCTION check_employee_is_engineer_staff();


CREATE OR REPLACE FUNCTION check_laboratory_employee_is_tester()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM employee e, employee_category_type ect, employee_category ec
             WHERE e.id = NEW.id AND
                   e.employee_type = ect.id AND
                   ect.employee_category = ec.id AND
                   ec.name = 'laboratory_tester'
  ) THEN
    RAISE EXCEPTION 'Laboratory employee must have employee_type = tester';
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
    SELECT 1 FROM employee e, employee_type_attribute eta
             WHERE e.id = NEW.employee AND
                   NEW.attribute = eta.id AND
                   eta.employee_type = e.employee_type
  ) THEN
    RAISE EXCEPTION 'Employee has no such attribute';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER employee_has_attribute
BEFORE INSERT OR UPDATE ON employee_property
FOR EACH ROW
EXECUTE FUNCTION check_employee_has_attribute();


CREATE OR REPLACE FUNCTION check_product_has_attribute()
RETURNS TRIGGER AS $$
BEGIN
  IF NOT EXISTS (
    SELECT 1 FROM product p, product_category_type pct, product_type_attribute pta
             WHERE NEW.product = p.id AND
                   NEW.attribute = pta.id AND
                   p.category_type = pct.id AND
                   pta.category = pct.category
    ) THEN
    RAISE EXCEPTION 'Product has no such attribute';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER product_has_attribute
BEFORE INSERT OR UPDATE ON product_property
FOR EACH ROW
EXECUTE FUNCTION check_product_has_attribute();



CREATE OR REPLACE FUNCTION check_status_product_process()
RETURNS TRIGGER AS $$
    DECLARE status_name VARCHAR(255);
BEGIN
  SELECT product_status.name INTO status_name FROM product_status WHERE NEW.status = product_status.id;

  IF ((status_name = 'assembled' AND NEW.release_date IS NULL) OR
      (status_name <> 'assembled' AND NEW.release_date IS NOT NULL))
  THEN RAISE EXCEPTION 'Product process must has release_date only if status is assembled';
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER status_product_process
BEFORE INSERT OR UPDATE ON product_process
FOR EACH ROW
EXECUTE FUNCTION check_status_product_process();

