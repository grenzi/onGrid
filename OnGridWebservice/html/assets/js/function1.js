
	
$(document).ready(function() {

	$("#ldp-options-container").hide();
	$("#ldp-options-left").hide();
	$("#ldp-options-right").hide();
	$("#path-error").hide();

	function setoptions_type2(start_options, end_options) {
		for (var i = 0; i < start_options.length; i++) {
			$("#start-cui-select").append('<option value = "' + start_options[i] + '">' + start_options[i] + '</option>');
		}
		for (var i = 0; i < end_options.length; i++) {
			$("#end-cui-select").append('<option value = "' + end_options[i] + '">' + end_options[i] + '</option>');
		}
		$("#ldp-options-left").show();
		$("#ldp-options-right").show();
		$("#ldp-options-container").show();
	}

	function setoptions_type34(type, options) {
		if (type == 3) {
			for (var i = 0; i < options.length; i++) {
				$("#start-cui-select").append('<option value = "' + options[i] + '">' + options[i] + '</option>');
			}
			$("#ldp-options-left").show();
		}
		else if (type == 4) {
			for (var i = 0; i < options.length; i++) {
				$("#end-cui-select").append('<option value = "' + options[i] + '">' + options[i] + '</option>');
			}
			$("#ldp-options-right").show();
		}
		$("#ldp-options-container").show();
	}
	
	function process_error(msg, response, t) {
		$("#path-error").text(msg);
		if (t == 2) {
			setoptions_type2(response.start_option, response.end_option);
		}
		else if (t == 3) {
			setoptions_type34(t, response.start_option);
		}
		else if (t == 4) {
			setoptions_type34(t, response.end_option);
		}
	}
	
	function getcui(x) {
		var reg = /[C]\d{7}/;
		var cui = reg.exec(x);
		if (cui == null) {
			return null;
		}
		return cui[0];
	}
	
	function process_submit_btn(sid, eid) {
		var s_cui = getcui(sid);
		var e_cui = getcui(eid);
		
		var t = 0;
		
		if (s_cui == null && e_cui == null) {
			t = 2;
		}
		else if (s_cui == null) {
			t = 3;
		}
		else if (e_cui == null) {
			t = 4;
		}	
		
		$.ajax({
			url: "process_function1.php",
			beforeSend: function() {
				$('#path-loading').Loadingdotdotdot({
					"speed": 400,
					"maxDots": 4
				});
			},
			complete: function() {
				$("#path-loading").Loadingdotdotdot("Stop");
				$("#path-loading").html("");
			},
			type: 'POST',
			data: { start_id: sid, end_id: eid, type: t },
			dataType: 'json',
			success: function(response) {
				console.log(response);
				if (response == null) {
					$("#path-error").text("There was an error with your request.");
					$("#path-error").show();
				}
				else if (response.hasOwnProperty('dberror')) {
					$("#path-error").text("There was an error with our database.  Please try again later.");
					$("#path-error").show();
				}
				else if (response.hasOwnProperty('error')) {
					if ($.trim(response['error']) == 91)
						process_error("There was an error with your request. The second CUI is isolated.", response, t);
					else if ($.trim(response['error']) == 1)
						process_error("There was an error with your request. The first CUI is isolated.", response, t);
					else if ($.trim(response['error']) == 2)
						process_error("There was an error with your request. The first CUI was not found.", response, t);
					else if ($.trim(response['error']) == 11)
						process_error("There was an error with your request. Both the first and second CUI are isolated.", response, t);
					else if ($.trim(response['error']) == 21)
						process_error("There was an error with your request. The first CUI was not found and the second CUI is isolated.", response, t);
					else if ($.trim(response['error']) == 92)
						process_error("There was an error with your request. The second CUI was not found.", response, t);
					else if ($.trim(response['error']) == 12)
						process_error("There was an error with your request. The first CUI is isolated and the second CUI was not found.", response, t);
					else if ($.trim(response['error']) == 22)
						process_error("There was an error with your request. Both the first and second CUI were not found.", rseponse, t);
					else if ($.trim(response['error']) == 88)
						process_error("There was an error with your request. Could not determine appropriate CUI's for your terms.", response, t);
					else
						$("#path-error").text("There was an error with your request.");
					
					$("#path-error").show();
				}
				else {
					$("#path-error").text("");
					$("#path-error").hide();
					distance = response.distance;
					var paths_str = response.paths;

					if (paths_str === "") {
						$("#path-error").text("The query contains no path.");
						$("#path-error").show();
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
						
						if (t == 2) {
							setoptions_type2(response.start_option, response.end_option);
						}
						else if (t == 3) {
							setoptions_type34(t, response.start_option);
						}
						else if (t == 4) {
							setoptions_type34(t, response.end_option);
						}
					}
				}
			},
			error: function(xhr, textStatus, error){
				alert("There was an error with your request.");
			}
		});
	}
	
	$('#path_submit_btn').click(function(e){
		e.preventDefault();
		$("#display").html("");
		$("#paths").html("");
		$("#path-error").html("");
		$("#path-error").hide();
		$("#start-cui-select").empty();
		$("#end-cui-select").empty();
		$("#ldp-options-left").hide();
		$("#ldp-options-right").hide();
		$("#ldp-options-container").hide();
		$('#graph-options').css("visibility","hidden");
		
		var sid = $('#start_id').val();
		var eid = $('#end_id').val();
		
		process_submit_btn(sid, eid);
	});
	
	
	$('#ldp_options_submit_btn').click(function(e){
		e.preventDefault();
		$("#display").html("");
		$("#paths").html("");
		$("#path-error").html("");
		$("#path-error").hide();
		$('#graph-options').css("visibility","hidden");
		
		var sid, eid;
		
		if ($("#start-cui-select").is(":empty")) {
			sid = $('#start_id').val();
		}
		else {
			if (!$("#start-cui-select option:selected").length) {
				$('#start-cui-select option:first').attr('selected', 'selected');
			}
			sid = $('#start-cui-select').val();
		}
		
		if ($("#end-cui-select").is(":empty")) {
			eid = $('#end_id').val();
		}
		else {
			if (!$("#end-cui-select option:selected").length) {
				$('#end-cui-select option:first').attr('selected', 'selected');
			}
			eid = $('#end-cui-select').val();
		}
		
		process_submit_btn(sid, eid);
	});
	
});              