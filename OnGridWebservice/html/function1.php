<!DOCTYPE html>
<?php session_start(); define('MyConst', TRUE); ?>

<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>One-One</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <!-- Le styles -->
    <link href="./assets/css/bootstrap.css" rel="stylesheet">
	 <link href="./assets/css/lagniappe.css" rel="stylesheet">
   <link href="./assets/css/graph.css" rel="stylesheet">
   <link href="./assets/css/powertip.min.css" rel="stylesheet">
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

    <script src="./assets/js/jquery-1.7.2.min.js"></script>
    <script src="./assets/js/d3.v2.js"></script>
    <script src = "./assets/js/jquery.loadingdotdotdot.js"></script>
    <script src = "./assets/js/powertip.min.js"></script>
	  <script src = "./assets/js/shortestpath-vis.js"></script>
    <script src = "./assets/js/function1.js"></script>
	
    <script language="javascript">
      function toggle() {
        var ele = document.getElementById("paths");
        var text = document.getElementById("displayText");
        if (ele.style.display == "block") {
    			ele.style.display = "none";
          text.innerHTML = "Show Paths in Plain Text";
        }
        else {
          ele.style.display = "block";
          text.innerHTML = "Hide Paths in Plain Text";
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
          <div class="hero-unit">
            <span class="badge badge-success">One-One&nbsp&nbsp&nbsp</span>
		        <h11>Please enter your query&nbsp&nbsp</h11><h9>(One CUI to One CUI)</h9><hr width=100% size=4 margin="2px" color=#357EC7 style="filter:alpha(opacity=100,finishopacity=0,style=2)">
		        <div id = "path-container">
              <div id = "path-helper">
                <div id = "path-input">
                  <form name="path_form" method="post">
                   <label class = "inline-label" for="start_id">Enter start CUI or term: </label><input type="text" id="start_id" size="7" class="input-medium"/>
						       <h16>(e.g., C1415882 or IDS Gene)</h16><br />
						       <label class = "inline-label" for="end_id">Enter end CUI or term: </label><input type="text" id="end_id" size="7" class="input-medium"/>
						        <h16>(e.g., C0023434 or Chronic Lymphocytic Leukemia)</h16><br />
						        <input type="submit" value="Submit" id="path_submit_btn" class="btn btn-small">
						      </form>
					      </div>
              </div>
              <div id = "path-loading"></div>
            </div>
			<div id = "path-error"></div>
			<div id = "ldp-options-container">
				<div id="ldp-options-left" class="go-left">
						<span class = "go-center">Start CUI Options:</span>
						<select id = "start-cui-select"></select>
				</div>
				<div id = "ldp-options-right" class="go-right">
						<span class = "go-center">End CUI Options:</span>
						<select id = "end-cui-select"></select>
				</div>
				<div id = "ldp-options-btn-container">
					<input type="submit" value="Submit Using Selected Options" id="ldp_options_submit_btn" class="btn btn-small">
				</div>
			</div>
            <div id="display" class="displaysvg"></div>
            <div id = "graph-options">
                    <input type="submit" value="Reset Visualization" id="reset_btn" class="btn btn-small"><br />
                    <input type="checkbox" name="showLinks" id="showLinks" /> Show All Edges<br />
                    <a id="displayText" href="javascript:toggle();">Show Paths in Plain Text</a>
            </div>
            <div id="paths" style="display:none; padding:20px;"></div>
          </div>
          <footer><?php include 'footer.php' ?></footer>
        </div><!--/.fluid-container-->
      </div>
    </div>
  </body>
</html>
