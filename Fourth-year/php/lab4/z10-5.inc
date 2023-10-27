<?php

function h_type2txt($type_id) {
    static $types;

    if (!isset($types))
    {
        $types = array();
        $constants = get_defined_constants(true);
        foreach ($constants['mysqli'] as $c => $n) if (preg_match('/^MYSQLI_TYPE_(.*)/', $c, $m))
            $types[$n] = $m[1];
    }

    return array_key_exists($type_id, $types)? $types[$type_id] : NULL;
}

function h_flags2txt($flags_num) {
    static $flags;

    if (!isset($flags))
    {
        $flags = array();
        $constants = get_defined_constants(true);
        foreach ($constants['mysqli'] as $c => $n) if (preg_match('/MYSQLI_(.*)_FLAG$/', $c, $m))
            if (!array_key_exists($n, $flags)) $flags[$n] = $m[1];
    }

    $result = array();
    foreach ($flags as $n => $t) if ($flags_num & $n) $result[] = $t;
    return implode(' ', $result);
}

function vid_structure($table) {
    global $conn;
    print "<p><h4>Структура таблицы $table</h4></p>";

    $query_res = mysqli_query($conn, "SELECT * FROM $table");
    $num_fields = mysqli_num_fields($query_res);

    print "<dl><dd>\n";
    for ($x=0; $x<$num_fields; $x++) {
        $info = mysqli_fetch_field_direct($query_res, $x);
        print "<i>";
        print h_type2txt($info->type);

        print "</i><i>";
        print " $info->length ";

        print "</i><b>";
        print " $info->name ";

        print "</b><i>";
        print h_flags2txt($info->flags);

        print "</i><br>\n";
    }
    print "</dl>\n";
}

function vid_content($table) {
    global $conn;
    print "<p><h4>Содержимое таблицы $table</h4></p>";

    $rus_name["snum"] = "номер продавца";
    $rus_name["sname"] = "имя продавца";
    $rus_name["city"] = "город";
    $rus_name["comm"] = "комиссионные продавца";
    $rus_name["cnum"] = "номер покупателя";
    $rus_name["cname"] = "имя покупателя";
    $rus_name["rating"] = "рейтинг покупателя";
    $rus_name["onum"] = "номер заказа";
    $rus_name["amt"] = "сумма заказа";
    $rus_name["odate"] = "дата";

    $result = mysqli_query($conn, "SELECT * FROM $table");
    $num_fields = mysqli_num_fields($result);

    print "<p><table border=1>\n";
    print "<tr>\n";
    for ($x=0; $x < $num_fields; $x++) {
        $name = mysqli_fetch_field_direct($result, $x);
        print "\t<th>" . $rus_name[$name->name] ."<br>$name->name\n";
    }
    while ($a_row = mysqli_fetch_row($result)) {
        print "<tr>\n";
        foreach ($a_row as $field)
            print "\t<td>$field</td>\n";
        print "</tr>\n";
    }
    print "</tr>\n";
    print "</table>\n";
}

if(isset($_GET["structure"])) {
    foreach($_GET["structure"] as $table) {
        vid_structure($table);
    }
}

if(isset($_GET["content"])) {
    foreach($_GET["content"] as $table) {
        vid_content($table);
    }
}

print "<p align=center><a href='z10-1.html'>Возврат к выбору таблицы</a>";
?>