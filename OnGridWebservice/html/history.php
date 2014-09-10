
<?php 
session_start();
define('MyConst', TRUE);

if( !(isset($_SESSION["status"]) && isset($_SESSION["email"]) && isset($_SESSION["name"]))){
	header('Location: index.php');
}
else {
	include 'connect.php';
	$user_email = $_SESSION["email"];
	if (isset($_POST['delete-btn']) && isset($_POST['cb'])) {
		$checked = $_POST['cb'];
		$dir_result = "/matrix_results/";
		$dir_upload = "/matrix_uploads/";
		foreach ($checked as $id) {

			// first remove files from directories
			$sql = "SELECT special_key FROM wkdd_matrix_jobs WHERE id = '$id' AND user_email = '$user_email'";
			$result = mysql_query($sql);
			$row = mysql_fetch_row($result);
			if (file_exists($dir_result . $row[0] . '_contribution.txt')) {
				@unlink($dir_result . $row[0] . '_contribution.txt');
			}
			if (file_exists($dir_result . $row[0] . '_distance.txt')) {
				@unlink($dir_result . $row[0] . '_distance.txt');
			}
			if (file_exists($dir_upload . $row[0] . '_col.txt')) {
				@unlink($dir_upload . $row[0] . '_col.txt');
			}
			if (file_exists($dir_upload . $row[0] . '_row.txt')) {
				@unlink($dir_upload . $row[0] . '_row.txt');
			}

			// then remove entries in database
			$sql = "DELETE FROM wkdd_matrix_jobs WHERE id = '$id' AND user_email = '$user_email'";
			$result = mysql_query($sql);
		}
	}
	$sql = "SELECT id, job_name, special_key, status, start_time, end_time FROM wkdd_matrix_jobs WHERE user_email = '$user_email' ORDER BY end_time ASC";
	$result = mysql_query($sql);
	$result_array = array();
	while($row = mysql_fetch_array($result)) {
		$result_array[] = $row;
	}
	mysql_close($con);
}
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>History</title>
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
		   			<span class="badge badge-warning">History&nbsp</span><h11>Track and manage previous jobs</h11><br>
		   			<hr width=100% size=4 color=#357EC7 style="filter:alpha(opacity=100,finishopacity=0,style=2)">
		   			<form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post" id="delete-form">
						<table class="table table-condensed">
							<thead>
								<tr>
									<th width=3%></th>
									<th width=3%>#</th>
									<th width=15%>Name</th>
									<th width=12%>Status</th>
									<th width=20%>Submission Time</th>
									<th width=20%>Completion Time</th>
									<th width=27%>Access</th>
								</tr>
							</thead>
							<tbody>
								<?php
									$count = 1;
									foreach ($result_array as $r) {
										if ($count % 2 == 0)
											echo '<tr>';
										else
											echo '<tr bgcolor="#d8e3e9">';

										echo '<td><input type="checkbox" name="cb[]" value="' . $r['id'] . '"></input></td>';
										echo '<td>' . $count . '</td>';
										echo '<td>' . $r['job_name'] . '</td>';
										echo '<td>' . $r['status'] . '</td>';
										if ($r['start_time'] == 'null') {
											echo '<td>N/A</td>';
										}
										else {
											echo '<td>' . $r['start_time'] . '</td>';
										}
										if ($r['end_time'] == 'null') {
											echo '<td>N/A</td>';
										}
										else {
											echo '<td>' . $r['end_time'] . '</td>';
										}
										if($r['status'] == 'complete'){
											echo '<td>' . '<a href="download.php?download_file=' . $r['special_key'] . '_contribution.txt">Relationship Matrix</a>&nbsp&nbsp
												<a href="download.php?download_file=' . $r['special_key'] . '_distance.txt">Distance Matrix</a>' . '</td>';
										}
										else {
											echo '<td>Please check it later</td>';
										}
										echo '</tr>';
										$count++;
									}
								?>
							</tbody>
						</table>
						<?php
							if (count($result_array) > 0) {
								echo '<input name="delete-btn" type="submit" id="delete-btn" value="Remove Selected" class="btn btn-small">';
							}
							echo '</form>';
						?>
					</div>
					<footer><?php include 'footer.php' ?></footer>
				</div><!--/.fluid-container-->
			</div>
		</div>

	    <script src="./assets/js/jquery-1.7.2.min.js"></script>
		<script>
			$(document).ready(function() {
				$("#delete-form").submit(function(evt) {
					if ($("#delete-form input:checkbox:checked").length > 0) {
			    		if(!confirm('OK to delete selected jobs?')) { 
			    			evt.preventDefault(); 
			    		}
		    		}
		    		else {
		    			evt.preventDefault(); 
		    		}
			   });
			});
		</script>
	</body>
</html>
