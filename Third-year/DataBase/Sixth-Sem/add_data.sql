insert into company(id, company_name) values (1, 'Lada'),
                                             (2, 'StroiZavod'),
                                             (3, 'Molot');


insert into employee values (1, 'Anastasia', 'Kirekova', '1234202021', 'engineering_staff', '10.01.2022', null),
                            (2, 'Petr', 'Gorlanov', '3428982122', 'worker', '10.01.2019', null),
                            (3, 'Maksim', 'Bobrov', '0116289821', 'worker', '11.12.2021', null),
                            (4, 'Kirill', 'Greer', '2328982122', 'worker', '23.07.2020', '30.09.2022'),
                            (5, 'Ivan', 'Motruhin', '5432092502', 'engineering_staff', '21.02.2021', null),
                            (6, 'Andrei', 'Gurkin', '3454234523', 'engineering_staff', '08.04.2022', null),
                            (7, 'Kirill', 'Flout', '3450004523', 'engineering_staff', '08.04.2022', '08.11.2022'),
                            (8, 'Aleksei', 'Gurkin', '9994234523', 'worker', '12.12.2022', null),
                            (9, 'Damir', 'Lend', '3454234222', 'worker', '28.10.2022', null),
                            (10, 'Vladimir', 'Derop', '3454298763', 'engineering_staff', '23.11.2022', null),
                            (11, 'Konstantin', 'Lok', '0951298763', 'tester', '21.10.2022', null),
                            (12, 'Aleksei', 'Korjov', '0854298763', 'tester', '23.01.2021', null),
                            (13, 'Ivan', 'Siroc', '0754298763', 'tester', '27.02.2023', null);

insert into worker values (2, 'locksmith'),
                          (3, 'welder'),
                          (4, 'assembler'),
                          (9, 'assembler');

insert into locksmith values (2, 'toolmaker');

insert into welder values (3, 'thermite');

insert into assembler values (4, 'motor'),
                             (9, 'chassis');

insert into engineering_staff values (1, 'engineer'),
                                     (5, 'technician'),
                                     (6, 'technologist'),
                                     (7, 'engineer'),
                                     (10, 'technologist');

insert into brigadier values (4),
                             (3);


insert into brigade values (1, 'Best', 3),
                           (2, 'Nozzle', 3),
                           (3, 'Import', 4);

insert into worker_brigade values (1, 2, 1),
                                  (2, 2, 2),
                                  (3, 3, 3),
                                  (4, 4, 1),
                                  (5, 9, 3);

insert into department_chief values (1),
                                    (10);

insert into department values (1, 'motor', 1, 1),
                              (2, 'device', 1, 10);

insert into department_region_chief values (1),
                                           (5);

insert into department_region values (1, 'First', 1, 1, 1),
                                     (2, 'Build', 2, 3, 5);

insert into category_product values ('motorbike'),
                                    ('truck'),
                                    ('passenger_car'),
                                    ('bus'),
                                    ('agricultural_car'),
                                    ('road_construction_car');

insert into category_type values ('Dodge Tomahawk', 'motorbike'),
                                 ('Suzuki Hayabusa', 'motorbike'),
                                 ('Yamaha YZF-R1', 'motorbike'),
                                 ('Ducati – 1098', 'motorbike'),
                                 ('kamaz-5490', 'truck'),
                                 ('Foton Auman', 'truck'),
                                 ('MAN TGX', 'truck'),
                                 ('Hyundai H350', 'bus'),
                                 ('Volvo 7900', 'bus'),
                                 ('Maz-251', 'bus'),
                                 ('BMW X5', 'passenger_car'),
                                 ('Mercedes GT', 'passenger_car'),
                                 ('Toyota Land Cruiser Prado', 'passenger_car'),
                                 ('claas', 'agricultural_car'),
                                 ('New Holland', 'agricultural_car'),
                                 ('niva', 'agricultural_car'),
                                 ('Volvo Construction Equipment', 'road_construction_car'),
                                 ('Liebherr R920', 'road_construction_car'),
                                 ('Cat 320D2L', 'road_construction_car');

insert into motorbike values ('Dodge Tomahawk', 220),
                             ('Suzuki Hayabusa', 200),
                             ('Yamaha YZF-R1', 180);

insert into truck values ('kamaz-5490', 20000),
                         ('Foton Auman', 30000),
                         ('MAN TGX', 40000);

insert into passenger_car values ('BMW X5', 5),
                                 ('Mercedes GT', 3),
                                 ('Toyota Land Cruiser Prado', 5);


insert into bus values ('Hyundai H350', 20),
                       ('Volvo 7900', 25),
                       ('Maz-251', 35);

insert into agricultural_car values ('claas', 1000),
                                    ('New Holland', 1500),
                                    ('niva', 2100);

insert into road_construction_car values ('Volvo Construction Equipment', 20000),
                                         ('Liebherr R920', 24000),
                                         ('Cat 320D2L', 22500);

insert into product values (1, 'Dodge Tomahawk', 1, 'David Belov'),
                           (2, 'kamaz-5490', 1, 'StroiMash'),
                           (3, 'Toyota Land Cruiser Prado', 2, 'Eduard Lomov');

insert into product_process values (1, 1, 'motor manufacturing', 2, 'assembly', null),
                                   (2, 1, 'packing', 1, 'release', '14.03.2021'),
                                   (3, 2, 'smth', 2, 'assembly', null),
                                   (4, 2, 'smth2', 1, 'testing', null),
                                   (5, 2, 'smth3', 1, 'release', '12.01.2022');


insert into region_brigade values (1, 1, 1),
                                  (2, 1, 2),
                                  (3, 1, 3),
                                  (4, 2, 3),
                                  (5, 2, 2);

insert into master values (6, 5);

insert into laboratory values (1, 'crash'),
                              (2, 'nondestructive'),
                              (3, 'metrology');

insert into laboratory_employee values (11, 1),
                                       (12, 2),
                                       (13, 3);

insert into equipment values ('0213213212'),
                             ('3433432121'),
                             ('2344213433'),
                             ('4332123231');

insert into test values (1, '0213213212', 11, 'strength'),
                                (2, '3433432121', 11, 'stability'),
                                (3, '2344213433', 12, 'solidity'),
                                (4, '4332123231', 13, 'permanency'),
                                (5, '3433432121', 12, '3433432121');

insert into laboratory_order values (1, 1, 'crash testing', '12.12.2021', 1),
                                    (2, 1, 'vehicle chassis check', '10.10.2020', 2),
                                    (3, 1, 'vehicle wiring check', '13.02.2022', 3);

insert into order_test values (1, 1, 1),
                              (2, 1, 2),
                              (3, 1, 3),
                              (4, 1, 4),
                              (5, 1, 5),
                              (6, 2, 2),
                              (7, 3, 2),
                              (8, 3, 5);
