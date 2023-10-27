<html lang="ru">
<head>
    <meta charset="utf-8">
    <title> z2-5 </title>
</head>
<body>

<?php

if (!isset($_GET['lang'])){
    print "Значение lang не установлено" . "<br>\n";
}

$lang = $_GET['lang'];

if ($lang == "ru"){
    print "Русский язык\n";
} elseif ($lang == "en"){
    print "Английский язык\n";
} elseif ($lang == "fr"){
    print "Французский язык\n";
} elseif ($lang == "de"){
    print "Немецкий язык\n";
} else{
    print "Язык неизвестен\n";
}
?>

</body>
</html>
