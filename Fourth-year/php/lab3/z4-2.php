<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title> z4-2 </title>
</head>
<style type="text/css">
    .temp{
        border: 1px solid black;
        width: 100px;
        height: 100px;
    }
</style>
<body>

<?php
@$align = $_GET['align'];
@$valign = $_GET['valign'];

if(!isset($align)){
    print $align;
    $align="left";
}

if(!isset($valign)){
    print $valign;
    $valign="top";
}

if(isset($align) && isset($valign)){
    print "<table style='border-collapse: separate; table-layout: fixed'>\n";
    print "<tr>\n\t<td class='temp' style='text-align: $align; vertical-align: $valign;'>Текст</td>\n</tr>\n";
    print "</table>";
}

?>

<form action="<?php print $_SERVER['PHP_SELF']; ?>" method="GET">
    <h3>Выберите горизонтальное расположение</h3>
    <p><label><input type="radio" name="align" value="left"></label>слева</p>
    <p><label><input type="radio" name="align" value="center"></label>по центру</p>
    <p><label><input type="radio" name="align" value="right"></label>справа</p>

    <h3>Выберите вертикальное положение</h3>
    <p><label><input type="checkbox" name="valign" value="top"></label>сверху</p>
    <p><label><input type="checkbox" name="valign" value="middle"></label>по середине</p>
    <p><label><input type="checkbox" name="valign" value="bottom"></label>внизу</p>

    <p><input type="submit" value="Выполнить"></p>
</form>

</body>
</html>
