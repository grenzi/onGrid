<!DOCTYPE html>
<?php session_start(); define('MyConst', TRUE); ?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>About</title>
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
              <li><a href='index.php'>Home</a></li>
              <li class='active'><a href='#'>About</a></li>
              <li><a href='discovery.php'>Start Discovery</a></li>
              <li><a href='publications.php'>Publications</a></li>
              <li><a href='terms.php'>Term of Use</a></li>
              <li><a href='contact.php'>Contact</a></li>
            </ul>
            <div class="bottomlogo">
              <?php include 'side_logos.php' ?>
	         </div>	
         </div><!--/.well -->
       </div>   
       <div class="span10">
        <div class="user">
          <?php include 'user_info.php' ?>
	      </div>
        <div class="hero-unit" height="500px">
          <h11>A brief tutorial</h11>
            <hr width=100% size=4 color="blue">
              <ul class="nav nav-pills">
                <li><a href="#function1">One-to-One</a></li>
          			<li><a href="#function2">Multiple-to-Multiple</a></li>
         			  <li><a href="#function3">Advanced</a></li>
        		  </ul>
            <div style="padding-left: 20px;" align="justify">
              <h5>Software version: 2.0 beta</br>
                Last Update: 3/29/2013</br>
                UMLS version: 2012AA (Level 0)</br></br>
                This version onGrid provides online kDLS query services for the UMLS. It follows the kDLS configuration (k=6) that has been used to perform disease gene prioritization in our publication <a href="http://www.sciencedirect.com/science/article/pii/S1532046411002000" target="_blank">"k-neighborhood Decentralization: A Comprehensive Solution to Index the UMLS for Large Scale Knowledge Discovery"</a>. For details about kDLS, please refer to this publication.</br></br>
                Please note that since k is configured to be 6, the method only guarantees finding exact distances under 6 hops, or at least one shortest paths under 6 hops, on the underlying graph. This restriction applies to all the services provided in the following. Please refer to the above publication for details on why we set this restriction.	 
              </h5>
      		    <h5>We organize the services in three parts. You can access them under "Start Discovery"!</h5><br>
              <a class="anchor" name="function1"><span class="badge badge-success">One-One&nbsp&nbsp&nbsp</span><h9>One concept to one concept</h9></a><br>
      		    <h12>This will find shortest paths connecting two concepts with the restriction described above. <br>Directions: </h12><br>
      		    <h12>1. Enter the start CUI and end CUI into the text boxes respectively, then click submit. <br>
                &nbsp&nbsp&nbsp&nbspThe right format of CUI consists 8 characters, a "C" followed by 7 numbers. e.g., C1415882</h12><br>
      		    <h12>2. Our system will process your request and show the specific paths between these two concepts.</h12><br>
      		    <h12>3. You can click "Show Path in Plain Text" to copy the path details. </h12><br><br>

              <a class="anchor" name="function2"><span class="badge badge-warning">Multi-Multi&nbsp</span><h9>Multiple concepts to multiple concepts</h9></a><br>
      		    <h12>This will build a distance matrix and a relationship matrix between two sets of input CUIs. <br>Directions: </h12><br>
      		    <h12>1. Your can enter multiple start CUIs and multiple end CUIs into the text boxes respectively. Please sperate different CUIs with a comma.<br>
                &nbsp&nbsp&nbsp&nbspOne set of CUIs (the start CUIs) will be the rows of tables, and the other set (the end CUIs) will be the columns.</h12><br>			
      		    <h12>2. Please click "Preview" on the right panel, our system will separate the valid CUIs into "Isolated CUIs" and "Query CUIs". <br>
      			    &nbsp&nbsp&nbsp&nbsp"Isolated CUIs" will not participate in querying because they do not have relationships with any other CUIs in the UMLS.</h12><br>
      		    <h12>3. Please click "Process" on the right panel. </h12><br>
      		    <h12>4. Our system will process your request and output the corresponding relatioship matrix and distance matrix.<br>
      		    <h12>5. You can click any number in the tables to start a path query between two concepts. </h12><br><br>

      		    <a class="anchor" name="function3"><span class="badge badge-info">Advanced&nbsp&nbsp</span><h9>Advanced function, Multiple concepts to multiple concepts in large scale</h9></a><br>
      		    <h12>This provides the same function as the above multi-multi, except that it supports large scale inputs <br>Directions: </h12><br>
      		    <h12>1. To use this advanced function, you will need to provide at least your email address to receive an access code</h12><br>
      		    <h12>2. With your email address and access code, you can submit a new job, and check your job history.</h12><br>
      		    <h12>3. Please prepare your start CUIs and end CUIs into two .txt files respectively. The Format is one CUI per line.</h12><br>		
      		    <h12>4. Our system will process your request. You can track your submitted job under "history".<br>
                Once one job's status is "Completed". you can download the corresponding relationship matrix and distance matrix there.</h12><br><br>
      			 
            </div>	            
          </div>
           <footer><?php include 'footer.php' ?></footer>
        </div><!--/.span10-->
      </div>
    </div>
  </body>
</html>
