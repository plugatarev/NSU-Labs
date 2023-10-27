<html>
<head>
    <title> Листинг 10-7 </title>
</head>

<body>

<?php
$PARAMS = (isset($_POST)) ? $_POST : $_GET;

foreach ($PARAMS as $key=>$value) {

    if (gettype($value) == "array") {
        print "$key = <br>\n";

        foreach ($value as $v)
            print "$v<br>";
    }
    else {
        print "$key = $value<br>\n";
    }
}
?>

</body>
</html>

