-- 1. Получить перечень видов изделий отдельной категории и в целом, собираемых указанным цехом, предприятием.

    select distinct product_category_type.name
        from product, product_category_type, product_category, department
            where product.category_type = product_category_type.id and
                  product_category_type.category = product_category.id and
                  product_category.name = 'passenger_car' and
                  product.manufacturer_department = department.id and
                  department.company = 2;


-- 2. Получить число и перечень изделий отдельной категории и в целом, собранных указанным цехом, участком, предприятием
--    в целом за определенный отрезок времени.

    select product_category_type.name, count(*)
        from product_process, product, department, department_region, product_category_type, product_category, product_status
             where product_process.product = product.id and
                   product_process.status = product_status.id and
                   product_status.name = 'assembled' and
                   product_process.release_date > '12.12.2012' and product_process.release_date < '13.12.2023' and
                   product.category_type = product_category_type.id and
                   product_category_type.category = product_category.id and
                   product_category.name = 'passenger_car' and
                   product_process.department_region = department_region.id and
                   department_region.department_id = department.id and
                   department.company = 2
    group by product_category_type.name;


-- 3. Получить данные о кадровом составе цеха, предприятия в целом и по указанным категориям инженерно-технического
--    персонала и рабочих.

    create view company_staff as
    with company_const as (values (1))
    select distinct employee.first_name, employee.second_name, employee.passport, employee.employment_date, employee.employee_type
        from employee, brigadier, brigade, department_region, department, company_const
            where brigadier.id = employee.id and
                  brigade.brigadier = brigadier.id and
                  brigade.department_region = department_region.id and
                  department_region.department_id = department.id and
                  department.company = company_const.column1
    union
    select distinct employee.first_name, employee.second_name, employee.passport, employee.employment_date, employee.employee_type
        from employee, worker_brigade, brigade, department_region, department, company_const
            where worker_brigade.worker = employee.id and
                  worker_brigade.brigade = brigade.id and
                  brigade.department_region = department_region.id and
                  department_region.department_id = department.id and
                  department.company = company_const.column1
    union
    select distinct employee.first_name, employee.second_name, employee.passport, employee.employment_date, employee.employee_type
    from employee, department_chief, department_region, department, company_const
        where department_chief.id = employee.id and
              department_region.chief = department_chief.id and
                  department_region.department_id = department.id and
                  department.company = company_const.column1
    union
    select distinct employee.first_name, employee.second_name, employee.passport, employee.employment_date, employee.employee_type
        from employee, department_region_chief, department_region, department, company_const
            where department_region_chief.id = employee.id and
                  department_region.chief = department_region_chief.id and
                  department_region.department_id = department.id and
                  department.company = company_const.column1
    union
    select distinct employee.first_name, employee.second_name, employee.passport, employee.employment_date, employee.employee_type
        from employee, master, department_region_chief, department_region, department, company_const
            where master.id = employee.id and
                  master.chief = department_region_chief.id and
                  department_region.chief = department_region_chief.id and
                  department_region.department_id = department.id and
                  department.company = company_const.column1;

    select first_name, second_name, passport, employment_date, employee_category_type.name as employee_category_type
        from company_staff, employee_category, employee_category_type
             where company_staff.employee_type = employee_category_type.id and
                   employee_category_type.employee_category = public.employee_category.id and
                   employee_category.name = 'engineering_staff';


-- 4. Получить число и перечень участков указанного цеха, предприятия в целом и их начальников.

    create or replace view regions as
    select department_region.id, region_name, collecting_brigade, employee.first_name chief_first_name, employee.second_name chief_second_name
        from department_region, department, employee
             where department_region.department_id = department.id and
                   department_region.chief = employee.id and
                   department.company = 2;

    select * from regions;
    select count(*) from regions;


-- 5. Получить перечень работ, которые проходит указанное изделие.

    select pp.description_work, pp.status
        from product_process as pp
             where pp.product = 1;


