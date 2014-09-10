<!DOCTYPE html>
<?php session_start(); define('MyConst', TRUE); ?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Information Management</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <!-- Le styles -->
    <link href="./assets/css/bootstrap.css" rel="stylesheet">
    <style type="text/css">
      body {
        padding-top: 10px;
        padding-bottom: 40px;
      }
      .sidebar-nav {
        padding: 9px 0;
      }
    </style>
    <link href="./assets/css/bootstrap-responsive.css" rel="stylesheet">
    <link href="./assets/css/docs.css" rel="stylesheet">
       <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
     <!--[if IE]>
      <script type="text/javascript">document.createElement('h7');</script>
      <script type="text/javascript">document.createElement('h8');</script>
      <script type="text/javascript">document.createElement('h9');</script>
      <script type="text/javascript">document.createElement('h10');</script>
      <script type="text/javascript">document.createElement('h11');</script>
      <script type="text/javascript">document.createElement('h12');</script>
      <script type="text/javascript">document.createElement('h13');</script>
      <script type="text/javascript">document.createElement('h14');</script>
      <script type="text/javascript">document.createElement('h15');</script>
      <script type="text/javascript">document.createElement('h16');</script>
      <script type="text/javascript">document.createElement('h17');</script>
    <![endif]-->

    <script language="javascript" type="text/javascript">
      function validation() {
  		  var frm = document.getElementById("myform");
		    var mailformat = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/;
		    if(frm.firstname.value.length>30) {
          alert("Please provide your firstname no larger than 30 characters");
          return false;
		    }
		    if(frm.lastname.value.length>30) {
          alert("Please provide your lastname no larger than 30 characters");
          return false;
		    }
        if(frm.institution.value.length>45) {
          alert("Please provide your institution no larger than 45 characters");
			    return false;
		    }
		    if(frm.department.value.length>45) {
			    alert("Please provide your department no larger than 45 characters");
          return false;
        }
        if(frm.title.value.length>30) {
          alert("Please provide your title no larger than 30 characters");
			    return false;
		    }
        else {
    			return true;
        }
      }
    </script>
  </head>

  <body>
    <div class="topban">
      <div class = "headline-left"><img src = "assets/img/ongrid.png" width='252' height='96' /></div>
      <div class="headline">
        <h7>Online conceptual study platform using Graph indexing</h7>
        <hr width=100% size=4 color="white" style="margin-left:20px;">
      </div>
    </div>
    <div class="container-fluid">
      <div class="row-fluid">
        <div class="span2">
          <div class="bs-docs-sidebar">
            <ul class="nav nav-list bs-docs-sidenav">
              <li class="active"><a href="index.php">Home</a></li>
		          <li><a href="tutorial.php">About</a></li>
              <li><a href="discovery.php">Start Discovery</a></li>
		          <li><a href="publications.php">Publications</a></li>
              <li><a href="terms.php">Term of Use</a></li>
              <li><a href="contact.php">Contact</a></li>
            </ul>
            <div class="bottomlogo">
              <?php include 'side_logos.php' ?>
            </div>	
          </div><!--/.well -->
        </div>   
        <div class="span10" float="right">
          <div class="user">
            <?php include 'user_info.php' ?>
          </div>
          <div class="hero-unit" height="500px">
            <span class="badge badge-info">Account</span><h11>Manage your information</h11><br>
            <hr width=100% size=4 color=#357EC7 style="filter:alpha(opacity=100,finishopacity=0,style=2)">
            <h9>Welcome!</h9><br>
            <?php
              include 'connect.php';
              if(!isset($_SESSION["email"])) {
                echo "Sorry we lost your session, Please ioglin again.<br>"; 
                echo "<a href='index.php'>Click here to go back to homepage.</a>";
                die();
              }
              $email = $_SESSION["email"];
              $query1 = mysql_query("SELECT * FROM wkdd_users WHERE email='$email'");
              if (!$query1) {
                die("There is some problem: " . mysql_error());
              }
              $row = mysql_fetch_assoc($query1);
              if ($row['firstname']!=NULL) {
                echo "<h13>Your Firstname: " . $row['firstname'] . "</h13><br>";
              }
              if ($row['lastname']!=NULL) {
                echo "<h13>Your Lastname: " . $row['lastname'] . "</h13><br>";
              }
              if ($row['title']!=NULL) {
                echo "<h13>Your Title: " . $row['title'] . "</h13><br>";
              }
              if ($row['department']!=NULL) {
                echo "<h13>Your Department: " . $row['department'] . "</h13><br>";
              }
              if ($row['institution']!=NULL) {
                echo "<h13>Your Institution: " . $row['institution'] . "</h13><br>";
              }
              echo "<br>";
              mysql_close($con);
            ?>
            <h9>Update your information</h9><br>
            <form id="myform" class="form-horizontal" action="updateinfo.php" method="post" onsubmit="return validation();">
              <div class="control-group">
                <label class="control-label" for="firstname">Your firstname</label>
                <div class="controls">
                  <input type="text" id="firstname" name="firstname" placeholder="Firstname">
                </div>
              </div>
              <div class="control-group">
                <label class="control-label" for="lastname">Your lastname</label>
                <div class="controls">
                  <input type="text" id="lastname" name="lastname" placeholder="Lastname">
                </div>
              </div>
              <div class="control-group">
                <label class="control-label" for="title">Your Title</label>
                <div class="controls">
                  <input type="text" id="title" name="title" placeholder="Title">
    				    </div>
  			      </div>
			        <div class="control-group">
    				    <label class="control-label" for="institution">Your Institution</label>
    				    <div class="controls">
      				    <input type="text" id="institution" name="institution" placeholder="Institution">
    				    </div>
  			      </div>
			        <div class="control-group">
    				    <label class="control-label" for="department">Your Department</label>
    				    <div class="controls">
      				    <input type="text" id="department" name="department" placeholder="Department">
    				    </div>
  			      </div>
 			        <div class="control-group">
    				    <div class="controls">
            			<button type="submit" class="btn">Update</button>
    				    </div>
  			      </div>
            </form>
          </div>
          <footer><?php include 'footer.php' ?></footer>
        </div><!--/.fluid-container-->
      </div>
    </div>
  </body>
</html>
