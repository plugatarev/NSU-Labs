<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title> z4-1b </title>
    <style type="text/css">
        .temp{
            border: 1px solid black;
            width: 100px;
            height: 100px;
        }
    </style>
</head>
<body>

<?php
@$align = $_GET['align'];
@$valign = $_GET['valign'];

if(!isset($align)){
    print "align is not defined";
}

if(!isset($valign)){
    print "valign is not defined";
}

if(isset($align) && isset($valign)){
    print "<table style='border-collapse: separate; table-layout: fixed'>\n";
    print "<tr>\n\t<td class='temp' style='text-align: $align; vertical-align: $valign;'>Текст</td>\n</tr>\n";
    print "<tr>\n\t<td style='text-align: center;'><a href='z4-1a.html'>Назад</a></td>\n</tr>\n";
    print "</table>";
}
?>

</body>
</html>

