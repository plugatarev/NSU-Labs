create type employee_type as enum ('worker', 'engineering_staff', 'tester');
create type worker_type as enum ('assembler', 'turner', 'welder', 'locksmith');
create type engineer_type as enum ('engineer', 'technologist', 'technician');
create type product_status as enum ('layout', 'assembly', 'testing', 'release');

create table company(
    id                  bigint primary key,
    company_name        varchar(255) not null unique
);

create table department(
    id                 bigint primary key,
    department_name    varchar(255) not null unique,
    company_id         bigint references company(id) on delete cascade
);

create table employee(
  contract_number         bigint primary key,
  first_name              varchar(255) not null,
  second_name             varchar(255) not null,
  passport                varchar(10) not null,
  employee_type           employee_type not null,
  employment_date         date not null,
  dismissal_date          date
);

create table engineering_type(
    id bigint primary key,
    name varchar(255) not null
);

create table engineering_type_attribute(
    id  bigint,
    type bigint references engineering_type(id) on delete cascade,
    attribute bigint references attribute(id) on delete set null
);

create table engineering_staff(
    contract_number     bigint primary key references employee(contract_number) on delete cascade,
    engineering_type    bigint references engineering_type(id) on delete cascade
);

create table engineering_type(
    id bigint primary key,
    name varchar(255) not null
);

create table worker(
    contract_number     bigint primary key references employee(contract_number) on delete cascade,
    worker_type         bigint references engineering_type(id) on delete cascade
);

create table engineer(
    contract_number       bigint primary key references engineering_staff(contract_number) on delete cascade,
    education             varchar(255) not null
);

create table technologist(
    contract_number       bigint primary key references engineering_staff(contract_number) on delete cascade,
    professional_grade    varchar(255) not null
);

create table technician(
    contract_number       bigint primary key references engineering_staff(contract_number) on delete cascade,
    specialization        varchar(255) not null
);

create table assembler(
    contract_number       bigint primary key references worker(contract_number) on delete cascade,
    detail_type           varchar(255) not null
);

create table turner(
    contract_number       bigint primary key references worker(contract_number) on delete cascade,
    specialization        varchar(255) not null
);

create table welder(
    contract_number       bigint primary key references worker(contract_number) on delete cascade,
    metal_welding_type    varchar(255) not null
);

create table locksmith(
    contract_number       bigint primary key references worker(contract_number) on delete cascade,
    profile               varchar(255) not null
);

create table brigadier(
    contract_number       bigint primary key references worker(contract_number) on delete cascade
);

create table brigade(
    id              bigint primary key,
    brigade_name    varchar(255) not null unique,
    brigadier       bigint references brigadier(contract_number) on delete set null
);

create table worker_brigade(
    id              bigint primary key,
    worker          bigint references worker(contract_number) on delete cascade,
    brigade         bigint references brigade(id) on delete cascade
);

create table department_chief(
    contract_number       bigint primary key references engineering_staff(contract_number) on delete cascade
);

create table department_region_chief(
    contract_number      bigint primary key references engineering_staff(contract_number) on delete cascade
);

create table department_region (
    id                  bigint primary key,
    region_name         varchar(255) not null unique,
    department_id       bigint references department(id) on delete cascade,
    collecting_brigade  bigint references brigade(id) on delete set null,
    chief bigint references department_region_chief(contract_number) on delete set null
);

create table category_product(
    name    varchar(255) primary key
);

create table attribute(
    id                  bigint primary key,
    attribute_name      varchar(255) not null,
    category_product    varchar(255) references category_product(name) on delete cascade
);

create table category_type(
    name        varchar(255) primary key,
    category    varchar(255) references category_product(name) on delete cascade
);

create table product(
    id                      bigint primary key,
    category_type           varchar(255) references category_type(name) on delete cascade,
    manufacturer_department bigint references department(id) on delete set null,
    customer                varchar(255) not null
);

create table product_attribute(
    id  bigint,
    product bigint references product(id) on delete cascade,
    attribute bigint references attribute(id) on delete set null
);

-- create table motorbike(
--     motorbike_type     varchar(255) primary key references category_type(name) on delete cascade,
--     engine_capacity    int not null check (engine_capacity > 0)
-- );
--
-- create table truck(
--     truck_type         varchar(255) primary key references category_type(name) on delete cascade,
--     load_capacity      int not null check (load_capacity > 0)
-- );
--
-- create table passenger_car(
--     passenger_car_type      varchar(255) primary key references category_type(name) on delete cascade,
--     doors_number            int not null check (doors_number > 0)
-- );
--
-- create table bus(
--     bus_type        varchar(255) primary key references category_type(name) on delete cascade,
--     capacity        int not null check (capacity > 10)
-- );
--
-- create table agricultural_car(
--     agricultural_car_type      varchar(255) primary key references category_type(name) on delete cascade,
--     performance                int not null check (performance > 0)
-- );
--
-- create table road_construction_car(
--     road_construction_car_type      varchar(255) primary key references category_type(name) on delete cascade,
--     pressure                        int not null check (pressure > 0)
-- );

create table product_process(
    id                  bigint primary key,
    product             bigint references product(id) on delete cascade,
    description_work    text not null,
    department_region   bigint references department_region(id) on delete set null,
    status_work         product_status not null,
    release_date        date
);

create table region_brigade(
    id      bigint primary key,
    region  bigint references department_region(id),
    brigade bigint references brigade(id)
);

create table master(
    person_id    bigint primary key references engineering_staff(contract_number) on delete cascade,
    chief        bigint references department_region_chief(contract_number) on delete set null
);

create table laboratory(
    id              bigint primary key,
    laboratory_name varchar(255) not null unique
);

create table laboratory_employee(
    id              bigint primary key references employee(contract_number) on delete cascade,
    laboratory      bigint references laboratory(id) on delete set null
);

create table equipment(
    serial_number   varchar(10) not null unique
);

create table test(
    id                  bigint primary key,
    equipment           varchar(10) references equipment(serial_number) on delete cascade,
    laboratory_employee bigint references laboratory_employee(id) on delete cascade,
    description         varchar(255) not null
);

create table laboratory_order(
    id              bigint primary key,
    product         bigint references product(id) on delete cascade,
    description     text not null,
    receipt_date    date not null,
    laboratory      bigint references laboratory(id) on delete cascade
);

create table order_test(
    id                  bigint primary key,
    laboratory_order    bigint references laboratory_order(id) on delete cascade,
    test                bigint references test(id) on delete cascade
);

alter table department add chief bigint references department_chief(contract_number) on delete set null;
