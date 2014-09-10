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
if($row['emailverified']=='yes'){
	echo "You have already confirmed your information.";
	die();
}

mysql_close($con);

echo "User informarion:" . "<br>";
echo "Email: " . $row['email'] . "<br>";
echo "Description of Specific Requirement: " . $row['description'] . "<br>";
echo "User Name: " . $row['firstname'] . " " . $row['lastname'] , "<br>";
echo "Title: " . $row['title'] . "<br>";
echo "Institution: " . $row['institution'] . "<br>";
echo "Department: " . $row['department'] . "<br>";


//This will update the database, to set the user status approved
echo "<a href='emailverify.php?email=" . $email . "&&request=" . $request .  "'>Please click here to confirm your information.</a> Your request will be processed after the netlab administrator receives your confirmation."
?>

