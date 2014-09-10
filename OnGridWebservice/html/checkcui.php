<?php

	function get_cui($str) {
		preg_match_all('/[C]\d{7}/', $str, $matches);
		$cui_list = array_values(array_unique($matches[0]));
		return $cui_list;
	}
	
	function get_ldp_cui($str) {
		$sql = "INSERT INTO wkdd_ldp_jobs(term, status, type, start_time) VALUES ('$str', 'pending', 1, NOW())";
		$return_val = "error";
		
		if (mysql_query($sql)) {
			$id = mysql_insert_id(); // get id generated in last query
			sleep(3);
				
			$count = 0;
			while ($count < 120) {
				// check for result
				$sql = "SELECT top_cui FROM wkdd_ldp_jobs WHERE id = '$id' AND status = 'complete'";
				$result = mysql_query($sql);
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

	if (isset($_POST['row']) && isset($_POST['col'])) {
	
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

		$row_in = preg_split("/[\r\n,]+/", trim($_POST['row']), -1, PREG_SPLIT_NO_EMPTY);
		$col_in = preg_split("/[\r\n,]+/", trim($_POST['col']), -1, PREG_SPLIT_NO_EMPTY);
		
		// change limit restrictions here.
		if ((count($row_in) > 25) || (count($col_in) > 25)) {
			$limit = array('limit' => -1);
			echo json_encode($limit);
		}
		else {
			$a = "";
			include 'connect.php';
			
			$row_list = array();
			$col_list = array();
			
			$row_ldp_str = "";
			$col_ldp_str = "";
			
			$row_count = 0;
			$col_count = 0;
			
			$row_cui_position = array();
			$col_cui_position = array();
			
			$position_count = 0;
			foreach ($row_in as $val) {
				$temp_cui = get_cui($val);
				if (sizeof($temp_cui) == 0) {  // not a cui
					$clean_val = str_replace("$", " ", $val);
					$row_ldp_str .= $clean_val . "$";
					$row_count++;
				}
				else {
					//array_push($row_list, $temp_cui[0]);
					$row_cui_position[$position_count] = $temp_cui[0];
				}
				$position_count++;
			}
			
			$position_count = 0;
			foreach ($col_in as $val) {
				$temp_cui = get_cui($val);
				if (sizeof($temp_cui) == 0) {  // not a cui
					$clean_val = str_replace("$", " ", $val);
					$col_ldp_str .= $clean_val . "$";
					$col_count++;
				}
				else {
					//array_push($col_list, $temp_cui[0]);
					$col_cui_position[$position_count] = $temp_cui[0];
				}
				$position_count++;
			}
			
			$pass = true;
			
			if (($row_ldp_str != "") && ($col_ldp_str != "")) { // both group1 and group2 need cui
				// remove trailing $
				$row_ldp_str = substr($row_ldp_str, 0, -1);
				$col_ldp_str = substr($col_ldp_str, 0, -1);
				$ldp_result = get_ldp_cui($row_ldp_str . "$" . $col_ldp_str);
				if ($ldp_result == "error") {
					$a = array('error' => 88);
					$pass = false;
				}
				else {
					$ldp_list = explode("$", $ldp_result);
					for($i = 0; $i < count($ldp_list); $i++) {
						if ($i < $row_count) {
							$temp_cui = get_cui($ldp_list[$i]);
							array_push($row_list, $temp_cui[0]);
						}
						else {
							$temp_cui = get_cui($ldp_list[$i]);
							array_push($col_list, $temp_cui[0]);
						}
					}
				}
			}
			else if ($row_ldp_str != "") { // only group1 needs cui
				// remove trailing $
				$row_ldp_str = substr($row_ldp_str, 0, -1);
				$ldp_result = get_ldp_cui($row_ldp_str);
				if ($ldp_result == "error") {
					$a = array('error' => 88);
					$pass = false;
				}
				else {
					$ldp_list = explode("$", $ldp_result);
					foreach ($ldp_list as $val) {
						$temp_cui = get_cui($val);
						array_push($row_list, $temp_cui[0]);
					}
				}
			}
			else if ($col_ldp_str != "") { // only group2 needs cui
				// remove trailing $
				$col_ldp_str = substr($row_ldp_str, 0, -1);
				$ldp_result = get_ldp_cui($row_ldp_str);
				if ($ldp_result == "error") {
					$a = array('error' => 88);
					$pass = false;
				}
				else {
					$ldp_list = explode("$", $ldp_result);
					foreach ($ldp_list as $val) {
						$temp_cui = get_cui($val);
						array_push($col_list, $temp_cui[0]);
					}
				}
			}
			
			// add cui into correct position
			foreach ($row_cui_position as $x => $x_val) {
				array_splice($row_list, $x, 0, $x_val);
			}
			
			foreach ($col_cui_position as $x => $x_val) {
				array_splice($col_list, $x, 0, $x_val);
			}
			
			if ($pass) {
				//ksort($row_list);
				//ksort($col_list);
			
				$row_cui_query = array();
				$row_cui_isolated = array();
				$col_cui_query = array();
				$col_cui_isolated = array();
				
				$row_name_query = array();
				$row_name_isolated = array();
				$col_name_query = array();
				$col_name_isolated = array();
			
				foreach ($row_list as $val) {
					$sql = "SELECT name FROM wkdd_connected_labelsxref WHERE cui = '$val'";
					$result = mysql_query($sql);
				
					if (mysql_affected_rows() > 0) {
						$row = mysql_fetch_row($result);
						array_push($row_cui_query, $val);
						array_push($row_name_query, $row[0]);
					}
					else {
						$sql = "SELECT name FROM wkdd_isolated_labels WHERE cui = '$val'";
						$result = mysql_query($sql);
					
						if (mysql_affected_rows() > 0) {
							$row = mysql_fetch_row($result);
							array_push($row_cui_isolated, $val);
							array_push($row_name_isolated, $row[0]);
						}
					}
				}
			
				foreach ($col_list as $val) {
					$sql = "SELECT name FROM wkdd_connected_labelsxref WHERE cui = '$val'";
					$result = mysql_query($sql);
				
					if (mysql_affected_rows() > 0) {
						$row = mysql_fetch_row($result);
						array_push($col_cui_query, $val);
						array_push($col_name_query, $row[0]);
					}
					else {
						$sql = "SELECT name FROM wkdd_isolated_labels WHERE cui = '$val'";
						$result = mysql_query($sql);
					
						if (mysql_affected_rows() > 0) {
							$row = mysql_fetch_row($result);
							array_push($col_cui_isolated, $val);
							array_push($col_name_isolated, $row[0]);
						}
					}
				}
			
				$a = array($row_cui_query, $row_name_query, $row_cui_isolated, $row_name_isolated, $col_cui_query, $col_name_query, $col_cui_isolated, $col_name_isolated);
			}

			echo json_encode($a);
			mysql_close($con);
		}
		
	}
	
?>
