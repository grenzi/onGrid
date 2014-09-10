
//global
var showLinks = false;
var pathList = null;
var distance = null;
var rankList = null;

// returns an array of size 2;  index 0 is the CUI, index 1 is the Name
function getCuiName(x) {
	var reg = /[C]\d{7}[:]/;  // uses the regex "C#######:" to parse the string
	var y = x.match(reg);
	var z = x.split(reg);
	return [(String(y).substring(0, String(y).length-1)), z[1]];
}

// returns an array containing arrays of objects
// Each subarray is a path, each object contains the information about a particular node
// The 'edge' attribute is held in the parent node, so the last node does not have an edge attribute (set as null)
function buildPathList(distance, paths_str) {
	var paths = paths_str.split('$');  // split by '$' character
	var pathList = [];  // array to hold array of paths

	// build each path
	for (i = 0; i < paths.length; i++) {
		var singlePath = []; // subarray which contains a single path
		var token = paths[i].split("\t");  // split by tab
		for (j = 0; j <= distance; j++) {
			var cn = getCuiName(token[j*2]);
			var l = null;
			if (j < distance)
				l = token[(j*2)+1];
			singlePath[j] = {cid : cn[0], name : cn[1], link : l};
		}
		pathList[i] = singlePath;
	}
	return pathList;
}
	
// returns an HTML representation of the pathList to be displayed
function buildPathHTML(pathList) {
	var buildStr = "";
	for (i = 0; i < pathList.length; i++) {   // loop through each path
		buildStr += "<h13>"
		for (j = 0; j < pathList[i].length; j++) {  // loop through each node in the path
			var cid = pathList[i][j].cid;
			var name = pathList[i][j].name;
			var link = "";
			if (pathList[i][j].link != null)
				link = pathList[i][j].link;
			buildStr += cid + ":" + name + "\t" + link + "\t";   // format the display of the path
		}
		buildStr += "</h13><br>";
	}
	return buildStr;
}

// returns true if the list contains the vertex specified by cid
function containsNode(list, cid) {
	var found = false;
	for (var i = 0; i < list.length; i++) {
		if (list[i].cid == cid) {
			found = true;
			break;
		}
	}
	return found;
}

// returns true if the list contains the edge specified by source and target cid
function containsLink(list, source, target) {
	var found = false;
	for (var i = 0; i < list.length; i++) {
		if (list[i].source == source && list[i].target == target) {
			found = true;
			break;
		}
	}
	return found;
}

function getRankColor(cui, rankList) {
	var rank = parseInt(rankList[cui], 10);
	switch(rank) {
		case 1: return d3.rgb("#00FF00");
		case 2: return d3.rgb("#87F717");
		case 3: return d3.rgb("#CCFB5D");
		case 4: return d3.rgb("#FAAFBE");
		case 5: return d3.rgb("#F75D59");
		case 6: return d3.rgb("#FF0000");
		default: return d3.rgb("#E5E5E5");
	}
}

// returns structured 2d array representation of vertices.
function getStructuredNodeList(pathList, distance, rankList) {
	var structuredNodeList = [];
	for (var i = 0; i < eval(distance)+1; i++) {
		structuredNodeList.push(new Array());
	}
	for (var i = 0; i < pathList.length; i++) {
		for (var j = 0; j < pathList[i].length; j++) {
			if (!containsNode(structuredNodeList[j], pathList[i][j].cid)) {
				var rankColor = getRankColor(pathList[i][j].cid, rankList);
				var v = {cid : pathList[i][j].cid, name : pathList[i][j].name, group : j, size: 15, color: rankColor};
				//if (j == 0 || j == pathList[i].length-1)
				//	v.size = 20;
				structuredNodeList[j].push(v);
			}
		}
	}
	return structuredNodeList;
}

// returns single list of nodes from structuredNodeList
function getSingleNodeList(structuredNodeList) {
	var nodeList = new Array();
	for (var i = 0; i < structuredNodeList.length; i++) {
		for (var j = 0; j < structuredNodeList[i].length; j++) {
			nodeList.push(jQuery.extend({},structuredNodeList[i][j]));  // clean clone of object 
		}
	}
	return nodeList;
}

// returns set of edges between vertices
function getLinkList(pathList) {
	var linkList = new Array();
	var weight = 1;  // assume equally weighted
	for (var i = 0; i < pathList.length; i++) {
		for (var j = 0; j < pathList[i].length-1; j++) {
			if (!containsLink(linkList, pathList[i][j].cid, pathList[i][j+1].cid)) {
				var e = {source : pathList[i][j].cid, target : pathList[i][j+1].cid, link : pathList[i][j].link, weight : 2, srcNode : null, tgtNode : null};
				linkList.push(e);
			}
		}
	}
	return linkList;
}

// returns max value of vertices from subarray of vertexList
function getMaxHeight(nodeList) {
	var max = nodeList[0].length;
	for (var i = 0; i < nodeList.length; i++) {
		if (nodeList[i].length > max)
			max = nodeList[i].length;
	}
	return max;
}

