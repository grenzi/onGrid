<?php

if(!defined('MyConst')){die('Direct access not permitted');}


if ( !isset($_SESSION["status"]) || !isset($_SESSION["email"]) || !isset($_SESSION["name"])) {
	echo "<form class='form-inline' method='post' action='login.php'>
  		<input type='text' class='input' placeholder='Email' id='email' name='email'>
 		<input type='text' class='input' placeholder='Access Code' id='code' name='code'>
  		<button type='submit' class='btn btn-small'>Check Job Status</button>
		</form> ";
}
else {
	if ($_SESSION["name"]==NULL){
		echo "Welcome!&nbsp&nbsp&nbsp&nbsp";
	}
	else {
		echo "Welcome, " . $_SESSION["name"] . "!&nbsp&nbsp&nbsp&nbsp";
	}
	echo "<a href='myaccount.php'>Job Management</a>";
	echo "&nbsp&nbsp&nbsp&nbsp";
	echo "<a href='logout.php'>Exit</a>";
}
?>
