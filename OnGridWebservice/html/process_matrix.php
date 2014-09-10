<?php

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
	
		include 'connect.php';
		$row = $_POST['row'];
		$col = $_POST['col'];
		
		$path = "/matrix_temporary_uploads/";
		$special_key = sha1(uniqid(mt_rand(), true));
		$row_filename = $special_key . "_row.txt";
		$col_filename = $special_key . "_col.txt";
		
		// write files
		$f_row = fopen($path . $row_filename, 'w') or die("Can't open file.");
		fwrite($f_row, $row);  // assume $row is right format (new line for each cui)
		fclose($f_row);
		
		$f_col = fopen($path . $col_filename, 'w') or die("Can't open file.");
		fwrite($f_col, $col);
		fclose($f_col);
		
		$sql = "INSERT INTO wkdd_matrix_jobs(row_filename, col_filename, special_key, status, start_time) VALUES ('row.txt', 'col.txt', '$special_key', 'pending', NOW())";
				
		if (mysql_query($sql)) {
			$id = mysql_insert_id();
			$count = 0;
			$completed = false;
			while ($count < 4) {
				sleep(3);
				$sql = "SELECT special_key FROM wkdd_matrix_jobs WHERE id = '$id' AND status = 'complete'";
				$result = mysql_query($sql);
				if (mysql_affected_rows() > 0) {
					$completed = true;
					$resultpath = "/matrix_temporary_results/";
					$con_name = $special_key . "_contribution.txt";
					$dist_name = $special_key . "_distance.txt";
					$con_array = file($resultpath . $con_name, FILE_IGNORE_NEW_LINES);
					$dist_array = file($resultpath . $dist_name, FILE_IGNORE_NEW_LINES);
					
					$con_array = array_splice($con_array, 1);
					$dist_array = array_splice($dist_array, 1);
					$con_out = array();
					$dist_out = array();
					foreach ($con_array as $qq) {
						array_push($con_out, substr($qq, 9));
					}
					foreach ($dist_array as $qq) {
						array_push($dist_out, substr($qq, 9));
					}
					echo json_encode(array($con_out, $dist_out));  						// echo results
					break;
				}
				$count++;
			}
			if (!$completed) {
				echo "error";
			}
		}
		else {
			echo "error";
		}
		
		mysql_close($con);
   }

?>