// set coordinates for each vertex
function setVertexCoordinates(nodeList, width, height) {
	var x, y;
	var widthCell = width / nodeList.length;
	var heightCell;
	var startX = widthCell / 2 ;
	var startY;
	for (var i = 0; i < nodeList.length; i++) {
		x = startX + (widthCell * i);
		heightCell = height / nodeList[i].length;
		startY = heightCell / 2;

		for (var j = 0; j < nodeList[i].length; j++) {
			y = startY + (heightCell * j);
			nodeList[i][j].x = x;
			nodeList[i][j].y = y;			
		}
	}
}

function setLinkCoordinates(nodeList, linkList) {
	for (var i = 0; i < linkList.length; i++) {
		var srcNode = getNode(nodeList, linkList[i].source);
		var tgtNode = getNode(nodeList, linkList[i].target);
		linkList[i].srcNode = srcNode;
		linkList[i].tgtNode = tgtNode;
	}
}

// returns node specified by cid
function getNode(nodeList, cid) {
	var node = null;
	for (var i = 0; i < nodeList.length; i++) {
		if (nodeList[i].cid == cid) {
				node = nodeList[i];
				break;
		}
	}
	return node;
}

// updates position of link text
function updateLinkCoordinates() {
	linkText
		.attr('x', function(d) { 
			if (d.tgtNode.x > d.srcNode.x) { 
				return ( d.srcNode.x + (d.tgtNode.x - d.srcNode.x)/2); 
			}
			else { 
				return (d.tgtNode.x + (d.srcNode.x - d.tgtNode.x)/2); 
			}
		})
		.attr('y', function(d) { 
			if (d.tgtNode.y > d.srcNode.y) { 
				return ( d.srcNode.y + (d.tgtNode.y - d.srcNode.y)/2); 
			}
	   		else { 
	   			return (d.tgtNode.y + (d.srcNode.y - d.tgtNode.y)/2); 
	   		}
		});
}

// generate visualization
function render(nodeList, linkList, width, height) {

	//var color = d3.scale.category10();

	var svg = d3.select("#display").append("svg")
		.attr("width", width)
		.attr("height", height);

	var drag = d3.behavior.drag()
		.origin(Object)
		.on("drag", function(d) {
			d.x = d3.event.x;
			d.y = d3.event.y;
			draw();
		});

	var link = svg.selectAll(".link")
		.data(linkList)
		.enter().append("g")
			.attr("class", "gLink")
		.append("line")
			.attr("class", "link")
			.attr("stroke", "blue")
			.style("stroke-width", function(d) { return d.weight; });

	linkText = svg.selectAll(".gLink")
		.data(linkList)
		.append("text")
		.attr('text-anchor', 'middle')
		.attr("class", "linkText")
		.attr("fill", "black")
		.text(function(d) { return d.link; } );

	var node = svg.selectAll(".node")
		.data(nodeList)
		.enter().append("g")
		.attr("class", "node")
		.call(drag);

	node.append("circle")
		.attr("r", function(d) { return d.size; })
		.style("fill", function(d) { return d.color; });//color(d.group); })

	node.append("text")
    	.attr("text-anchor", "middle")
    	.attr("fill", "black")
		.text(function(d) { return d.name });

	$('circle').powerTip({
		placement: 'e',
		fadeInTime: 75	
	});

	$('circle').data('powertip', function() {
		var d = this.__data__;
		return d.cid + '<br>' + d.name;
	});

	$('.link').powerTip({
		placement: 'e',
		fadeInTime: 75,
		followMouse: true
	});

	$('.link').data('powertip', function() {
		var d = this.__data__;
		return d.link;
	});

	draw();
	
	function draw() {
		if (showLinks) {
			linkText.style("visibility", "visible");
			updateLinkCoordinates();
		}
		else {
			linkText.style("visibility", "hidden");	
		}
	
		link.attr("x1", function(d) { return d.srcNode.x; })
			.attr("y1", function(d) { return d.srcNode.y; })
			.attr("x2", function(d) { return d.tgtNode.x; })
			.attr("y2", function(d) { return d.tgtNode.y; });
	
		node.attr("transform", function(d) { return "translate(" + d.x + "," + d.y + ")"; });
	}
}

$(document).ready(function() {

	$('#showLinks').change(function() {
	    if($(this).is(":checked")) {
	    	showLinks = true;
	    	if (linkText != null) {
	    		linkText.style("visibility", "visible");
	    		updateLinkCoordinates();
	    	}
	    }
	    else {
	    	showLinks = false;
	    	if (linkText != null) {
	    		linkText.style("visibility", "hidden");
	    		updateLinkCoordinates();
	    	}
	    }
	});

	$('#reset_btn').click(function(e) {
		e.preventDefault();
		if (pathList != null && distance != null && rankList != null) {
			$('#display').html('');
			var structuredNodeList = getStructuredNodeList(pathList, distance, rankList);
			var linkList = getLinkList(pathList);
			var width = $("#display").width();
			var height = getMaxHeight(structuredNodeList) * 100;
			setVertexCoordinates(structuredNodeList, width, height);
			var nodeList = getSingleNodeList(structuredNodeList);
			setLinkCoordinates(nodeList, linkList);
			render(nodeList, linkList, width, height);
		}
	});
	
});