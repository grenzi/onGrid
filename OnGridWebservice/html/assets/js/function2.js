
// assumes no colon
function getCui(x) {
	var reg = /[C]\d{7}/;
	var cui = reg.exec(x);
	return cui[0];
}

function process_request(id1, id2){

		$("#paths").html("");
		$("#display").html("");
		$("#path-error").text("");
		$("#request-head").html("");	

		var sid = id1;
		var eid = id2;
		
		var t = 0;  // assume input is already cui
		
		var headline = "<h9>" + sid + " and " + eid + "</h9>";
		$("#request-head").html(headline);		
		$.ajax({
			url: "process_function1.php",
			beforeSend: function() {
				$('#path-loading2').Loadingdotdotdot({
					"speed": 400,
					"maxDots": 4
				});
			},
			complete: function() {
				$("#path-loading2").Loadingdotdotdot("Stop");
				$("#path-loading2").html("");
			},
			type: 'POST',
			data: { start_id: sid, end_id: eid, type : t },
			dataType: 'json',
			success: function(response) {
				if (response == null) {
					$("#path-error").text("There was an error with your request.");
				}
				else if (response.hasOwnProperty('dberror')) {
					$("#path-error").text("There was an error with our database.  Please try again later.");
				}
				else if (response.hasOwnProperty('error')) {
					if ($.trim(response['error']) == 91)
						$("#path-error").text("There was an error with your request. The second CUI is isolated.");
					else if ($.trim(response['error']) == 1)
						$("#path-error").text("There was an error with your request. The first CUI is isolated.");
					else if ($.trim(response['error']) == 11)
						$("#path-error").text("There was an error with your request. Both the first and second CUI are isolated.");
					else if ($.trim(response['error']) == 21)
						$("#path-error").text("There was an error with your request. The first CUI was not found and the second CUI is isolated.");
					else if ($.trim(response['error']) == 92)
						$("#path-error").text("There was an error with your request. The second CUI was not found.");
					else if ($.trim(response['error']) == 12)
						$("#path-error").text("There was an error with your request. The first CUI is isolated and the second CUI was not found.");
					else if ($.trim(response['error']) == 22)
						$("#path-error").text("There was an error with your request. Both the first and second CUI were not found.");
					else
						$("#path-error").text("There was an error with your request.");
				}
				else {
					$("#path-error").text("");
					distance = response.distance;
					var paths_str = response.paths;

					if (paths_str === "") {
						$("#path-error").text("The query contains no path.");
					}
					else {
						rankList = response.ranklist;	
						pathList = buildPathList(distance, paths_str);
						var pathHTML = buildPathHTML(pathList);
						$("#paths").html(pathHTML);

						var structuredNodeList = getStructuredNodeList(pathList, distance, rankList);
						var linkList = getLinkList(pathList);

						var width = $("#display").width();
						var height = getMaxHeight(structuredNodeList) * 100;
						setVertexCoordinates(structuredNodeList, width, height);
						var nodeList = getSingleNodeList(structuredNodeList);
						setLinkCoordinates(nodeList, linkList);

						render(nodeList, linkList, width, height);
						$('#graph-options').css("visibility","visible");
					}
				}
			},
			error: function(xhr, textStatus, error){
				alert("Error processing the request.");
			}
	});
}

