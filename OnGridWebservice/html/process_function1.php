<?php

	function process_cui($scui, $ecui) {
		$a;
	
		$sid = "";
		$eid = "";
		$not_found = 9;  // 9 indicates both CUI are valid/connected
		
		$sql = "SELECT idxref FROM wkdd_connected_labelsxref WHERE cui = '$scui'";
		$result = mysql_query($sql);
		if (mysql_affected_rows() > 0) {
			$row = mysql_fetch_row($result);
			$sid = $row[0];
		}
		else {
			// check if first entry is isolated CUI
			$sql = "SELECT id FROM wkdd_isolated_labels WHERE cui = '$scui'";
			$result = mysql_query($sql);
			if (mysql_affected_rows() > 0) {
				$not_found = 1;
			}
			// first CUI is not in database
			else {
				$not_found = 2;
			}
		}
		
		$sql = "SELECT idxref FROM wkdd_connected_labelsxref WHERE cui = '$ecui'";
		$result = mysql_query($sql);
		if (mysql_affected_rows() > 0) {
			$row = mysql_fetch_row($result);
			$eid = $row[0];
		}
		else {
			// check if second entry is isolated CUI
			$sql = "SELECT id FROM wkdd_isolated_labels WHERE cui = '$ecui'";
			$result = mysql_query($sql);
			if (mysql_affected_rows() > 0) {
				if ($not_found == 9)
					$not_found = 91;  // first CUI is ok, second is isolated
				else if ($not_found == 1)
					$not_found = 11;  // first CUI is isolated, second is isolated
				else if ($not_found == 2)
					$not_found = 21;  // first is not found, second is isolated
			}
			// second CUI is not in database
			else {
				if ($not_found == 9)
					$not_found = 92;  // first CUI is ok, second is not found
				else if ($not_found == 1)
					$not_found = 12;  // first CUI is isolated, second is not found
				else if ($not_found == 2)
					$not_found = 22;  // first is not found, second is not found
			}
		}
		
		if ($not_found == 9) {  // everything is ok
		
			$sql = "INSERT INTO wkdd_single_jobs(start_id, end_id, status, start_time) VALUES ('$sid', '$eid', 'pending', NOW())";
			
			if (mysql_query($sql)) {
				$id = mysql_insert_id(); // get id generated in last query
				sleep(3);
				
				$count = 0;
				while ($count < 3) {
					// check for result
					$sql = "SELECT distance, paths FROM wkdd_single_jobs WHERE id = '$id' AND status = 'complete'";
					$result = mysql_query($sql);
					if (mysql_affected_rows() > 0) {
						$row = mysql_fetch_row($result);

						// get semantic rankings for cui's
						$rank_list = get_rank_list($row[1]);
						// $out = ......
						$a = array('distance' => $row[0], 'paths' => $row[1], 'ranklist' => $rank_list);
						//echo json_encode($out);
						break;
					}
					
					// check for failure
					$sql = "SELECT distance FROM wkdd_single_jobs WHERE id = '$id' AND status = 'rejected'";
					$result = mysql_query($sql);
					if (mysql_affected_rows() > 0) {
						$row = mysql_fetch_row($result);
						$a = "error";
						//echo "error";
						break;
					}
					
					sleep(2);
					$count++;
				}
			}
			else {
				$a = array('dberror' => -1);
				//echo json_encode($a);
			}
		}
		else if ($not_found == 91) {  // first is ok, second is isolated
			$a = array('error' => 91);
			//echo json_encode($a);
		}
		else if ($not_found == 11) {  // first is isolated, second is isolated
			$a = array('error' => 11);
			//echo json_encode($a);
		}
		else if ($not_found == 1) {  // first is isolated, second is ok
			$a = array('error' => 1);
			//echo json_encode($a);
		}
		else if ($not_found == 21) {  // first is not found, second is isolated
			$a = array('error' => 21);
			//echo json_encode($a);
		}
		else if ($not_found == 92) {  // first is ok, second is not found
			$a = array('error' => 92);
			//echo json_encode($a);
		}
		else if ($not_found == 12) {  // first is isolated, second is not found
			$a = array('error' => 12);
			//echo json_encode($a);
		}
		else if ($not_found == 22) {  // first is not found, second is not found
			$a = array('error' => 22);
			//echo json_encode($a);
		}
		else if ($not_found == 2) {	// first is not found, second is ok
			$a = array('error' => 2);
		}
		else {
			//echo "this shouldn't occur";
		}
		return $a;
	}
	
	function get_ldp_cui_type34($str, $type) {
		$return_val = "error";
		$sql_insert = "INSERT INTO wkdd_ldp_jobs(term, status, type, start_time) VALUES ('$str', 'pending', '$type', NOW())";
		$sql_select;
		
		if (mysql_query($sql_insert)) {
			$id = mysql_insert_id(); // get id generated in last query
			if ($type == 3) {
				$sql_select = "SELECT start_cui_option FROM wkdd_ldp_jobs WHERE id = '$id' AND status = 'complete'";
			}
			else if ($type == 4) {
				$sql_select = "SELECT end_cui_option FROM wkdd_ldp_jobs WHERE id = '$id' AND status = 'complete'";
			}
			else {
				return $return_val;
			}
			
			sleep(3);
				
			$count = 0;
			while ($count < 10) {
				// check for result
				$result = mysql_query($sql_select);
				if (mysql_affected_rows() > 0) {
					$row = mysql_fetch_row($result);
					$return_val = $row[0];
					break;
				}
					
				sleep(2);
				$count++;
			}
		}
		
		return $return_val;
	}

	function get_ldp_cui_type2($str) {
		$return_val1 = "error";
		$return_val2 = "error";
		$sql_insert = "INSERT INTO wkdd_ldp_jobs(term, status, type, start_time) VALUES ('$str', 'pending', 2, NOW())";
		
		if (mysql_query($sql_insert)) {
			$id = mysql_insert_id(); // get id generated in last query
			$sql_select = "SELECT start_cui_option, end_cui_option FROM wkdd_ldp_jobs WHERE id = '$id' AND status = 'complete'";
			sleep(3);
				
			$count = 0;
			while ($count < 10) {
				// check for result
				$result = mysql_query($sql_select);
				if (mysql_affected_rows() > 0) {
					$row = mysql_fetch_row($result);
					$return_val1= $row[0];
					$return_val2 = $row[1];
					break;
				}
					
				sleep(2);
				$count++;
			}
		}
		
		return array($return_val1, $return_val2);
	}

	function get_cui($str) {
		preg_match_all('/[C]\d{7}/', $str, $matches);
		$cui_list = array_values(array_unique($matches[0]));
		return $cui_list;
	}

	function get_rank_list($str) {
		preg_match_all('/[C]\d{7}[:]/', $str, $matches);
		$cui_list = array_values(array_unique($matches[0]));
		$rank_list = array();

		foreach ($cui_list as $cui) {
			$cui = substr($cui, 0, -1);  // remove ':' character
			$rows = array();
			$sql = "SELECT semantic_type FROM wkdd_semantic_labels WHERE cui = '$cui'";
			$type_result = mysql_query($sql);
			
			// store all semantic_types
			while (($row = mysql_fetch_array($type_result))) {
				$rows[] = $row;
			}

			$rank = 10;
			// get rank for each type, store lowest one
			foreach ($rows as $type) {
				$temp = $type[0];
				$sql = "SELECT ranking FROM wkdd_semantic_ranking WHERE semantic_type = '$temp'";
				$rank_result = mysql_query($sql);
				$row = mysql_fetch_row($rank_result);
				$temp_rank = $row[0];
				if ($temp_rank < $rank)
					$rank = $temp_rank;
			}
			$rank_list[$cui] = $rank;
		}
		return $rank_list;
	}

	// load json functions
	if (!function_exists('json_decode')) {
		function json_decode($content, $assoc=false) {
			require_once 'JSON.php';
			if ($assoc) {
				$json = new Services_JSON(SERVICES_JSON_LOOSE_TYPE);
			}
			else {
				$json = new Services_JSON;
			}
			return $json->decode($content);
		}
	}

	if (!function_exists('json_encode')) {
		function json_encode($content) {
			require_once 'JSON.php';
			$json = new Services_JSON;
			return $json->encode($content);
		}
	}

	if (isset($_POST['start_id']) && isset($_POST['end_id']) && isset($_POST['type'])) {
		$a = "";
		$start_option = "";
		$end_option = "";
		
		include 'connect.php';
		$scui = $_POST['start_id'];
		$ecui = $_POST['end_id'];
		$type = $_POST['type'];

		$scui = trim($scui);
		$ecui = trim($ecui);
		$type = trim($type);
		
		$pass = false;
		
		if ($type == 0) {  // both terms are cui
			$start = get_cui($scui);
			$end = get_cui($ecui);
			$scui = $start[0];
			$ecui = $end[0];
			$pass = true;
		}
		else if ($type == 1) {  // this case is handled in checkcui.php, should not occur here
		}
		else if ($type == 2) {  // neither terms are cui
			$clean_scui = str_replace("$", " ", $scui);  // don't allow $ character in string
			$clean_ecui = str_replace("$", " ", $ecui);
			$ldp_result = get_ldp_cui_type2($clean_scui . "$" . $clean_ecui);
			
			if ($ldp_result[0] == "error") {
				$a = array('error' => 88);
			}
			else {
				$start_option = explode("$", $ldp_result[0]);
				$end_option = explode("$", $ldp_result[1]);
				$ldp_start = get_cui($start_option[0]);
				$ldp_end = get_cui($end_option[0]);
				$scui = $ldp_start[0];
				$ecui = $ldp_end[0];
				$pass = true;
			}
		}
		else if ($type == 3) {  // second term is cui, need to get first term
			$clean_scui = str_replace("$", " ", $scui);
			$ldp_result = get_ldp_cui_type34($clean_scui, $type);
			
			if ($ldp_result == "error") {
				$a = array('error' => 88);
			}
			else {
				$start_option = explode("$", $ldp_result);
				$ldp_start = get_cui($start_option[0]);
				$scui = $ldp_start[0];
				$pass = true;
			}
		}
		else if ($type == 4) {  // first term is cui, need to get second term
			$clean_ecui = str_replace("$", " ", $ecui);
			$ldp_result = get_ldp_cui_type34($clean_ecui, $type);
			
			if ($ldp_result == "error") {
				$a = array('error' => 88);
			}
			else {
				$end_option = explode("$", $ldp_result);
				$ldp_end = get_cui($end_option[0]);
				$ecui = $ldp_end[0];
				$pass = true;
			}
		}
		
		if ($pass) {
			$a = process_cui($scui, $ecui);
		}
		
		$error = false;
		$b;
		
		if (array_key_exists("error", $a)) {
			$error = true;
		}
		
		if ($type == 0 || $type == 1) {
			echo json_encode($a);
		}
		if ($type == 2) {
			if ($error)
				$b = array('error' => $a['error'], 'start_option' => $start_option, 'end_option' => $end_option);
			else
				$b = array('distance' => $a['distance'], 'paths' => $a['paths'], 'ranklist' => $a['ranklist'], 'start_option' => $start_option, 'end_option' => $end_option);
			echo json_encode($b);
		}
		else if ($type == 3) {
			if ($error)
				$b = array('error' => $a['error'],'start_option' => $start_option);
			else
				$b = array('distance' => $a['distance'], 'paths' => $a['paths'], 'ranklist' => $a['ranklist'],'start_option' => $start_option);
			echo json_encode($b);
		}
		else if ($type == 4) {
			if ($error)
				$b = array('error' => $a['error'], 'end_option' => $end_option);
			else
				$b = array('distance' => $a['distance'], 'paths' => $a['paths'], 'ranklist' => $a['ranklist'], 'end_option' => $end_option);
			echo json_encode($b);
		}
		
		mysql_close($con);
	}
?>
