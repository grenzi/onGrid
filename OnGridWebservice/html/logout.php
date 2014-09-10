
<?php 
session_start();
if($_SESSION["status"]="logged") {
	session_unset(); 
	session_destroy();
}
header("location: index.php");

