<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>Задание №1</title>
</head>
<body>
	<?php

		$mysqli_user = "root";
		$mysqli_password = "root";
		$conn = mysqli_connect("127.0.0.1", $mysqli_user, $mysqli_password);
		if(!$conn) 
			die("Нет соединения с MySQL");

		$resultSelectDB = mysqli_select_db($conn,"sample");

		if(!$resultSelectDB)
			die("<p>Не удалось выбрать базу данных</p>" . mysqli_error($conn));

		$queryDropTable = "DROP TABLE IF EXISTS notebook_br06";

		$resultDropTable = mysqli_query($conn, $queryDropTable);

		if(!$resultDropTable)
			die("<p>Нельзя уничтожить таблицу notebook_br06</p>" . mysqli_error($conn));

		$queryCreateTable = "CREATE TABLE notebook_br06 (id INT NOT NULL AUTO_INCREMENT, PRIMARY KEY(id), name VARCHAR(50), city VARCHAR(50), address VARCHAR(50), birthday DATE, mail VARCHAR(50))";

		$resultCreateTable = mysqli_query($conn, $queryCreateTable);

		if(!$resultCreateTable)
			die("<p>Нельзя создать таблицу notebook_br06</p>" . mysqli_error($conn));

	?>
</body>
</html>