<?php
$token = $argv[1];
$url_api = $argv[2];
$keep_orig_file = $argv[3];
//$dirpath = $argv[4];
//$outputdir = $argv[5];
$depth = $argv[4];
$root_dir = $argv[5];
$images_path = $argv[6];
$dirlist = getFileList($images_path,true, $depth);
//count jpeg file
//$i=0;
//unlink($root_dir."count.txt");
//unlink($root_dir."error.log");
//foreach($dirlist as $file) {
//        if($file['type'] != 'image/jpeg') continue;
//	 $i++;
 // }
//error_log("Total: $i", 3, "$root_dir/count.txt");
foreach($dirlist as $file) {
  	if($file['type'] != 'image/jpeg') continue;
	//$outputuploadfile = $outputdir.$file['name'];
	//echo $outputuploadfile;
	//echo "\n";
	$uploadfile = $file['name'];
  $filename = $file['name'];
       error_log("uploadfile: $uploadfile \n", 3, "$root_dir/error.log");
  //error_log("Call visqua_compress \n", 3, "/home/www/dev.visqua.com/error.log");
	//visqua_compress($url_api,$token,$root_dir,$uploadfile,$keep_orig_file);
  }

function visqua_compress($url,$token,$root_dir,$uploadfile,$keep_orig_file='no')
{
    if ($keep_orig_file == 'yes'){    
      $original_files = $root_dir."original_files/";
      $path_full = str_replace($root_dir, $original_files, $uploadfile);
      $path_parts = pathinfo($path_full);
      $new_path = $path_parts['dirname'];
      if (!is_dir($new_path)){
        mkdir($new_path, 0777, true);
      }
      copy($uploadfile,$path_full);
    }     
    //error_log("root_dir: $root_dir \n", 3, "/home/www/dev.visqua.com/error.log");
    $exif_data = exif_read_data($uploadfile);
      $esoftware =$exif_data['Software'];
      if ($esoftware != "Compressed by Visqua"){
    	$file_visqua = fopen($uploadfile.'.tmp.visqua', 'w');
          $ch = curl_init($url);
          curl_setopt($ch, CURLOPT_POST, 1);
          curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
          curl_setopt($ch, CURLOPT_FILE, $file_visqua);
          curl_setopt($ch, CURLOPT_POSTFIELDS, array ('userfile'=>"@$uploadfile", 'token'=>"$token"));
          curl_setopt($ch, CURLOPT_HEADER, 0); 
          $postResult = curl_exec($ch);
          $status=curl_getinfo($ch, CURLINFO_HTTP_CODE);
          curl_close($ch);
          fclose($file_visqua);
        if (($status == 200)){
		      copy($uploadfile.'.tmp.visqua', $uploadfile);
		      unlink($uploadfile.'.tmp.visqua');
        }
        else {
     		unlink($uploadfile.'.tmp.visqua');
        }
      }
}
function getFileList($dir, $recurse=false, $depth=false)
  {
    // array to hold return value
    $retval = array();

    // add trailing slash if missing
    if(substr($dir, -1) != "/") $dir .= "/";

    // open pointer to directory and read list of files
    $d = @dir($dir) or die("getFileList: Failed opening directory $dir for reading");
    while(false !== ($entry = $d->read())) {
      // skip hidden files
      if($entry[0] == ".") continue;
      if(is_dir("$dir$entry")) {
        $retval[] = array(
	  "dir" => "$dir",
          "name" => "$dir$entry/",
          "type" => filetype("$dir$entry"),
          "size" => 0,
          "lastmod" => filemtime("$dir$entry")
        );
        if($recurse && is_readable("$dir$entry/")) {
          if($depth === false) {
            $retval = array_merge($retval, getFileList("$dir$entry/", true));
          } elseif($depth > 0) {
            $retval = array_merge($retval, getFileList("$dir$entry/", true, $depth-1));
          }
        }
      } elseif(is_readable("$dir$entry")) {
        $retval[] = array(
	  "dir" => "$dir",
          "name" => "$dir$entry",
          //"type" => getimagesize("$dir$entry"),
          //"type" => mime_content_type("$dir$entry"),
          "type" => finfo_file(finfo_open(FILEINFO_MIME_TYPE),"$dir$entry"),
          "size" => filesize("$dir$entry"),
          "lastmod" => filemtime("$dir$entry")
        );
      }
    }
    $d->close();

    return $retval;
}
function read_dir($dir, $array = array()){ 
        $dh = opendir($dir); 
        $files = array(); 
        while (($file = readdir($dh)) !== false) { 
            $flag = false; 
            if($file !== '.' && $file !== '..' && !in_array($file, $array) && $file !=='.php') { 
                $files[] = $file; 
            } 
        } 
        return $files; 
    } 
//$dirlist = getFileList("./",true, 1);
?>
