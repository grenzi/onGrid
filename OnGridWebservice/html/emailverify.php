<?php
//After the admin approved one application, database is updated, a confirmation email is sent to user then.
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
	


// Update the database to set the user email verified
   $query = mysql_query("UPDATE wkdd_users SET emailverified='yes' WHERE email='$email'");
   if(!$query){
	   die("There is some problem: " . mysql_error());
   }
   echo "You have successfully confirmed your information!<br>";


//send request email to the netlab admin
$link = "approve.php?email=" . $email . "&&request=" . $request;
$message = "A new access request has been received as follows: \r\n" . "Name: " . $row['firstname'] . " " . $row['lastname'] . "\r\nTitle: " . $row['title'] . "\r\nEmail (verified): " . $row['email'] . "\r\nDescription: " . $row['description']. "\r\nInstitution: " . $row['institution'] . "\r\nDepartment: " . $row['department'] . "\r\nClick here to approve it: " . $link . "\r\nThank you!";
;
//Please edit system's email, and change netlab@osumc.edu to our future email address
$headers = 'From: my_email' . "\r\n" . 'Reply-To: my_reply_email' . "\r\n" . 'X-Mailer: PHP/' . phpversion();

//Please edit user's email address below, and change 'jxnmany@gmail.com" to $email, have been changed!
$sendmail = mail('my_email', 'onGrid: new request', $message, $headers);
if($sendmail){
	echo "A request email is sent to the netlab admin.<br>";
	echo "You will receive an Access Code once your request is approved.<br>";
	echo "You may close the page or <a href='index.php'>click here to visit our homepage.</a>";
}else{
	echo "Error in sending a request email to the netlab admin.;
}

	mysql_close($con);
?>

