<!DOCTYPE html>
<?php session_start(); define('MyConst', TRUE); ?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Multiple-Multiple</title>
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
	<script src = "./assets/js/function2.js"></script>
	
    <script language="javascript"> 
	function toggle() {
		var ele = document.getElementById("paths");
		var text = document.getElementById("displayText");
		if(ele.style.display == "block") {
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
            	<div class="hero-unit-large">
            		<span class="badge badge-warning">Multi-Multi&nbsp</span>
            		<h11>Please provide your query CUIs&nbsp&nbsp</h11><h9>(Multiple CUI to Multiple CUI)</h9><hr width=100% size=4 margin="2px" color=#357EC7 style="filter:alpha(opacity=100,finishopacity=0,style=2)">
            		<div id = "hands-container">
            			<div id = "input-container">
							<div id = "row-container">
								<span class = "go-center">CUI Group 1:</span>
								<textarea id = "row-input" cols="90" rows="8"></textarea>
								<div id = "hints">
								<h15>(e.g., C1537734,C1835840,C1537910,C1537912,C0812286,C1537797</h15><br />
								<h15>or MIR29B2 gene,MIR29B1 gene,MIRLET7A1 gene,MIRLET7A3 gene,NFKB2 gene,MIR143 gene)</h15>
								</div>
								<div class = "display-extra"><a id="toggle-row" href="">Hide isolated/query</a></div>
								<div id = "row-left" class="go-left">
									<span class = "go-center">Isolated CUIs:</span>
									<textarea id = "row-isolated" cols="40" rows="8" readonly="readonly"></textarea>
								</div>
								<div id = "row-right" class="go-right">
									<span class = "go-center">Query CUIs:</span>
									<textarea id = "row-query" cols="40" rows="8" readonly="readonly"></textarea>
								</div>
							</div>
							<div id = "col-container">
								<span class = "go-center">CUI Group 2:</span>
								<textarea id = "col-input" cols="80" rows="8"></textarea>
								<div id="hints">
								<h15>(e.g., C1537910,C1537912,C0079419,C0919524,C0376571,C1333544</h15><br />
								<h15>or MIRLET7A1 gene,MIRLET7A3 gene,TP53 gene,ATM gene,BRCA1 gene,FGFR4 gene)</h15>
								</div>
								<div class = "display-extra"><a id="toggle-col" href="">Hide isolated/query</a></div>
								<div id="col-left" class="go-left">
									<span class = "go-center">Isolated CUIs:</span>
									<textarea id = "col-isolated" cols="40" rows="8" readonly="readonly"></textarea>
								</div>
								<div id = "col-right" class="go-right">
									<span class = "go-center">Query CUIs:</span>
									<textarea id = "col-query" cols="40" rows="8" readonly="readonly"></textarea>
								</div>
							</div>
						</div>
						<div id = "options-container">
							<input type="submit" value="Preview" id = "preview-btn" class="btn btn-small">
							<input type="submit" value="Process" id = "process-btn" class="btn btn-small">
							<p>Accepted input includes a list of CUIs delimited by comma. Input is limited to a matrix of 25x25 concepts for viewing purposes. Please allow up to 10 seconds for the request to be processed.</p>
							<p>Isolated CUIs indicate elements which have no connection to any other node in the UMLS.</p>
							<p>Query CUIs will be used for the actual query results.</p>
							<div id = "matrix-loading"></div>
						</div>
					</div>
					<div id = "matrix-container">
						<p>Relationship Matrix: </p><h16>(Click number to see specific path)</h16>
						<div id = "contribution-container"></div>
						<p>Distance Matrix: </p><h16>(Click number to see specific path)</h16>
						<div id = "distance-container"></div>
						<div id = "path-loading2"></div>
					</div>
					<div id="request-head" class="align-left"></div>
					<div id="display" class="displaysvg"></div>
					 <div id = "path-error"></div>
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
