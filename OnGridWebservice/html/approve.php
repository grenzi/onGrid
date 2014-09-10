<?php
//This is the page where admin can approve an application (after receiving the request email)
session_start(); 

include 'connect.php';

$email = $_GET["email"];
$request = $_GET["request"];

$query = mysql_query("SELECT * FROM wkdd_users WHERE email='$email'");
if(!$query){
	die("There is some problem: " . mysql_error());
}
$record_number = mysql_num_rows($query);
if($record_number !=1 ){
	echo "Error in retrieving the user information.<br>";
	die();
}   
$row = mysql_fetch_assoc($query);
if($row['request']!=$request){
	echo "Error: unmatched request number.<br>";
	die();
}
if($row['approved']=='yes'){
	echo "The request has already been approved";
	die();
} 

echo "User informarion:" . "<br>";
echo "User ID: " . $row['id'] . "<br>";
echo "Email: " . $row['email'] . "<br>";
echo "Description: ". $row['description'] . "<br>";
echo "User Name: " . $row['firstname'] . " " . $row['lastname'] , "<br>";
echo "Title: " . $row['title'] . "<br>";
echo "Institution: " . $row['institution'] . "<br>";
echo "Department: " . $row['department'] . "<br>";


//This will update the database, to set the user status approved
echo "<a href='confirm.php?email=" . $email . "&&request=" . $request .  "'>Please click here to approve this application and send the Access Code to this user.</a>";

mysql_close($con);

?>

