<?php
$user = "root";
$pass = "root";
$db = "info";

$conn = mysqli_connect("127.0.0.1", $user, $pass);
if (!$conn) die("Нет соединения с MySQL");

mysqli_select_db($conn, $db) or die ("Нельзя открыть $db");
?>