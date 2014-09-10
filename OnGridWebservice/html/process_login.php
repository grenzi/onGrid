<?php


if(!defined('MyConst')){die('Direct access not permitted');}

include 'connect.php';

$email = $_POST["email"];
$code = $_POST["code"];

if($email==NULL || $code==NULL){
	echo "Please provide your email and code to access your job management.<br>"; 
	echo "If you have not registered, <a href='accessnow.php'>click here to do so.</a>";
	die();
}

$query1 = mysql_query("SELECT * FROM wkdd_users WHERE email='$email'");
if(!$query1){
	die("There is some problem: " . mysql_error());
}
$email_number = mysql_num_rows($query1);
if($email_number<1){
	echo "The email addresses you entered is not in our system.<br />";
	echo "<a href='accessnow.php'>Click here to return and register for an access code.</a>";
	die();
}

$query2 = mysql_query("SELECT * FROM wkdd_users WHERE email='$email' AND code='$code'");
if(!$query2){
	die("There is some problem: " . mysql_error());
}
$code_test = mysql_num_rows($query2);
if($code_test<1){
	echo "The Access Code you entered is not correct.<br />";
	echo "<a href='javascript:history.go(-1)'>Return to the previous page and try again</a><br>";
	die();
}

$row = mysql_fetch_assoc($query2);
if($row['approved']=='no'){
	echo "We are sorry, but your job management function has not been activated.<br />";
	echo "Please wait for a confirmation mail from us, or you are welcome to contact our admin.";
	die();
}

echo "<h9>Welcome back, ";
if($row['firstname']!=NULL){
	echo $row['firstname'] . "!</h9><br>";}
else{
	echo $row['email'] . "!</h9><br>";}
echo "<h9><a href='myaccount.php'>Please click here to manage your jobs and information</a></h9>";

$_SESSION["email"] = $email;
$_SESSION["name"] = $row["firstname"];
$_SESSION["status"]="logged";

mysql_close($con);

?>
