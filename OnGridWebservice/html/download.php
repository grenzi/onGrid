<?php   

function readfile_chunked($filename, $retbytes=true) { 
   $chunksize = 1*(1024*1024); // how many bytes per chunk 
   $buffer = ''; 
   $cnt =0;
   $handle = fopen($filename, 'rb'); 
   if ($handle === false) { 
       return false; 
   } 
   while (!feof($handle)) { 
       $buffer = fread($handle, $chunksize); 
       echo $buffer; 
       ob_flush(); 
       flush(); 
       if ($retbytes) { 
           $cnt += strlen($buffer); 
       } 
   } 
       $status = fclose($handle); 
   if ($retbytes && $status) { 
       return $cnt; // return num. bytes delivered like readfile() does. 
   } 
   return $status; 
}

$dir_base="/matrix_results/";
$file_name=$_GET['download_file'];
$fullPath= $dir_base.$file_name;
header("Expires: 0");   
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");   
header("Cache-Control: no-store, no-cache, must-revalidate");   
header("Cache-Control: post-check=0, pre-check=0", false);   
header("Pragma: no-cache");   
header("Content-type: text/plain");   
// tell file size   
header('Content-length: '.filesize($dir_base.$file_name));   
// set file name   
header('Content-disposition: attachment; filename='.basename($dir_base.$file_name));   
//readfile($dir_base.$file_name);   
readfile_chunked($dir_base.$file_name);
// Exit script. So that no useless data is output-ed.   
exit;
?>  

