insert into company values (1, 'Lada'),
                           (2, 'Nada');

insert into department values (1, 'motor', 1),
                              (2, 'device', 2);

insert into employee_category values (1, 'worker'),
                                     (2, 'engineering_staff'),
                                     (3, 'laboratory_tester');

insert into employee_category_type values (1, 'engineer', 2),
                                          (2, 'technologist', 2),
                                          (3, 'technician', 2),
                                          (4, 'assembler', 1),
                                          (5, 'turner', 1),
                                          (6, 'welder', 1),
                                          (7, 'locksmith', 1),
                                          (8, 'tester', 3);

insert into employee_type_attribute values (1, 'education', 1),
                                           (2, 'professional_grade', 2),
                                           (3, 'specialization', 3),
                                           (4, 'detail_type', 4),
                                           (5, 'specialization', 5),
                                           (6, 'metal_welding_type', 6),
                                           (7, 'profile', 7);

insert into employee values (1, 'Anastasia', 'Kirekova', '1234202021', 1, '10.01.2022', null),
                            (2, 'Petr', 'Gorlanov', '3428982122', 7, '10.01.2019', null),
                            (3, 'Maksim', 'Bobrov', '0116289821', 6, '11.12.2021', null),
                            (4, 'Kirill', 'Greer', '2328982122', 4, '23.07.2020', '30.09.2022'),
                            (5, 'Ivan', 'Motruhin', '5432092502', 3, '21.02.2021', null),
                            (6, 'Andrei', 'Gurkin', '3454234523', 2, '08.04.2022', null),
                            (7, 'Kirill', 'Flout', '3450004523', 1, '08.04.2022', '08.11.2022'),
                            (8, 'Aleksei', 'Gurkin', '9994234523', 5, '12.12.2022', null),
                            (9, 'Damir', 'Lend', '3454234222', 4, '28.10.2022', null),
                            (10, 'Vladimir', 'Derop', '3454298763', 2, '23.11.2022', null),
                            (11, 'Konstantin', 'Lok', '0951298763', 8, '21.10.2022', null),
                            (12, 'Aleksei', 'Korjov', '0854298763', 8, '23.01.2021', null),
                            (13, 'Ivan', 'Siroc', '0754298763', 8, '27.02.2023', null);


insert into employee_property values (1, 2, 7, 'toolmaker'),
                                     (2, 3, 6, 'thermite'),
                                     (3, 4, 4, 'motor'),
                                     (4, 9, 4, 'chassis'),
                                     (5, 1, 1, 'nsu'),
                                     (6, 5, 3, '5'),
                                     (7, 6, 2, 'smth'),
                                     (8, 7, 1, 'msu'),
                                     (9, 8, 5, '3'),
                                     (10, 10, 2, 'grade');


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

update department set chief = 1 where department.id = 1;
update department set chief = 10 where department.id = 2;

insert into department_region_chief values (1),
                                           (5);

insert into department_region values (1, 'First', 1, 1, 1),
                                     (2, 'Build', 2, 3, 5);

update brigade set department_region = 1 where id = 1;
update brigade set department_region = 2 where id = 2;
update brigade set department_region = 1 where id = 3;



insert into product_category values (1, 'motorbike'),
                                    (2, 'truck'),
                                    (3, 'passenger_car'),
                                    (4, 'bus'),
                                    (5, 'agricultural_car'),
                                    (6, 'road_construction_car');

insert into product_type_attribute values (1, 'engine_capacity', 1),
                                          (2, 'load_capacity', 2),
                                          (3, 'doors_number', 3),
                                          (4, 'capacity', 4),
                                          (5, 'performance', 5),
                                          (6, 'pressure', 6),
                                          (7, 'engine_capacity', 3);

insert into product_category_type values (1, 'Dodge Tomahawk', 1),
                                         (2, 'Suzuki Hayabusa', 1),
                                         (3, 'Yamaha YZF-R1', 1),
                                         (4, 'Ducati – 1098', 1),
                                         (5, 'kamaz-5490', 2),
                                         (6, 'Foton Auman', 2),
                                         (7, 'MAN TGX', 2),
                                         (8, 'Hyundai H350', 4),
                                         (9, 'Volvo 7900', 4),
                                         (10, 'Maz-251', 4),
                                         (11, 'BMW X5', 3),
                                         (12, 'Mercedes GT', 3),
                                         (13, 'Toyota Land Cruiser Prado', 3),
                                         (14, 'claas', 4),
                                         (15, 'New Holland', 4),
                                         (16, 'niva', 4),
                                         (17, 'Volvo Construction Equipment', 5),
                                         (18, 'Liebherr R920', 5),
                                         (19, 'Cat 320D2L', 5);

insert into product values (1, 12, 1, 'David Belov'),
                           (2, 5, 1, 'StroiMash'),
                           (3, 13, 2, 'Eduard Lomov'),
                           (4, 2, 2, 'Bloty');

insert into product_property values (1, 1, 3, '5'),
                                    (2, 2, 2, '20000'),
                                    (3, 3, 3, '5'),
                                    (4, 3, 7, '300');

insert into product_status values ('waiting'),
                                  ('assembling'),
                                  ('assembled');

insert into product_process values (1, 1, 'motor manufacturing', 2, 'assembled', '02.12.2021'),
                                   (2, 1, 'packing', 1, 'assembling', null),
                                   (3, 2, 'smth', 2, 'assembled', '02.03.2022'),
                                   (4, 2, 'smth2', 1, 'assembling', null),
                                   (5, 2, 'smth3', 1, 'waiting', null);

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