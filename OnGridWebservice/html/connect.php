<?php

$server = "localhost";
$user = "user_name";
$password = "password";
$database = "db_name";

$con = mysql_connect($server, $user, $password);
if (!$con) {
  die('Could not connect.');
}

mysql_select_db($database) or die('Could not find database.');

?>
