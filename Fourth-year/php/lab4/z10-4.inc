<?php
$user = "root";
$pass = "diplomnsu";
$db = "info_search";

$conn = mysqli_connect("localhost", $user, $pass);
if (!$conn) die("Нет соединения с MySQL");

mysqli_select_db($conn, $db) or die ("Нельзя открыть $db");
?>