<!DOCTYPE html>
<?php 
session_start(); 
define('MyConst', TRUE);
if( !(isset($_SESSION["status"]) && isset($_SESSION["email"]) && isset($_SESSION["name"]))){
  header('Location: accessnow.php');
}
?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>New Job</title>
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
              <li class="active"><a href="discovery.php">Start Discovery</a></li>
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
            <span class="badge badge-success">Upload&nbsp</span><h11>Submit new job</h11><br>
            <hr width=100% size=4 color=#357EC7 style="filter:alpha(opacity=100,finishopacity=0,style=2)">
            <h5>Notice: The maximum number of jobs kept in your history is 10. Once that limit is reached, newly submitted jobs will overwrite the oldest in your history. You may also <a href="history.php">manage your job history manually</a>.</h5>
            <form action="process_upload.php" method="post" enctype="multipart/form-data">
              <input type="hidden" name="MAX_FILE_SIZE" value="2000000" />
              <table class="table1">
                <tr>
                  <td width="13%"><h9>Give a name for the job:</h9></td>
         		      <td width="50%"><input type="text" id = "job_name" name="job_name" size = "35"></td>
			          </tr>
         		    <tr>
                  <td width="13%"><h9>Upload your file for CUI Group 1</h9></td>
         		      <td width="50%"><input type="file" name="row_file" id="row_file" class="btn btn-small" width="100px"/> </td>
			          </tr>
			          <tr>
                  <td width="13%"><h9>Upload your file for CUI Group 2</h9></td>
         		      <td width="50%"><input type="file" name="col_file" id="col_file" class="btn btn-small" width="100px"/> </td>
                </tr>
                <tr>
                  <td width="13%"></td>
           		    <td width="30%"><input type="submit" id="process-button" name="process-button" class="btn btn-primary" value="Submit"/></td>
                </tr> 
              </table>
            </form>
          </div>
          <footer><?php include 'footer.php' ?></footer>
        </div><!--/.fluid-container-->
      </div>
    </div>
  </body>
</html>

