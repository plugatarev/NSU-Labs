<html lang="ru">
<head>
    <meta charset="utf-8">
    <title> z3-1 </title>
</head>
<body>
<?php
$i = 1;
#Цвет ячеек главной диагонали
$color = "grey";

print "<table style='border-collapse: separate;'>\n";
while ($i <= 10){
    $j = 1;
    print "<tr>\n";

    while ($j <= 10){
        $cur_color = ($i == $j) ? $color : "white";
        print "\t<td style='border: 1px solid black; padding: 5px; background-color: $cur_color'>" . ($i * $j) . "</td>\n";
        ++$j;
    }

    print "</tr>\n";
    ++$i;
}
print "</table>";
?>
</body>
</html>
