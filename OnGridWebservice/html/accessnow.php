<!DOCTYPE html>
<?php session_start(); define('MyConst', TRUE); ?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Access</title>
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

    <script src="./assets/js/gen_validatorv4.js" type="text/javascript"></script>
    <script language="javascript" type="text/javascript">
	function validation() {
  		var frm = document.getElementById("myform");
		var mailformat = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/;
		var checked = document.getElementById("agreebox").checked;
		if(frm.email1.value.length==0){
			alert("Please provide your email address");
			return false;
		}
		if(!(frm.email1.value.match(mailformat))){
			alert("You have entered an invalid email address");
			return false;
		}
		if(frm.email2.value.length==0){
			alert("Please repeat your email address");
			return false;
		}
		if(frm.email1.value!=frm.email2.value){
			alert("The email address you have entered do not match, please recheck that");
			return false;
		}
		if(frm.firstname.value.length>30){
			alert("Please provide your firstname no larger than 30 characters");
			return false;
		}
		if(frm.lastname.value.length>30){
			alert("Please provide your lastname no larger than 30 characters");
			return false;
		}
		if(frm.institution.value.length>45){
			alert("Please provide your institution no larger than 45 characters");
			return false;
		}
		if(frm.department.value.length>45){
			alert("Please provide your department no larger than 45 characters");
			return false;
		}
		if(frm.title.value.length>30){
			alert("Please provide your title no larger than 30 characters");
			return false;
		}
		if(checked==false){
			alert("Please read and agree the terms before submitting your request");
			return false;
		}
  		else
  		{
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
	      				<li><a href="index.php">Home</a></li>
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
		   		<div class="hero-unit">
		   			<div class="getin">
		   				<form id="mylogin" method='post' action='login.php'>
		   					<legend>Access your job with Code</legend>
		   					<label>Your Email Address: </label>
							<input type='text' class='input' placeholder='Email' id='email' name='email'>
							<label>Your Access Code: </label>
	 		 				<input type='password' class='input' placeholder='Access Code' id='code' name='code'></br>
	  						<button type='submit' class='btn btn-small'>Check Job Status</button>
						</form> 
					</div>
					<div class="access">
						<form id="myform" action="access.php" method="post" onsubmit="return validation();">
	 			 			<legend>Get your Access Code now!</legend>
	  			 			<label>Your Email Address*: (This will be your username)</label>
	 			 			<input type="text" placeholder="Email address" name="email1" id="email1" style="width:200px"><h16>&nbsp(required)</h16>
							<label>Please repeat your email address*:</label>
				 		 	<input type="text" placeholder="Repeat email address" name="email2" id="email2" style="width:200px"><h16>&nbsp(required)</h16>
							<label>Description of Your Specific Requirement (200 characters maximum):</label>
						 	<textarea rows="4" cols="60" name="description" id="description" maxlength="200"></textarea>
				 			<label>Your Firstname:</label>
				 			<input type="text" placeholder="Firstname" name="firstname" id="firstname" style="width:200px">
							<label>Your Lastname:</label>
				 			<input type="text" placeholder="Lastname" name="lastname" id="lastname" style="width:200px">
							<label>Your Title:</label>
				 			<input type="text" placeholder="Title" name="title" id="title" style="width:200px">
							<label>Your Institution:</label>
				 			<input type="text" placeholder="Institution" name="institution" id="institution" style="width:200px">
							<label>Your Department:</label>
				 			<input type="text" placeholder="Department" name="department" id="department" style="width:200px">
							<label>Terms of Use</label>
							<div class="agreement">
								<p style="width:380px; word-wrap: break-word; font-size:12px; line-height:15px;">Agreement</br>
									Agreement info here.
								</p>
							</div>
							<div style="padding-left: 0">
								<label class="checkbox"><input type="checkbox" id="agreebox" name="agreebox">I agree</label>  
							</div>
							<button type="submit" class="btn">Submit</button>
						</form>
					</div>
				</div>
				<footer><?php include 'footer.php' ?></footer>
			</div><!--/.fluid-container-->
		</div>
	</div>
  </body>
</html>
