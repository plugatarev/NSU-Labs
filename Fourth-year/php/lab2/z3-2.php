<html lang="ru">
<head>
    <meta charset="utf-8">
    <title> z3-2 </title>
</head>
<body>
<?php
$i = 1;
$color = "blue";

print "<table style='border-collapse: separate;'>\n";

while ($i <= 10){
    $j = 1;
    print "<tr>\n";

    while ($j <= 10){
        $cur_color = "black";
        $cur_value = 0;

        if($i == 1 && $j == 1){
            $cur_value = "+";
            $cur_color = "red";
        }elseif ($i == 1){
            $cur_value = $j;
            $cur_color = $color;
        }
        elseif ($j == 1){
            $cur_value = $i;
            $cur_color = $color;
        }else{
            $cur_value = $i + $j;
        }

        print "\t<td style='border: 1px solid black; padding: 5px; color: $cur_color'>" . $cur_value . "</td>\n";
        ++$j;
    }

    print "</tr>\n";
    ++$i;
}
print "</table>";
?>
</body>
</html>
