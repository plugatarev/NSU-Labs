<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>Задание №4</title>
</head>
<body>
			<?php

				$mysqli_user = "root";
				$mysqli_password = "root";
				$conn = mysqli_connect("127.0.0.1", $mysqli_user, $mysqli_password);
				if ($conn->connect_error) {
                    die('Ошибка подключения: ' . $conn->connect_error);
                }

				$resultSelectDB = mysqli_select_db($conn,"sample");

				if(!$resultSelectDB)
					die("<p>Не удалось выбрать базу данных</p>" . mysqli_error($conn));

				if (isset($_POST['id']) && isset($_POST['field_name'])) {
					$id = $_POST['id'];
					$field_name = $_POST['field_name'];

					if (!isset($_POST['field_value'])  || empty($_POST['field_value'])) {
						$field_value = "NULL";
					}else{
						$field_value = "'" . $_POST['field_value'] . "'";
					}

					$queryUpdate = "UPDATE notebook_br06 SET $field_name=$field_value WHERE id = $id";

					$resultUpdate = mysqli_query($conn, $queryUpdate);

					if(!$resultUpdate)
						die("<p>Не удалось изменить запись в таблице notebook_br06</p>" . mysqli_error($conn));
					print "<p>Успешно изменено</p>";
					print "<p><a href=\"z09-3.php\">Вывести все записи таблицы</a></p>";
				}else if (!isset($_POST['id'])) {
					
					$script = $_SERVER['PHP_SELF'];
					print "<form action=\"$script\" method=\"POST\">
								<table cellpadding=0 border=1>
									<tr>
										<td>Имя</td>
										<td>Город</td>
										<td>Адрес</td>
										<td>Дата рождения</td>
										<td>E-Mail</td>
										<td>Заменить</td>
									</tr>";

					$querySelect = "SELECT id, name, city, address, birthday, mail FROM `notebook_br06`";
					
					$resultSelect = mysqli_query($conn, $querySelect);

					if(!$resultSelect)
						die("<p>Не удалось выбрать записи из таблицы notebook_br06</p>" . mysqli_error($conn));


					foreach ($resultSelect as $row) {
						print "<tr>";
						print "<td>" . $row['name'] . "</td>";
						print "<td>" . $row['city'] . "</td>";
						print "<td>" . $row['address'] . "</td>";
						print "<td>" . $row['birthday'] . "</td>";
						print "<td>" . $row['mail'] . "</td>";
						print "<td><input type=\"radio\" name=\"id\" value=\"" . $row['id'] . "\"></td>";
						print "</tr>";
					}

					print "	</table>
							<p><button type=\"submit\">Выбрать</button></p>
							</form>";

				}else if (isset($_POST['id'])) {
					$id = $_POST['id'];
				

					$querySelect = "SELECT id, name, city, address, birthday, mail FROM `notebook_br06` WHERE id = $id";

					$resultSelect = mysqli_query($conn, $querySelect);

					if(!$resultSelect)
						die("<p>Не удалось выбрать запись из таблицы notebook_br06</p>" . mysqli_error($conn));
					
					$row = mysqli_fetch_row($resultSelect);
					$script = $_SERVER['PHP_SELF'];

					print "<form action=\"$script\" method=\"POST\">";
					print "<p><select name=\"field_name\">
						    <option value=\"name\" selected>$row[1]</option>
						    <option value=\"city\">$row[2]</option>
						    <option value=\"address\">$row[3]</option>
						    <option value=\"birthday\">$row[4]</option>
						    <option value=\"mail\">$row[5]</option>
						   </select>
						   <input type=\"text\" name=\"field_value\"></p>";
					print "<input type=\"hidden\" name=\"id\" value=\"$id\">";
					print "<button type=\"submit\">Заменить</button>";
					print "</form>";
				}
			?>

</body>
</html>