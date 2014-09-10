<!DOCTYPE html>
<?php
  //This page works after user has submitted an access request
  //An request email will send to admin to let he/she exmaine the application/approve it
  //Please edit specific email address accoding to the commentss below:
  session_start(); 
  define('MyConst', TRUE);
?>
<html lang='en'>
  <head>
    <meta charset='utf-8'>
    <title>Thank you!</title>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta name='description' content=''>
    <meta name='author' content=''>

    <!-- Le styles -->
    <link href='./assets/css/bootstrap.css' rel='stylesheet'>
    <style type='text/css'>
      body {
        padding-top: 10px;
        padding-bottom: 40px;
      }
      .sidebar-nav {
        padding: 9px 0;
      }
    </style>
    <link href='./assets/css/bootstrap-responsive.css' rel='stylesheet'>
    <link href='./assets/css/docs.css' rel='stylesheet'>
       <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src='http://html5shim.googlecode.com/svn/trunk/html5.js'></script>
    <![endif]-->

     <!--[if IE]>
      <script type='text/javascript'>document.createElement('h7');</script>
      <script type='text/javascript'>document.createElement('h8');</script>
      <script type='text/javascript'>document.createElement('h9');</script>
      <script type='text/javascript'>document.createElement('h10');</script>
      <script type='text/javascript'>document.createElement('h11');</script>
      <script type='text/javascript'>document.createElement('h12');</script>
      <script type='text/javascript'>document.createElement('h13');</script>
      <script type='text/javascript'>document.createElement('h14');</script>
      <script type='text/javascript'>document.createElement('h15');</script>
      <script type='text/javascript'>document.createElement('h16');</script>
      <script type='text/javascript'>document.createElement('h17');</script>
    <![endif]-->

  </head>

  <body>
    <div class='topban'>
      <div class='headline'>
        <h7>Online conceptual study platform using Graph indexing</h7>
        <hr width=100% size=4 color='white' style='margin-left:20px;'>
      </div>
    </div>
    <div class='container-fluid'>
      <div class='row-fluid'>
        <div class='span2'>
          <div class='bs-docs-sidebar'>
            <ul class='nav nav-list bs-docs-sidenav'>
              <li><a href='index.php'>Home</a></li>
		          <li><a href='tutorial.php'>About</a></li>
              <li><a href='discovery.php'>Start Discovery</a></li>
		          <li><a href='#'>Publications</a></li>
              <li><a href='terms.php'>Term of Use</a></li>
              <li><a href='contact.php'>Contact</a></li>
            </ul>
            <div class='bottomlogo'>
              <?php include 'side_logos.php' ?>
            </div>
          </div><!--/.well -->
        </div>   
        <div class='span10' float='right'>
          <div class='hero-unit' height='500px'>
            <?php
              include 'connect.php';
              $email = $_POST["email1"];
              $firstname = $_POST["firstname"];
              $lastname = $_POST["lastname"];
              $title = $_POST["title"];
              $description = $_POST["description"];
              $institution = $_POST["institution"];
              $department = $_POST["department"];
              $request = substr(number_format(time() * rand(),0,'',''),0,6);
              $email_check = mysql_query("SELECT email FROM wkdd_users WHERE email='$email'");
              $email_number = mysql_num_rows($email_check);
              if ($email_number>0) {
                echo "Email address is already in use<br>";
	              echo "<a href='javascript:history.go(-1)'>Return to the previous page</a>";
	              die();
              }
              $insert = mysql_query("INSERT INTO wkdd_users (email, title, firstname, lastname, institution, department, description, request) VALUES ('$email', '$title', '$firstname', '$lastname', '$institution', '$department', '$description', '$request')");
              if (!$insert) {
                die("There was a problem with the database.");
              }
              if ($firstname!=NULL) {
                echo "Dear " . $firstname . ", Thanks for providing your information! Your will receive an email asking you to verify your information. Once your confirmation is received, your request will be handled by the netlab system admin.<br>";
              }
              else {
                echo "Dear " . $email . ", Thanks for providing your email! Your will receive an email asking you to verify your information. Once your confirmation is received, your request will be handled by the netlab system admin.<br>";
              }
              echo "Please <a href='index.php'>click here to return the homapage</a><br>";
              $link = "emailverifynow.php?email=" . $email . "&&request=" . $request;
              $message = "Thanks for your interest in onGrid. We have received an access request from you as follows: \r\n" . "Name: " . $firstname . " " . $lastname . "\r\nTitle: " . $title . "\r\nEmail: " . $email . "\r\nDescription: " . $description . "\r\nInstitution: " . $institution . "\r\nDepartment: " . $department . "\r\nClick here to confirm your information: " . $link . "\r\nThank you!";
              //Please edit and change netlab@osumc.edu to our future system address
              $headers = 'From:my_email' . "\r\n" . 'Bcc:another_email' . "\r\n" . 'X-Mailer: PHP/' . phpversion();
              //Please edit and change netlab@osumc.edu to our future admin's address
              $sendmail = mail($email, 'onGrid: Email Confirmation', $message, $headers);
              if($sendmail){
                echo "Verification email is sent!";
              }
              mysql_close($con); 
            ?>
          </div>
          <footer><?php include 'footer.php' ?></footer>
        </div><!--/.fluid-container-->
      </div>
    </div>
  </body>
</html>