function write_contribution_table(c_matrix, row_query, col_query) {

// define and edit the range (and interval) of contribution value
	var top = 3.5;
	var bottom = 0;
	var num_interval = 8;
	var interval = (top-bottom)/(num_interval-1);
	var interval_small = interval/2;

	var row_array = row_query.split("\n");
	var col_array = col_query.split("\n");
	var buildstr = "<table class='table table-bordered' width='60%'>";
	buildstr += "<tr><td><h9>CUI</h9></td>";
	for (var i = 0; i < col_array.length; i++) {
		buildstr += "<td><h9>" + col_array[i] + "</h9></td>";;
	}
	buildstr += "</tr>";
	
	for (var i = 0; i < c_matrix.length; i++) {
		var row = c_matrix[i].split("\t");
		buildstr += "<tr><td><h9>" + row_array[i] + "</h9></td>";
		for (var j = 0; j < row.length; j++) {
			var id = "path_submit_btn_" + i + "_" + j;
			var cui1 = getCui(row_array[i]);
			var cui2 = getCui(col_array[j]);
			var id1 = "&quot;" + cui1 + "&quot;";
			var id2 = "&quot;" + cui2 + "&quot;";
			
			if( row[j] >= top ){
				var color = "#0155B3";
			}
			else if( row[j] >= top-interval && row[j] < top ){
				var color = "#0B5FBD";
			}
			else if( row[j] >= top-2*interval && row[j] < top-interval ){
				var color = "#297DDB";
			}
			else if( row[j] >= top-3*interval && row[j] < top-2*interval ){
				var color = "#3D91EF";
			}
			else if( row[j] >= top-4*interval && row[j] < top-3*interval ){
				var color = "#51A5FF";
			}
			else if( row[j] >= top-5*interval && row[j] < top-4*interval ){
				var color = "#6FC3FF";
			}
			else if( row[j] >= top-6*interval && row[j] < top-5*interval ){
				if( row[j] >= top-6*interval+interval_small && row[j] < top-5*interval ){
					var color = "#83D7FF" ;
				}
				else{
					var color = "#97EBFF";
				}

			}
			else if( row[j] >= top-7*interval && row[j] < top-6*interval ){
				if( row[j] >= top-7*interval+interval_small && row[j] < top-6*interval ){
					var color = "#B5FFFF";
				}
				else{
					var color = "#DDFFFF" ;
				}
			}
			else{
				var color = "FFFFFF";
			}
		
			buildstr += "<td bgcolor='" + color + "'><input type='submit' value='" + row[j] + "' id='" + id + "' class='btn-transparent' onclick = 'process_request(" + id1 + "," + id2 + ")'></td>";
		}
		buildstr += "</tr>";
	}
	buildstr += "</table>";
	$("#contribution-container").html(buildstr);
}

function write_distance_table(d_matrix, row_query, col_query) {

// define and edit the range (and interval) of contribution value
	var top = 7;
	var bottom = 0;
	var num_interval = (top-bottom)+1;

	var row_array = row_query.split("\n");
	var col_array = col_query.split("\n");
	var buildstr = "<table class='table table-bordered' width='60%'>";
	buildstr += "<tr><td><h9>CUI</h9></td>";
	for (var i = 0; i < col_array.length; i++) {
		buildstr += "<td><h9>" + col_array[i] + "</h9></td>";;
	}
	buildstr += "</tr>";
	
	for (var i = 0; i < d_matrix.length; i++) {
		var row = d_matrix[i].split("\t");
		buildstr += "<tr><td><h9>" + row_array[i] + "</h9></td>";
		for (var j = 0; j < row.length; j++) {
			var id = "path_submit_btn_" + i + "_" + j;
			var cui1 = getCui(row_array[i]);
			var cui2 = getCui(col_array[j]);
			var id1 = "&quot;" + cui1 + "&quot;";
			var id2 = "&quot;" + cui2 + "&quot;";

			if( row[j] == bottom ){
				var color = "#CDCDCD";
			}
			else if( row[j] >= bottom && row[j] > bottom && row[j] <= bottom+1 ){
				var color = "#008040";
			}
			else if( row[j] >= bottom && row[j] > bottom+1 && row[j] <= bottom+2 ){
				var color = "#28A868";
			}
			else if( row[j] >= bottom && row[j] > bottom+2 && row[j] <= bottom+3 ){
				var color = "#5ADA9A";
			}
			else if( row[j] >= bottom && row[j] > bottom+3 && row[j] <= bottom+4 ){
				var color = "#A0FFE0";
			}
			else if( row[j] >= bottom && row[j] > bottom+4 && row[j] <= bottom+5 ){
				var color = "#D2FFFF";
			}
			else if( row[j] >= bottom && row[j] > bottom+5 && row[j] <= bottom+6 ){
				var color = "#DCFFFF";
			}
			else if( row[j] >= bottom && row[j] > bottom+6 && row[j] <= top+5 ){
				var color = "E6FFFF";
			}
			else{
				var color = "#FFFFFF";
			}			
			buildstr += "<td bgcolor='" + color + "'><input type='submit' value='" + row[j] + "' id='" + id + "' class='btn-transparent' onclick = 'process_request(" + id1 + "," + id2 + ")'></td>";
		}
		buildstr += "</tr>";
	}
	buildstr += "</table>";
	$("#distance-container").html(buildstr);
}

function reset_elements() {
	$("#paths").html("");
	$("#display").html("");
	$("#request-head").html("");
}

