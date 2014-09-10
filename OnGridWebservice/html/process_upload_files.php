<?php

if (isset($_SESSION["status"]) && isset($_SESSION["email"]) && isset($_SESSION["name"]) && isset($_POST['process-button'])) {

	if (is_uploaded_file($_FILES['row_file']['tmp_name']) && is_uploaded_file($_FILES['col_file']['tmp_name'])) {

		$job_limit = 10;
	
		$job_name = "N/A";
		if (isset($_POST['job_name'])) {
			$job_name = $_POST['job_name'];
			if ($job_name == "") {
				$job_name = "N/A";
			}
		}
	
		$user_email = $_SESSION["email"];
	
		$allowed_filetypes = array('.txt','.dat'); // These will be the types of file that will pass the validation.
		$max_filesize = 2000000;// Maximum filesize in BYTES (currently 2.0MB).
		$upload_path = "/matrix_uploads/";
		
		$special_key = sha1(uniqid(mt_rand(), true));
	 
		$row_filename = $_FILES['row_file']['name']; // Get the name of the file (including file extension).
		$col_filename = $_FILES['col_file']['name']; // Get the name of the file (including file extension).
		$row_ext = substr($row_filename, strpos($row_filename,'.'), strlen($row_filename)-1); // Get the extension from the filename.
		$col_ext = substr($col_filename, strpos($col_filename,'.'), strlen($col_filename)-1); // Get the extension from the filename.
	 
		if(!in_array($row_ext,$allowed_filetypes) || !in_array($col_ext,$allowed_filetypes)) {
			echo "Incorrect filetype";
		}
		else if(filesize($_FILES['row_file']['tmp_name']) > $max_filesize || filesize($_FILES['col_file']['tmp_name']) > $max_filesize) {
			echo "Filesize is too large.";
		}
		else if(!is_writable($upload_path)) {
			echo "Writing error.";
		}
		else {
			if(move_uploaded_file($_FILES['row_file']['tmp_name'], $upload_path . $special_key . "_row.txt") && move_uploaded_file($_FILES['col_file']['tmp_name'], $upload_path . $special_key . "_col.txt")) {
			
				include 'connect.php';

				// check to see if limit is reached
				$sql = "SELECT id, special_key FROM wkdd_matrix_jobs WHERE user_email = '$user_email' ORDER BY start_time ASC";
				$result = mysql_query($sql);
				$num_rows = mysql_num_rows($result);
				if ($num_rows >= $job_limit) {
					$row = mysql_fetch_row($result);
					$old_id = $row[0];
					$old_key = $row[1];

					// remove from database
					$sql = "DELETE FROM wkdd_matrix_jobs WHERE id = '$old_id'";
					mysql_query($sql);		

					// delete local file	
					$result_path = "/matrix_results/";
					unlink($result_path . $old_key . "_contribution.txt");
					unlink($result_path . $old_key . "_distance.txt");
				}
				
				$sql = "INSERT INTO wkdd_matrix_jobs(job_name, row_filename, col_filename, special_key, status, start_time, user_email) VALUES ('$job_name', 'row.txt', 'col.txt', '$special_key', 'pending', NOW(), '$user_email')";
				
				if (mysql_query($sql)) {
					echo "Your job has been successfully submitted.  When it has been completed, you should receive an email notification to download your results from your <a href='history.php'>job history page</a>.";
				}
				else {
					echo "There was an error during the uploading process.";
				}
			
				mysql_close($con);
			}
		}
   }
	else {
		echo 'Error during file upload.';
	}
	
}

?>
