-- 1. Получить перечень видов изделий отдельной категории и в целом, собираемых указанным цехом, предприятием.

--  Для цеха всех категорий
    select distinct product.category_type, product.customer from product, department_region
                                                            where product.manufacturer_department = department_region.id and
                                                                  department_region.department_id = 1;

--  Для предприятия всех категорий
    select distinct product.category_type from product, department, company
                where product.manufacturer_department = department.id and
                      department.company_id = company.id and
                      company.id = 1;

--  Для предприятия отдельной категории
    select distinct product.category_type from product, department, company, category_type
                where product.manufacturer_department = department.id and
                      department.company_id = company.id and
                      company.id = 1 and
                      product.category_type = category_type.name and
                      category_type.category = 'motorbike';

-- 2. Получить число и перечень изделий отдельной категории и в целом, собранных указанным цехом, участком, предприятием
--    в целом за определенный отрезок времени.

-- В целом
select public.product.category_type, count(*) from product_process, product
               where product_process.product = product.id and
                     product_process.status_work = 'release' and
                     product_process.release_date > '12.12.2012' and product_process.release_date < '13.12.2023'
group by product.category_type;

-- В указанном предпирятии и с указанным типом
select public.product.category_type, count(*) from product_process, product, department, category_type
               where product_process.product = product.id and
                     product_process.status_work = 'release' and
                     product_process.release_date > '12.12.2012' and product_process.release_date < '13.12.2023' and
                     product.manufacturer_department = department.id and
                     department.company_id = 1 and
                     product.category_type = category_type.name and
                     category_type.category = 'motorbike'
group by product.category_type;


-- 3. Получить данные о кадровом составе цеха, предприятия в целом и по указанным категориям инженерно-технического
--    персонала и рабочих.

    select distinct employee.first_name, employee.second_name, employee.passport from worker_brigade, worker, employee, brigade, region_brigade
        where worker_brigade.worker = public.worker.contract_number and
              worker.contract_number = employee.contract_number and
              worker_brigade.brigade = brigade.id and
              region_brigade.brigade = brigade.id and
              region_brigade.region = 1
    union
    select distinct employee.first_name, employee.second_name, employee.passport from engineering_staff, department_region_chief, employee, department_region
                      where department_region_chief.contract_number = engineering_staff.contract_number and
                            employee.contract_number = department_region_chief.contract_number and
                            department_region.chief = department_region_chief.contract_number and
                            department_region.id = 1;




-- 4. Получить число и перечень участков указанного цеха, предприятия в целом и их начальников.


-- 5. Получить перечень работ, которые проходит указанное изделие.
    select pp.description_work, pp.status_work from product_process as pp
             where pp.product = 1;



-- 6. Получить состав бригад указанного участка, цеха.


-- 7. Получить перечень мастеров указанного участка, цеха.
--
--     select * from master, department_region_chief as drc, department_region, employee as e
--              where master.chief = drc.contract_number and
--                    drc.contract_number = department_region.chief and
--                    department_region.chief = 2 and
--                    master.person_id = e.contract_number;


-- 8. Получить перечень изделий отдельной категории и в целом, собираемых в настоящий момент указанным участком, цехом,
--    предприятием.



-- 9. Получить состав бригад, участвующих в сборке указанного изделия.


-- 10. Получить перечень испытательных лабораторий, участвующих в испытаниях некоторого конкретного изделия.


-- 11. Получить перечень изделий отдельной категории и в целом, проходивших испытание в указанной лаборатории
--     за определенный период.


-- 12. Получить перечень испытателей, участвующих в испытаниях указанного изделия, изделий отдельной категории и
--     в целом в указанной лаборатории за определенный период.



-- 13. Получить состав оборудования, использовавшегося при испытании указанного изделия, изделий отдельной категории и
--     в целом в указанной лаборатории за определенный период.


-- 14. Получить число и перечень изделий отдельной категории и в целом, собираемых указанным цехом, участком,
--     предприятием в настоящее время.

    select * from employee where employee.dismissal_date is null;