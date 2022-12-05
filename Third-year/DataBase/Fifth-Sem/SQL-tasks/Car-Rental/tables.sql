CREATE DATABASE Прокат;
USE Прокат;

CREATE TABLE IF NOT EXISTS Клиент(
	Идентификатор		INT PRIMARY KEY AUTO_INCREMENT,
	Фамилия 			VARCHAR(100) NOT NULL,
    Имя 				VARCHAR(100) NOT NULL,
    Отчество 			VARCHAR(100),
    Водительский_стаж  	INT CHECK(Водительский_стаж >= 0) NOT NULL
);

INSERT INTO Клиент(Фамилия, Имя, Отчество, Водительский_стаж)
VALUES
	('Адамович', 'Татьяна', 'Владимировна', 10),
    ('Алёшин', 'Владимир', 'Вениаминович', 5),
    ('Худин', 'Петр', 'Сергеевич', 7),
    ('Шиповалов', 'Александр', 'Евгеньевич', 1),
    ('Белавин', 'Александр', 'Абрамович', 15),
    ('Беляев', 'Михаил', 'Геннадьевич', 2),
    ('Беляев', 'Михаил', 'Сергеевич', 10),
    ('Бурнашев', 'Марат', 'Валиевич', 7),
    ('Витушко', 'Михаил', 'Анатольевич', 12),
    ('Воронов', 'Дмитрий', 'Анатольевич', 4);	
    
SELECT * FROM Клиент;
DROP TABLE Клиент;

CREATE TABLE IF NOT EXISTS Автомобиль(
	Идентификатор	INT PRIMARY KEY AUTO_INCREMENT,
	Марка 			VARCHAR(100) NOT NULL,
    Год_выпуска		INT NOT NULL
);

INSERT INTO Автомобиль(Марка, Год_выпуска)
VALUES
	('AUDI', 2009),
    ('PORSHE', 2021),
    ('Lada', 2001),
    ('BMW', 2020),
    ('Mersedes', 2015),
    ('Toyota', 2010);
    
SELECT * FROM Автомобиль;
DROP TABLE Автомобиль;
    

CREATE TABLE IF NOT EXISTS Прокат(
	Идентификатор	INT PRIMARY KEY AUTO_INCREMENT,
	Клиент 			INT NOT NULL,
    Автомобиль		INT NOT NULL,
    Стоимость 		INT CHECK(Стоимость > 0) NOT NULL,
    
    FOREIGN KEY (Клиент) REFERENCES Клиент(Идентификатор) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (Автомобиль) REFERENCES Автомобиль(Идентификатор) ON UPDATE CASCADE ON DELETE CASCADE
);

INSERT INTO Прокат(Клиент, Автомобиль, Стоимость)
VALUES
	(1, 2, 700),
    (1, 3, 100),
    (2, 2, 1000),
    (3, 4, 500),
    (4, 5, 800),
    (6, 6, 900),
    (7, 1, 740),
    (7, 4, 510),
    (8, 6, 860),
    (9, 2, 990),
    (10, 3, 302);
    
SELECT * FROM Прокат;
DROP TABLE Прокат;
