<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title> z4-1b </title>
</head>
<body>

<?php
$otvs = array("6", "9", "4", "1", "3", "2", "5", "8", "7");
$answers = $_GET['answers'];

$count = 0;
for($i = 0, $size = count($otvs); $i < $size; ++$i){
    if($otvs[$i] === $answers[$i]){
        ++$count;
    }
}
$username = $_GET['username'];
if(!isset($username)){
    $username = 'Аноним';
}

print "<p>" . $username . " вы ";
switch ($count){
    case 9:
        print "великолепно знаете географию";
        break;
    case 8:
        print "отлично знаете географию";
        break;
    case 7:
        print "очень хорошо знаете географию";
        break;
    case 6:
        print "хорошо знаете географию";
        break;
    case 5:
        print "удовлетворительно знаете географию";
        break;
    case 4:
        print "терпимо знаете географию";
        break;
    case 3:
        print "плохо знаете географию";
        break;
    case 2:
        print "очень плохо знаете географию";
        break;
    default:
        print "вообще не знаете географию";
}
print "</p>";
?>

</body>
</html>
