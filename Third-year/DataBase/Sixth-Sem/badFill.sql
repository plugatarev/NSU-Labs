insert into brigadier values (1);
insert into brigadier values (8);
delete from brigadier where brigadier.id = 8;

insert into worker_brigade values(50, 1, 1);
insert into worker_brigade values (50, 8, 1);
delete from worker_brigade where worker_brigade.worker = 8;

insert into department_chief values (8);
insert into department_chief values (1);
delete from department_chief where department_chief.id = 1;

insert into department_region_chief values (8);
insert into department_region_chief values (1);
delete from department_region_chief where department_region_chief.id = 8;

insert into master values (8);
insert into master values (1);
delete from master where master.id = 1;

insert into laboratory_employee values (1);
insert into laboratory_employee values (12);
delete from laboratory_employee where laboratory_employee.id = 12;

insert into employee values (123, 'asdfsd', 'sdfsd', '1234qwer32', 1, '21.12.2022', null);
delete from employee where employee.id = 123;
insert into employee_property values (50, 123, 2, 'sdf');
insert into employee_property values (50, 123, 1, 'nsu');
delete from employee_property where employee_property.id = 50;

insert into product values (123, 2, 1, '1234qwer32');
delete from product where product.id = 123;
insert into product_property values (50, 123, 4, 'bad');
insert into product_property values (50, 123, 1, 'ok');
delete from product_property where product_property.id = 50;

insert into product_process values (28, 1, 'motor manufacturing', 2, 2, '12.12.2021')
