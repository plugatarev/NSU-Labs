<html lang="ru">
<head>
    <meta charset="utf-8">
    <title> z3-5 </title>
</head>
<body>

<?php

function printArray($num, $arr)
{
    print "<div>" . $num . ") ";
    for ($i = 0, $count = count($arr); $i <= $count; ++$i){
        if (isset($arr[$i])) {
            print $arr[$i] . "  ";
        }
    }
    print "</div>\n";
}

$treug = array();
print "<div style='white-space: pre'>1) ";
for ($i = 1; $i <= 10; ++$i){
    $treug[$i] = $i * ($i + 1) / 2;
    print $treug[$i] . "  ";
}
print "</div>\n";

$kvd = array();

for($i = 1; $i <= 10; ++$i){
    $kvd[$i - 1] = $i * $i;
}

printArray(2, $kvd);

$rez = array_merge($treug, $kvd);
printArray(3, $rez);

sort($rez);
printArray(4, $rez);

array_shift($rez);
printArray(5, $rez);

$rez1 = array_unique($rez);
printArray(6, $rez1);

?>

</body>
</html>
