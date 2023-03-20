create table department(
    id                 bigint primary key,
    department_name    varchar(255) not null unique
);

create table employee_category(
    id      bigint primary key,
    name    varchar(255) not null unique
);

create table employee_category_type(
    id                bigint primary key,
    name              varchar(255) not null unique,
    employee_category bigint references employee_category(id) on delete cascade on update cascade
);

create table employee_type_attribute(
    id                  bigint primary key,
    attribute_name      varchar(255) not null,
    employee_type       bigint references employee_category_type(id) on delete cascade on update cascade
);

create table employee(
  id                      bigint primary key,
  first_name              varchar(255) not null,
  second_name             varchar(255) not null,
  passport                varchar(10) not null,
  employee_type           bigint references employee_category_type(id) on delete set null on update cascade,
  employment_date         date not null,
  dismissal_date          date
);

-- ok
-- TODO: проверить employee_type_attribute.employee_type = employee.employee_category_type
create table employee_property(
    id             bigint primary key,
    employee       bigint references employee(id) on delete cascade on update cascade,
    attribute      bigint references employee_type_attribute(id) on delete cascade on update cascade,
    value          varchar(255)
);

-- ok
-- TODO: триггер на добавление работников в бригадиры - тип должен быть worker
create table brigadier(
    id    bigint primary key references employee(id) on delete cascade on update cascade
);

create table brigade(
    id              bigint primary key,
    brigade_name    varchar(255) not null unique,
    brigadier       bigint references brigadier(id) on delete set null on update cascade
);

-- ok
-- TODO: триггер на добавление работника и бригады - тип должен быть worker
create table worker_brigade(
    id              bigint primary key,
    worker          bigint references employee(id) on delete cascade on update cascade,
    brigade         bigint references brigade(id) on delete cascade on update cascade
);

-- ok
-- TODO: триггер на добавление работника в начальника цеха - тип должен быть engineer_staff
create table department_chief(
    id     bigint primary key references employee(id) on delete cascade on update cascade
);

-- ok
-- TODO: триггер на добавление работника в начальника департамента - тип должен быть engineer_staff
create table department_region_chief(
    id     bigint primary key references employee(id) on delete cascade on update cascade
);

create table department_region (
    id                  bigint primary key,
    region_name         varchar(255) not null unique,
    department_id       bigint references department(id) on delete cascade on update cascade,
    collecting_brigade  bigint references brigade(id) on delete set null on update cascade,
    chief               bigint references department_region_chief(id) on delete set null on update cascade
);

create table product_category(
    id      bigint primary key,
    name    varchar(255) not null unique
);

create table product_type_attribute(
    id                  bigint primary key,
    attribute_name      varchar(255) not null,
    category    bigint references product_category(id) on delete cascade on update cascade
);

create table product_category_type(
    id          bigint primary key,
    name        varchar(255) not null unique,
    category    bigint references product_category(id) on delete cascade on update cascade
);

create table product(
    id                      bigint primary key,
    category_type           bigint references product_category_type(id) on delete cascade on update cascade,
    manufacturer_department bigint references department(id) on delete set null on update cascade,
    customer                varchar(255) not null
);

-- ok
-- TODO
create table product_property(
    id          bigint primary key,
    product     bigint references product(id) on delete cascade on update cascade,
    attribute   bigint references product_type_attribute(id) on delete cascade on update cascade,
    value       varchar(255)
);

create table product_status(
    name varchar(255) primary key
);

create table product_process(
    id                  bigint primary key,
    product             bigint references product(id) on delete cascade on update cascade,
    description_work    text not null,
    department_region   bigint references department_region(id) on delete set null on update cascade,
    status_work         varchar(255) references product_status(name) on delete cascade on update cascade,
    release_date        date
);

create table region_brigade(
    id      bigint primary key,
    region  bigint references department_region(id),
    brigade bigint references brigade(id)
);

-- ok
-- TODO: триггер на добавление работника в мастеров - тип должен быть engineer_staff
create table master(
    id           bigint primary key references employee(id) on delete cascade on update cascade,
    chief        bigint references department_region_chief(id) on delete set null on update cascade
);

create table laboratory(
    id              bigint primary key,
    laboratory_name varchar(255) not null unique
);

-- ok
-- TODO: триггер, что человек является лабораторным работником
create table laboratory_employee(
    id              bigint primary key references employee(id) on delete cascade on update cascade,
    laboratory      bigint references laboratory(id) on delete set null on update cascade
);

create table equipment(
    serial_number   varchar(10) not null unique
);

create table test(
    id                  bigint primary key,
    equipment           varchar(10) references equipment(serial_number) on delete cascade on update cascade,
    laboratory_employee bigint references laboratory_employee(id) on delete cascade on update cascade,
    description         varchar(255) not null
);

create table laboratory_order(
    id              bigint primary key,
    product         bigint references product(id) on delete cascade on update cascade,
    description     text not null,
    receipt_date    date not null,
    laboratory      bigint references laboratory(id) on delete cascade on update cascade
);

create table order_test(
    id                  bigint primary key,
    laboratory_order    bigint references laboratory_order(id) on delete cascade on update cascade,
    test                bigint references test(id) on delete cascade on update cascade
);