CREATE DATABASE Прокат;
USE Прокат;
SET sql_mode = '' ;
CREATE TABLE IF NOT EXISTS Клиент(
	Идентификатор		INT PRIMARY KEY AUTO_INCREMENT,
	Фамилия 			VARCHAR(100) NOT NULL,
    Имя 				VARCHAR(100) NOT NULL,
    Отчество 			VARCHAR(100),
    Водительский_стаж  	INT CHECK(Водительский_стаж >= 0) NOT NULL
);

INSERT INTO Клиент(Идентификатор, Фамилия, Имя, Отчество, Водительский_стаж)
VALUES
	(1, 'Ivanov', 'Sergej', 'Petrovich', 1),
	(2, 'Ustinov', 'Oleg', 'Viktorovich', 10),
	(3, 'Kim', 'Dmitrij', 'Konstantinovich', 9),
	(4, 'Markova', 'Ekaterina', 'Andreevna', 25),
	(5, 'Sidorov', 'Aleksej', 'Andreevich', 15),
	(6, 'Akopjan', 'Vasilij', 'Ilich ', 10),
	(7, 'Ivanov', 'Aleksej', 'Viktorovich', 9),
	(8, 'Ustinov', 'Dmitrij', 'Petrovichh', 30);
	
  
INSERT INTO Клиент(Фамилия, Имя, Отчество, Водительский_стаж) VALUE ('Марков', 'Илья', 'Владимирович', 11);
SELECT * FROM Клиент;
DROP TABLE Клиент;

CREATE TABLE IF NOT EXISTS Автомобиль(
	Идентификатор	INT PRIMARY KEY AUTO_INCREMENT,
	Марка 			VARCHAR(100) NOT NULL,
    Год_выпуска		YEAR CHECK(Год_выпуска >= 1886 and Год_выпуска <= 2022) NOT NULL
);

INSERT INTO Автомобиль(Идентификатор, Марка, Год_выпуска)
VALUES
	(1, 'Lada', '1999'),
	(2, 'Hyundai', '2017'),
	(3, 'Toyota', '2013'),
	(4, 'Hyundai', '2021'),
	(5, 'Lada', '2018'),
	(6, 'Volga', '2015'),
	(7, 'Volga', '2020'),
	(8, 'Volvo', '2012'),
	(9, 'Hyundai', '2000'),
	(10, 'Toyota', '2010'),
	(11, 'Volga', '2020'),
	(12, 'Lada', '2011'),
	(13, 'Hyundai', '2000'),
	(14, 'Toyota', '2010');
    
INSERT INTO Автомобиль(Марка, Год_выпуска) VALUE ('Ford', 2012);
INSERT INTO Автомобиль(Марка, Год_выпуска) VALUE ('Lamborghini', 2021);
INSERT INTO Автомобиль(Марка, Год_выпуска) VALUE ('AUDI', 2015);

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
	(1, 1, 500),
	(8, 9, 4500),
	(5, 5, 1000),
	(2, 2, 5500),
	(5, 7, 6800),
	(2, 6, 3400),
	(3, 5, 1000),
	(5, 9, 600),
	(6, 3, 800),
	(1, 7, 3400),
	(6, 10, 3500),
	(3, 5, 4500),
	(7, 1, 5000),
	(8, 4, 8000),
	(2, 9, 10000),
	(5, 9, 10000),
	(3, 3, 2500),
	(1, 11, 4800),
	(6, 10, 3200),
	(3, 5, 3600),
	(7, 1, 5700),
	(8, 4, 3100),
	(2, 12, 3700),
	(5, 9, 4200),
	(3, 14, 2000),
	(3, 5, 200);
    
INSERT INTO Прокат(Клиент, Автомобиль, Стоимость) VALUE (3, 3, 400);
    

SELECT * FROM Прокат;
DROP TABLE Прокат;

