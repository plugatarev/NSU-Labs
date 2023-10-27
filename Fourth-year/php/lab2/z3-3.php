<html lang="ru">
<head>
    <meta charset="utf-8">
    <title> z3-3 </title>
</head>
<body>

<?php
function Ru($color){
    print "<p style='color: $color'>Здравствуйте!</p>\n";
}

function En($color){
    print "<p style='color: $color'>Hello!</p>\n";
}

function Fr($color){
    print "<p style='color: $color'>Bonjour!</p>\n";
}

function De($color){
    print "<p style='color: $color'>Guten Tag!</p>\n";
}

$lang = $_GET['lang'];
$color = $_GET['color'];

if (!isset($lang)) {
    print "Lang is not defined";
} elseif (!isset($color)) {
    print "Color is not defined";
} else {
    $lang($color);
}

?>
</body>
</html>