-- 6. Получить состав бригад указанного участка, цеха.

    select distinct e.first_name, e.second_name, e.employment_date, et.name
        from worker_brigade, brigade, department_region, employee e, employee_category_type et
             where worker_brigade.brigade = brigade.id and
                   e.id = worker_brigade.worker and
                   e.employee_type = et.id and
                   brigade.department_region = department_region.id and
                   department_region.department_id = 1;


-- 7. Получить перечень мастеров указанного участка, цеха.

    select distinct e.first_name, e.second_name, e.passport
        from master, department_region, employee e
             where master.chief = department_region.chief and
                   master.id = e.id and
                   department_region.department_id = 2;


-- 8. Получить перечень изделий отдельной категории и в целом, собираемых в настоящий момент указанным участком, цехом,
--    предприятием.

    create view products as
    select distinct p.id, product_category_type.name, p.customer
        from product_process pp, product_status ps, product p, product_category_type, product_category, department_region, department
             where pp.status = ps.id and
                   ps.name = 'assembling' and
                   pp.product = p.id and
                   p.category_type = product_category_type.id and
                   product_category_type.category = product_category.id and
                   product_category.name = 'passenger_car' and
                   pp.department_region = department_region.id and
                   department_region.department_id = department.id and
                   department.company = 1;


-- 9. Получить состав бригад, участвующих в сборке указанного изделия.

    select distinct e.first_name, e.second_name, e.passport, b.id
        from product_process pp, department_region dr, brigade b, worker_brigade wb, employee e
              where pp.product = 1 and
                    pp.department_region = dr.id and
                    dr.collecting_brigade = b.id and
                    b.id = wb.brigade and
                    wb.worker = e.id;


-- 10. Получить перечень испытательных лабораторий, участвующих в испытаниях некоторого конкретного изделия.

    select distinct laboratory.laboratory_name
        from laboratory_order, laboratory
            where laboratory_order.laboratory = laboratory.id and
                laboratory_order.product = 1;


-- 11. Получить перечень изделий отдельной категории и в целом, проходивших испытание в указанной лаборатории
--     за определенный период.

    select distinct product.id, product_category_type.name, order_test.testing_date
        from laboratory_order, product, product_category_type, product_category, order_test
              where laboratory_order.product = product.id and
                    product.category_type = product_category_type.id and
                    product_category_type.category = product_category.id and
                    product_category.name = 'passenger_car' and
                    order_test.laboratory_order = laboratory_order.id and
                    order_test.testing_date > '12.12.2012' and order_test.testing_date < '12.12.2022' and
                    laboratory_order.laboratory = 1;


-- 12. Получить перечень испытателей, участвующих в испытаниях указанного изделия, изделий отдельной категории и
--     в целом в указанной лаборатории за определенный период.

    select distinct employee.first_name, employee.second_name, employee.passport
        from laboratory_order, order_test, product_category_type, product_category, product, test, employee
              where order_test.laboratory_order = laboratory_order.id and
                    laboratory_order.product = product.id and
                    product.category_type = product_category_type.id and
                    product_category_type.category = product_category.id and
                    product_category.name = 'passenger_car' and
                    order_test.testing_date > '12.12.2020' and order_test.testing_date < '12.12.2022' and
                    test.id = order_test.test and
                    laboratory_order.laboratory = 1 and
                    test.laboratory_employee = employee.id;


-- 13. Получить состав оборудования, использовавшегося при испытании указанного изделия, изделий отдельной категории и
--     в целом в указанной лаборатории за определенный период.

    select distinct equipment.serial_number
        from laboratory_order, order_test, test, equipment, product, product_category_type, product_category
            where order_test.laboratory_order = laboratory_order.id and
                  test.id = order_test.test and
                  test.equipment = equipment.id and
                  order_test.testing_date > '12.12.2020' and order_test.testing_date < '12.12.2022' and
                  laboratory_order.product = product.id and
                  product.category_type = product_category_type.id and
                  product_category_type.category = product_category.id and
                  product_category.name = 'passenger_car';


-- 14. Получить число и перечень изделий отдельной категории и в целом, собираемых указанным цехом, участком,
--     предприятием в настоящее время.
    select * from products;
    select count(*) from products;