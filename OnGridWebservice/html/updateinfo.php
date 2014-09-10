
<!DOCTYPE html>
<?php
//This page works after user has submitted an application/registeration
//An request email will send to admin to let he/she exmaine the application/approve it
//Please edit specific email address accoding to the commentss below:
session_start(); define('MyConst', TRUE); 
?>
<html lang='en'>
  <head>
    <meta charset='utf-8'>
    <title>Update</title>
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
      <div class = 'headline-left'><img src = 'assets/img/ongrid.png' width='252' height='96' /></div>
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
              <li class='active'><a href='index.php'>Home</a></li>
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
          <div class='user'>
            <?php include 'user_info.php' ?>
          </div>
          <div class='hero-unit' height='500px'>
          <?php 
            include 'connect.php';
            $email = $_SESSION["email"];
            $firstname = $_POST["firstname"];
            $lastname = $_POST["lastname"];
            $title = $_POST["title"];
            $institution = $_POST["institution"];
            $department = $_POST["department"];
            $email_check = mysql_query("SELECT email FROM wkdd_users WHERE email='$email'");
            $email_number = mysql_num_rows($email_check);
            if ($email_number<=0) {
              echo "Erroe happens<br>";
            }
            if ($firstname!=NULL) {
              $alter = mysql_query("UPDATE wkdd_users SET firstname='$firstname' WHERE email='$email'");
              if (!$alter) {
                die("There is some problem: " . mysql_error());
              }
              else {
                echo "Firstname is updated<br>";
              }
            }
            if ($lastname!=NULL) {
              $alter = mysql_query("UPDATE wkdd_users SET lastname='$lastname' WHERE email='$email'");
              if (!$alter) {
                die("There is some problem: " . mysql_error());
              }
              else {
                echo "Lastname is updated<br>";
              }
            }
            if ($title!=NULL) {
              $alter = mysql_query("UPDATE wkdd_users SET title='$title' WHERE email='$email'");
              if (!$alter) {
                die("There is some problem: " . mysql_error());
              }
              else {
                echo "Title is updated<br>";
              }
            }
            if ($institution!=NULL) {
              $alter = mysql_query("UPDATE wkdd_users SET institution='$institution' WHERE email='$email'");
              if (!$alter) {
                die("There is some problem: " . mysql_error());
              }
              else {
                echo "Institution is updated<br>";
              }
            }
            if ($department!=NULL) {
              $alter = mysql_query("UPDATE wkdd_users SET department='$department' WHERE email='$email'");
              if (!$alter) {
                die("There is some problem: " . mysql_error());
              }
              else {
                echo "Department is updated<br>";
              }
            }
            mysql_close($con);
          ?>
          <a href='update.php'>Click here to return back</a>
          </div>
          <footer><?php include 'footer.php' ?></footer>
        </div><!--/.fluid-container-->
      </div>
    </div>
  </body>
</html>

