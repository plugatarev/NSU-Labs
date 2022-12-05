CREATE DATABASE IF NOT EXISTS Служащие;
use Служащие;

CREATE TABLE IF NOT EXISTS Сотрудник(
	Идентификатор	INT PRIMARY KEY AUTO_INCREMENT,
	Фамилия 		VARCHAR(100) NOT NULL,
    Имя 			VARCHAR(100) NOT NULL,
    Отчество 		VARCHAR(100),
    Адрес 			VARCHAR(200) NOT NULL
);

DROP TABLE Сотрудник;
truncate table Сотрудник;
select * from Сотрудник;

CREATE TABLE IF NOT EXISTS Должность(
	Название			VARCHAR(100) PRIMARY KEY,
    Почасовая_оплата 	INT CHECK (Почасовая_оплата >= 0) NOT NULL
);

DROP TABLE Должность;

CREATE TABLE IF NOT EXISTS Место_работы(
	Название_организации	VARCHAR(100) PRIMARY KEY,
    Адрес 					VARCHAR(200) NOT NULL,
	Телефон 				VARCHAR(11)  NOT NULL,
	Отчисления				TINYINT CHECK (Отчисления >= 0 AND Отчисления <= 100) NOT NULL
);

DROP TABLE Место_работы;

CREATE TABLE IF NOT EXISTS Работа(
	Идентификатор	INT PRIMARY KEY AUTO_INCREMENT,		
	Сотрудник 		INT,
    Место_работы 	VARCHAR(100),
    Должность 		VARCHAR(100),
	Дата_назначения DATE NOT NULL,
	Кол_часов		INT NOT NULL CHECK (Кол_часов > 0),
    FOREIGN KEY (Сотрудник) REFERENCES Сотрудник(Идентификатор) ON UPDATE CASCADE ON DELETE SET NULL,
    FOREIGN KEY (Место_работы) REFERENCES Место_работы(Название_организации) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (Должность) REFERENCES Должность(Название) ON UPDATE CASCADE ON DELETE SET NULL
);

DROP TABLE Работа;

#Заполнение корректными данными (Сотрудник)
INSERT INTO Сотрудник(Фамилия, Имя, Отчество, Адрес) VALUES ('Ivanov', 'Sergej', 'Petrovich', 'Morskoj prospekt 12');
INSERT INTO Сотрудник(Фамилия, Имя, Отчество, Адрес) VALUES ('Ustinov', 'Oleg', 'Viktorovich', 'Krasnyj prospekt 20');
INSERT INTO Сотрудник(Фамилия, Имя, Отчество, Адрес) VALUES ('Kim', 'Dmitrij', 'Konstantinovich', 'Ul Pirogova 20');
INSERT INTO Сотрудник(Фамилия, Имя, Отчество, Адрес) VALUES ('Markova', 'Ekaterina', 'Andreevna', 'Ul Furmanova 12');
INSERT INTO Сотрудник(Фамилия, Имя, Отчество, Адрес) VALUES ('Sidorov', 'Aleksej', 'Andreevich', 'Ul Geroev truda');
INSERT INTO Сотрудник(Фамилия, Имя, Отчество, Адрес) VALUES ('Akopjan', 'Vasilij', 'Ilich ', 'Ul Gogolja');

SET SQL_SAFE_UPDATES = 0;
SELECT * FROM Сотрудник;

INSERT INTO Должность (Название, Почасовая_оплата) VALUES ('Bujgalker',	25);
INSERT INTO Должность (Название, Почасовая_оплата) VALUES ('Inzhener', 40);
INSERT INTO Должность (Название, Почасовая_оплата) VALUES ('Programmist', 60);
INSERT INTO Должность (Название, Почасовая_оплата) VALUES ('Povar',	20);
INSERT INTO Должность (Название, Почасовая_оплата) VALUES ('Barmen', 15);
INSERT INTO Должность (Название, Почасовая_оплата) VALUES ('Menedzher',	70);

SELECT * FROM Должность;

INSERT INTO Место_работы (Название_организации, Адрес, Телефон, Отчисления) 
		VALUES ('Artehk', 'Novosibirsk Metro Krasnyj prospekt',	'23-23-45',	10);
        
INSERT INTO Место_работы (Название_организации, Адрес, Телефон, Отчисления) 
		VALUES ('Ajron', 'Moskva Taganskaja 9', '345-23-21', 15);
        
INSERT INTO Место_работы (Название_организации, Адрес, Телефон, Отчисления) 
		VALUES ('Gotti', 'Novosibirsk  Ploschad Kondratjuka', '34-23-12', 12);
        
INSERT INTO Место_работы (Название_организации, Адрес, Телефон, Отчисления) 
		VALUES ('Landor', 'Novosibirsk Mikrorajon Sch', '43-78-90',	10);
                

SELECT * FROM Место_работы;

INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (1, 'Artehk', 'Inzhener', '02-05-22', 8);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (2, 'Artehk', 'Bujgalker', '02-05-26', 6);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (3, 'Ajron', 'Programmist', '02-01-10', 8);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (4, 'Gotti', 'Povar', '02-05-10', 6);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (5, 'Gotti', 'Barmen', '02-01-20', 10);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (6, 'Landor', 'Menedzher', '02-01-30', 8);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (1, 'Artehk', 'Menedzher', '02-07-29', 8);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (3, 'Ajron', 'Menedzher', '02-12-17', 8);
SELECT * FROM Работа;

-- Данные для доп. запроса  
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (5, 'Landor', 'Barmen', '02-01-20', 10);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (5, 'Artehk', 'Barmen', '02-01-20', 10);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (5, 'Ajron', 'Barmen', '02-01-20', 10);

INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (1, 'Landor', 'Barmen', '02-05-22', 8);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (1, 'Gotti', 'Inzhener', '02-05-22', 8);
INSERT INTO Работа(Сотрудник, Место_работы, Должность, Дата_назначения, Кол_часов) VALUES (1, 'Ajron', 'Inzhener', '02-05-22', 8);
