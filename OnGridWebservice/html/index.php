<!DOCTYPE html>
<?php session_start(); define('MyConst', TRUE); ?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>onGrid - online conceptual study platform using Graph indexing</title>
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
        <hr width='100%' size='4' color="white" style="margin-left:20px;"/>
      </div>
    </div>
    <div class="container-fluid">
      <div class="row-fluid">
        <div class="span2">
          <div class="bs-docs-sidebar">
            <ul class="nav nav-list bs-docs-sidenav">
              <li class='active'><a href='#'>Home</a></li>
              <li><a href='tutorial.php'>About</a></li>
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
        <div class="hero-unit" height="500px" align="justify">
          <h3>Welcome to onGrid, a conceptual study platform using Graph indexing</h3>
		 <p> The Unified Medical Language System (UMLS) contains many important ontologies in which terms are connected by semantic relations. Using transitively associated information from ontologies and the UMLS has been shown to be effective for many studies on the relationships between biomedical concepts. Although there are a few tools and methods available for extracting transitive relationships from the UMLS, they usually have major restrictions on the length of transitive relations or on the number of data sources. Therefore, to provide an online platform to enable efficient studies on the conceptual relationships between medical terms for the biomedical community, we designed this <u>on</u>line conceptual study platform using <u>Gr</u>aph <u>i</u>n<u>d</u>exing (abbr. "onGrid").</p>
		<p>We will keep improving onGrid to better serve the biomedical community, and we expect this web service will continue evolving and supporting new and powerful knowledge discovery methods.</p>
          <p><a href="tutorial.php" class='btn btn-primary btn-large'>A brief tutorial &raquo;</a></p>
        </div>
        <footer><?php include 'footer.php' ?></footer>
      </div>
    </div>
  </div>
  </body>
</html>
