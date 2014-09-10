<?php
//After the admin approved one application, database is updated, a confirmation email is sent to user then.

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

// generate an acess code, first three letters of email + three random numbers
$random = substr(number_format(time() * rand(),0,'',''),0,3);
$position = strpos($email, "@");
$initial = substr($email, 0, $position);
$code = $initial . $random;

//approve the user and update database
$query = mysql_query("UPDATE wkdd_users SET approved='yes' WHERE email='$email'");
if(!$query){
	die("There is some problem: " . mysql_error());
}

$query = mysql_query("UPDATE wkdd_users SET code='$code' WHERE email='$email'");
if(!$query){
	die("There is some problem: " . mysql_error());
}

//send confirmation email to the user
$link = "www.my_homepage.com";
$message = "Thanks for choosing Knowledge Discovery UMLS!\r\n" . "Your Access Code is: " . $code . "\r\nPlease Click here to get started: " . $link . "\r\nThank you!";
;
//Please edit system's email, and change netlab@osumc.edu to our future email address
$headers = 'From: my_email' . "\r\n" . 'Reply-To: my_reply_email' . "\r\n" . 'X-Mailer: PHP/' . phpversion();

//Please edit user's email address below, and change 'jxnmany@gmail.com" to $email, have been changed!
$sendmail = mail($email, 'Knowledge Discovery UMLS: Your Access Code!', $message, $headers);
if($sendmail){
	echo "You have successfully approved this application!<br>";
	echo "Confirmation email with an Access Code is sent to the user! You may close this page now.<br>";
}else{
	echo "Error in sending a confirmation email to the user.";
}

mysql_close($con);

?>

