<?php

$mysqli_user = "root";
$mysqli_password = "root";

$conn = mysqli_connect("127.0.0.1", $mysqli_user, $mysqli_password);

if ($conn->connect_error) {
    die('Ошибка подключения: ' . $conn->connect_error);
}

$insertQueries = [
    "INSERT INTO notebook (name, city, address, birthday, mail) VALUES ('Иванов', 'Москва', 'ул. Ленина, 123', '1990-05-15', 'ivanov@example.com')",
    "INSERT INTO notebook (name, city, address, birthday, mail) VALUES ('Петров', 'Санкт-Петербург', 'пр. Невский, 456', '1985-08-20', 'petrov@example.com')",
    "INSERT INTO notebook (name, city, address, birthday, mail) VALUES ('Сидоров', 'Екатеринбург', 'ул. Свердлова, 789', '1995-02-10', 'sidorov@example.com')"
];

foreach ($insertQueries as $query) {
    if ($conn->query($query) === TRUE) {
        echo "Запись успешно добавлена.<br>";
    } else {
        echo "Ошибка при добавлении записи: " . $conn->error . "<br>";
    }
}

$conn->close();
?>