SELECT Клиент.Фамилия FROM Клиент GROUP BY Клиент.Фамилия HAVING count(*) > 1 ORDER BY Клиент.Фамилия;


SELECT DISTINCT Автомобиль.Марка FROM Прокат, Автомобиль WHERE Автомобиль.Идентификатор = Прокат.Автомобиль;

-- на какой машине больше всего заработали
CREATE VIEW Заработок as
(SELECT Автомобиль.Марка as Марка, SUM(Прокат.Стоимость) as Сумма
FROM Прокат, Автомобиль 
WHERE Прокат.Автомобиль = Автомобиль.Идентификатор
GROUP BY Автомобиль.Идентификатор);

SELECT * FROM Заработок WHERE Заработок.Сумма = (SELECT MAX(Сумма) FROM Заработок);

-- Клиент, потративший больше всего денег

-- Клиент сколько раз арендовал тачку
SELECT Клиент.Фамилия, Клиент.Имя, Клиент.Отчество, COUNT(*)
FROM Клиент, Прокат WHERE Клиент.Идентификатор = Прокат.Клиент
GROUP BY Клиент.Фамилия, Клиент.Имя, Клиент.Отчество;

-- Среднее стоимость по машине в прокате
SELECT Автомобиль.Марка, AVG(Прокат.Стоимость) as Среднее
FROM Автомобиль, Прокат WHERE Автомобиль.Идентификатор = Прокат.Автомобиль
GROUP BY Автомобиль.Марка
ORDER BY Среднее DESC;

-- Какую машину чаще всего брали в прокат
CREATE VIEW Количество as
(SELECT Автомобиль.Марка as Марка, COUNT(*) as Число
FROM Автомобиль, Прокат WHERE Автомобиль.Идентификатор = Прокат.Автомобиль
GROUP BY Автомобиль.Марка);

SELECT Марка, Число
FROM Количество WHERE Количество.Число = (SELECT MAX(Число) FROM Количество);

-- -------------------------------------------------------------------------------------------------------------------------- 
-- Вариант запросов 1
-- Вариант 1
-- 1. Автомобили, которые не брали в прокат.
-- Формат вывода: ID, марка, год выпуска
-- 2. Количество прокатов со стоимостью > X
-- Формат вывода: количество
-- 3. Самые популярные марки автомобилей для проката.
-- Формат вывода: марка, количество
-- 4. Для каждого клиента суммарную стоимость всех его прокатов
-- Формат вывода: ID, ФИО, сумма
-- 5. Все клиенты, стаж которых > X лет.
-- Формат вывода: ID, ФИО, стаж

-- 1 запрос
SELECT Автомобиль.Идентификатор, Автомобиль.Марка, Автомобиль.Год_выпуска
FROM Автомобиль LEFT JOIN Прокат ON Автомобиль.Идентификатор = Прокат.Автомобиль
GROUP BY Автомобиль.Идентификатор
HAVING COUNT(Прокат.Идентификатор) = 0;

-- 2 запрос
SELECT COUNT(*)	
FROM Прокат 
WHERE Прокат.Стоимость > 5000;

-- 3 запрос
CREATE VIEW Количество as
(SELECT Автомобиль.Марка as Марка, COUNT(*) as Число
FROM Автомобиль, Прокат WHERE Автомобиль.Идентификатор = Прокат.Автомобиль
GROUP BY Автомобиль.Марка);

SELECT Марка, Число
FROM Количество WHERE Количество.Число = (SELECT MAX(Число) FROM Количество);

-- 4 запрос
SELECT Клиент.Идентификатор, Клиент.Фамилия, Клиент.Имя, Клиент.Отчество, COALESCE(SUM(Прокат.Стоимость), 0) as Сумма_проката
FROM Клиент LEFT JOIN Прокат ON Клиент.Идентификатор = Прокат.Клиент
GROUP BY Клиент.Идентификатор;

-- 5 запрос 
SELECT * 
FROM Клиент
WHERE Клиент.Водительский_стаж > 24;