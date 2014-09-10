<!DOCTYPE html>
<?php session_start(); define('MyConst', TRUE); ?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>onGrid - an online conceptual study platform using Graph indexing</title>
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

  </head>

  <body>
    <div class="topban">
    <div class = "headline-left"><img src = "assets/img/ongrid.png" width='252' height='96' /></div>
    <div class="headline">
		 <h7>Online conceptual study platform using Graph indexing</h7>
		  <hr width=100% size=4 color="white" style="margin-left:20px;"/>
    </div>
    </div>
    <div class="container-fluid">
      <div class="row-fluid">
        <div class="span2">
          <div class="bs-docs-sidebar">
            <ul class="nav nav-list bs-docs-sidenav">
              <li><a href="index.php">Home</a></li>
              <li><a href="tutorial.php">About</a></li>
              <li class="active"><a href="#">Start Discovery</a></li>
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
  		      <h11>We provide three comprehensive solutions to index UMLS, please select the method here.</h11><hr width=100% size=4 color=#357EC7 style="filter:alpha(opacity=100,finishopacity=0,style=2)">
  		      <span class="badge badge-success">One-One&nbsp&nbsp&nbsp</span>&nbsp&nbsp&nbsp&nbsp<a href="function1.php"><h9>One Concept to One Concept</h9></a><br>
  		      <h10 style="padding-left: 80px">Find shortest paths connecting two concepts</h10><br><br>
  		      <span class="badge badge-warning">Multi-Multi&nbsp</span>&nbsp&nbsp&nbsp&nbsp<a href="function2.php"><h9>Multiple Concepts to Multiple Concepts</h9></a><br>
  		      <h10 style="padding-left: 80px">build a distance matrix and a relationship matrix between two sets of input CUIs</h10><br><br>
  		      <span class="badge badge-info">Advanced&nbsp&nbsp</span>&nbsp&nbsp&nbsp&nbsp<a href="upload.php"><h9>Multiple Concepts to Multiple Concepts--Upload files (Requires Access Code)</h9></a><br>
  		      <h10 style="padding-left: 80px">build a distance matrix and a relationship matrix between two input CUI files</h10><br><br>
          </div>

           <footer><?php include 'footer.php' ?></footer>
        </div><!--/.fluid-container-->
      </div>
    </div>
  </body>
</html>