function write_result(response) {
	var i;
	var row_query_str = "", row_isolated_str = "", col_query_str = "", col_isolated_str = "";
	
	for (i = 0; i < response[0].length; i++) {
		row_query_str += response[0][i] + "\t" + response[1][i] + "\n";
	}
	for (i = 0; i < response[2].length; i++) {
		row_isolated_str += response[2][i] + "\t" + response[3][i] + "\n";
	}
	for (i = 0; i < response[4].length; i++) {
		col_query_str += response[4][i] + "\t" + response[5][i] + "\n";
	}
	for (i = 0; i < response[6].length; i++) {
		col_isolated_str += response[6][i] + "\t" + response[7][i] + "\n";
	}
	
	$('#row-query').val(row_query_str);
	$('#row-isolated').val(row_isolated_str);
	$('#col-query').val(col_query_str);
	$('#col-isolated').val(col_isolated_str);
}

$(document).ready(function() {
	
	$("#process-btn").attr("disabled", "disabled"); 
	$('#matrix-container').hide();
	$('#toggle-area').hide();
	$('.go-left').hide();
	$('.go-right').hide();
	$('#toggle-row').hide();
	$('#toggle-col').hide();
	
	$('#preview-btn').click(function(e){
		e.preventDefault();
		reset_elements();
		$('#matrix-container').hide();
		$('#toggle-area').hide();
		if ($('#toggle-row').html() == "Show isolated/query") {
			$('#toggle-row').click();
		}
		if ($('#toggle-col').html() == "Show isolated/query") {
			$('#toggle-col').click();
		}
		$('#toggle-row').hide();
		$('#toggle-col').hide();
		$('.go-left').hide();
		$('.go-right').hide();
		var row_in = $('#row-input').val();
		var col_in = $('#col-input').val();
		$("#process-btn").attr("disabled", "disabled");

		$.ajax({
			url: "checkcui.php",
			beforeSend: function() {
				$('#matrix-loading').Loadingdotdotdot({
					"speed": 400,
					"maxDots": 4
				});
			},
			complete: function() {
				$("#matrix-loading").Loadingdotdotdot("Stop");
				$("#matrix-loading").html("");
			},
			type: 'POST',
			data: { row: row_in, col : col_in },
			dataType: 'json',
			success: function(response) {
				if (response['limit'] != null) {
					$("#process-btn").attr("disabled", "disabled");
					alert("Please limit the input to a 25x25 matrix.");
				}
				else if (response['error'] != null) {
					$("#process-btn").attr("disabled", "disabled");
					alert("Error in CUI lookup.");
				}
				else {
					write_result(response);
					$('#process-btn').removeAttr('disabled');
					$('.go-left').fadeIn();
					$('.go-right').fadeIn();
					$('#toggle-row').fadeIn();
					$('#toggle-col').fadeIn();
				}
			},
			error: function(xhr, textStatus, error){
				alert("Error processing the request.");
			}
		});
	});
	
	$('#process-btn').click(function(e){
		e.preventDefault();
		var row_query = $('#row-query').val();
		var col_query = $('#col-query').val();
		$.ajax({
			url: "process_matrix.php",
			beforeSend: function() {
				$('#matrix-loading').Loadingdotdotdot({
					"speed": 400,
					"maxDots": 4
				});
			},
			complete: function() {
				$("#matrix-loading").Loadingdotdotdot("Stop");
				$("#matrix-loading").html("");
			},
			type: 'POST',
			data: { row : row_query, col : col_query },
			dataType: 'json',
			success: function(response) {
				if ($.trim(response) == "error") {
					alert("Error processing the request.");
				}
				else {
					var c_matrix = response[0];
					var d_matrix = response[1];
					write_contribution_table(c_matrix, row_query, col_query);
					write_distance_table(d_matrix, row_query, col_query);
					$('#matrix-container').show();
					$('#toggle-area').show();
				}
			},
			error: function(xhr, textStatus, error){
				alert("Error processing the request.");
			}
		});
	});
	
	$('#toggle-row').toggle(function(event) {
			$('#row-left').hide();
			$('#row-right').hide();
			$('#toggle-row').html("Show isolated/query");
			event.preventDefault();
		},
		function(event) {
			$('#row-left').show();
			$('#row-right').show();
			$('#toggle-row').html("Hide isolated/query");
			event.preventDefault();
	});
	
	$('#toggle-col').toggle(function(event) {
			$('#col-left').hide();
			$('#col-right').hide();
			$('#toggle-col').html("Show isolated/query");
			event.preventDefault();
		},
		function(event) {
			$('#col-left').show();
			$('#col-right').show();
			$('#toggle-col').html("Hide isolated/query");
			event.preventDefault();
	});
	
});
