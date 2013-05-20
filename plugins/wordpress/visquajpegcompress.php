<?php
/*
Plugin Name: VisquaJPEGCompress
Plugin URI: http://visqua.com 
Description: This plugin compress uploaded images after uploading, discarding the original uploaded file in the process.
Author: visqua
Version: 1.0
Author URI: http://visqua.com

Copyright (C) 2012 Visqua

Includes hints and code by:
	
  Quang Ho-Ngoc (http://visqua.com)  

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

$version = get_option('vq_compress_version');
if ($version == '') {
  	add_option('vq_compress_version','1.0','Version of the plugin VisquaJPEGCompress','yes');
  	add_option('vq_compress_yesno','yes','Compress image if yes (set to no instead of unloading the plugin)','yes');
  	add_option('vq_compress_token','','Token','');
	add_option('vq_compress_url_api','','URL API','http://api.visqua.com');
	add_option('vq_compress_keep_orig_file','','Keep original file','');
	add_option('vq_compress_scan_images','','Scan all images in your website for compress','yes');
}  
/* actions */
add_action( 'admin_menu', 'vq_compress_options_page' ); // add option page
if (get_option('vq_compress_yesno') == 'yes') {
  	add_action('wp_handle_upload', 'vq_compress_upload'); // apply our modifications
  	add_filter( 'jpeg_quality', 'jpeg_full_quality' );
  	add_filter( 'image_send_to_editor', 'visqua', 10, 7);
}
/* add option page */
function vq_compress_options_page(){
  	if(function_exists('add_options_page')){
    	add_options_page('VisquaJPEGCompress','VisquaJPEGCompress','manage_options','visqua-jpeg-compress','vq_compress_options');
  }
}
/* the real option page */
function vq_compress_options(){
  if (isset($_POST['vq_options_update'])){
	$yesno = $_POST['yesno'];
	$token = $_POST['token'];
	$url_api = $_POST['url_api'];
	$keep_orig_file = $_POST['keep_orig_file'];
	$scan_images = $_POST['scan_images'];
    update_option('vq_compress_token',$token);
	update_option('vq_compress_url_api',$url_api);
	update_option('vq_compress_keep_orig_file',$keep_orig_file);
    if ($yesno == 'yes') {
      update_option('vq_compress_yesno','yes');
    }
    else {
      update_option('vq_compress_yesno','no');
    }
    if ($scan_images == 'yes'){
    	echo ('<div id="message" class="updated fade"><p><strong>Scan once your web and compress images. It may take several minutes</strong></p></div>');
    	update_option('vq_compress_scan_images','no');
    	$exec  = PHP_BINDIR.'/php';
    	$depth = 3;
    	$uploads = wp_upload_dir();
    	$images_path = $uploads['basedir'];
    	$root_dir = $_SERVER['DOCUMENT_ROOT'];
    	$plugins_url = plugin_dir_path(__FILE__)."functions.visquajpegcompress.php";
    	system(''.$exec.' -q '.$plugins_url.' '.$token.' '.$url_api.' '.$keep_orig_file.' '.$depth.' '.$root_dir.' '.$images_path.'> '.$root_dir.'/compress.log &');
    	unlink($root_dir."/compress.log");
    }    
    echo('<div id="message" class="updated fade"><p><strong>Your option are saved.</strong></p></div>');
}
//get options and show settings form
$yesno = get_option('vq_compress_yesno');
$token = get_option('vq_compress_token');
$url_api = get_option('vq_compress_url_api');
$keep_orig_file = get_option('vq_compress_keep_orig_file');
$scan_images = get_option('vq_compress_scan_images');
echo('<div class="wrap">');
echo('<form method="post" accept-charset="utf-8">');
echo('<h2>VisquaJPEGCompress Options</h2>');
echo('<p>This plugin compress uploaded images to a given width after uploading, discarding the original uploaded file in the process.</p>');
echo('<p>Your file will be compressed, there will not be a copy or backup with the original size.</p>');
echo('<p>Set the option \'Compress\' to no if you don\'t want to compress, this way you shouldn\'t deactivate the plugin 
   in case you don\'t want to compress for a while.</p>');
echo('<h3>Settings</h3>
<table class="form-table">
	<tr>
		<td>Compress:&nbsp;</td>
    	<td>
    		<select name="yesno" id="yesno">  
    		<option value="no" label="no"'); if ($yesno == 'no') echo(' selected=selected'); echo('>no</option>
			<option value="yes" label="yes"'); if ($yesno == 'yes') echo(' selected=selected'); echo('>yes</option>
			</select> 
    	</td>
	</tr>
	<tr>
    	<td>Keep Original File:&nbsp;</td>
	    <td>
	    	<select name="keep_orig_file" id="keep_orig_file">  
    			<option value="yes" label="yes"'); if ($keep_orig_file == 'yes') echo(' selected=selected'); echo('>yes</option>
				<option value="no" label="no"'); if ($keep_orig_file == 'no') echo(' selected=selected'); echo('>no</option>
			</select> </br>
			<small>Store in '); echo $_SERVER['DOCUMENT_ROOT']."/original_files" ;echo ('</small>
		</td>
	</tr>
	<tr>
		<th scope="row"><lable for="admin_token">Token:&nbsp;</lable></th>
		<td><input type="text" name="token" size="10" id="token" value="'.$token.'" />
<br/>
    <small>Enter Token</small> <a href="http://portal.visqua.com/index.php?_page=account:add" target="_blank">Get token</a>
		</td>
	</tr>
	<tr>
	    <th scope="row"><lable for="admin_token">URL API:&nbsp;</lable></th>
	    <td>
			<select name="url_api" id="url_api">
				<option value="http://api.us.visqua.com" label="US"'); if ($url_api == 'http://api.us.visqua.com') echo(' selected=selected'); echo('>US</option>
	    		<option value="http://api.vn.visqua.com" label="VN"'); if ($url_api == 'http://api.vn.visqua.com') echo(' selected=selected'); echo('>VN</option>
	    	</select><br/>
	    		<small>Select URL API</small>
		</td>
	</tr>
	<tr>
	    <th scope="row"><lable for="admin_token">Scan directory for images compress:&nbsp;</lable></th>
	    <td>
			<select name="scan_images" id="scan_images">
				<option value="yes" label="Yes"'); if ($scan_images == 'yes') echo(' selected=selected'); echo('>Yes</option>
	    		<option value="no" label="No"'); if ($scan_images == 'no') echo(' selected=selected'); echo('>No</option>
	    	</select><br/>
	    		<small>Run once</small>
		</td>
	</tr>
    </table>');  
 echo('<p class="submit">
  <input type="hidden" name="action" value="update" />
  <input type="submit" name="vq_options_update" value="Update Options &raquo;" />
  </p>
  </form>');
 echo('</div>');
}
/* This function will apply changes to the uploaded file */
function vq_compress_upload($array){ 
  // $array contains file, url, type
	if ($array['type'] == 'image/jpeg' OR $array['type'] == 'image/gif' OR $array['type'] == 'image/png') {
		$uploadfile = $array['file'];
		$token = get_option('vq_compress_token');
		$url_api = get_option('vq_compress_url_api');
		$keep_orig_file = get_option('vq_compress_keep_orig_file');
		if ($keep_orig_file == 'yes'){
			$original_files = $_SERVER['DOCUMENT_ROOT']."original_files/";
			
			$path_full = str_replace($_SERVER['DOCUMENT_ROOT'], $original_files, $uploadfile);
			$path_parts = pathinfo($path_full);
			$new_path = $path_parts['dirname'];
			if (!is_dir($new_path)){
				mkdir($new_path, 0777, true);
			}
			copy($uploadfile,$path_full);

		}
		//check tag Software of metadata
		//ini_set('exif.encode_unicode', 'UTF-8');
		$exif_data = @exif_read_data("$uploadfile");
	    $esoftware =$exif_data['Software'];
	    if ($esoftware != "Compressed by Visqua"){
			$file_visqua = fopen($uploadfile.'visqua', 'w');
			$ch = curl_init($url_api);
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
			curl_setopt($ch, CURLOPT_HEADER, 0);
			curl_setopt($ch, CURLOPT_FILE, $file_visqua);
			curl_setopt($ch, CURLOPT_POSTFIELDS, array ('userfile'=>"@$uploadfile", 'token'=>"$token"));
			$postResult = curl_exec($ch);
			$status=curl_getinfo($ch, CURLINFO_HTTP_CODE);
			curl_close($ch);
			fclose($file_visqua);
			if (($status == 200)){
					unlink($uploadfile);
					rename($uploadfile.'visqua', $uploadfile);
			}
			else{
				unlink($uploadfile.'visqua');
			}
		} // if
	}
  return $array;
} // function
function visqua($html, $id, $alt, $title, $align, $url, $size) {
	$url_img = wp_get_attachment_image_src($id, 'full'); // Grab the current image URL
	$url_img1 = wp_get_attachment_image_src($id, 'thumbnail');
	$url_img2 = wp_get_attachment_image_src($id, 'medium');
	$url_img3 = wp_get_attachment_image_src($id, 'large');
	$url_img_filename = basename($url_img[0]);
	$url_img_filename1 = basename($url_img1[0]);
	$url_img_filename2 = basename($url_img2[0]);
	$url_img_filename3 = basename($url_img3[0]);
	$basedir = wp_upload_dir();
	$dir = $basedir['path'];
	$uploadfile = $dir."/".$url_img_filename;
	$uploadfile1 = $dir."/".$url_img_filename1;
	$uploadfile2 = $dir."/".$url_img_filename2;
	$uploadfile3 = $dir."/".$url_img_filename3;
	$token = get_option('vq_compress_token');
	$url_api = get_option('vq_compress_url_api');
	//check if not original_file
	if (!($uploadfile1 == $uploadfile)){
		$file1 = fopen($uploadfile1.'visqua', 'w');
		$ch1 = curl_init($url_api);
		curl_setopt($ch1, CURLOPT_POST, 1);
		curl_setopt($ch1, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch1, CURLOPT_FILE, $file1);
		curl_setopt($ch1, CURLOPT_HEADER, 0);
		curl_setopt($ch1, CURLOPT_POSTFIELDS, array ('userfile'=>"@$uploadfile1", 'token'=>"$token"));
		$postResult1 = curl_exec($ch1);
		$status=curl_getinfo($ch1, CURLINFO_HTTP_CODE);
		fclose($file1);
		if (($status == 200)){
				unlink($uploadfile1);
				rename($uploadfile1.'visqua', $uploadfile1);
		}
		else{			
			unlink($uploadfile1.'visqua');
		}
	}
	if (!($uploadfile2 == $uploadfile)){
		$file2 = fopen($uploadfile2.'visqua', 'w');
		$ch2 = curl_init($url_api);
		curl_setopt($ch2, CURLOPT_POST, 1);
		curl_setopt($ch2, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch2, CURLOPT_FILE, $file2);
		curl_setopt($ch2, CURLOPT_HEADER, 0);
		curl_setopt($ch2, CURLOPT_POSTFIELDS, array ('userfile'=>"@$uploadfile2", 'token'=>"$token"));
		$postResult2 = curl_exec($ch2);
		$status=curl_getinfo($ch2, CURLINFO_HTTP_CODE);
		fclose($file2);
		if (($status == 200)){
				unlink($uploadfile2);
				rename($uploadfile2.'visqua', $uploadfile2);
		}
		else{
			
			unlink($uploadfile2.'visqua');
		}
	}
	if (!($uploadfile3 == $uploadfile)){
		$file3 = fopen($uploadfile3.'visqua', 'w');
		$ch3 = curl_init($url_api);
		curl_setopt($ch3, CURLOPT_POST, 1);
		curl_setopt($ch3, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch3, CURLOPT_FILE, $file3);
		curl_setopt($ch3, CURLOPT_HEADER, 0);
		curl_setopt($ch3, CURLOPT_POSTFIELDS, array ('userfile'=>"@$uploadfile3", 'token'=>"$token"));
		$postResult3 = curl_exec($ch3);
		fclose($file3);
		$status=curl_getinfo($ch3, CURLINFO_HTTP_CODE);
		if (($status == 200)){
				unlink($uploadfile3);
				rename($uploadfile3.'visqua', $uploadfile3);
		}
		else{	
			unlink($uploadfile3.'visqua');
		}
	}
	return $html;
}
function jpeg_full_quality( $quality ) { 
return 100; 
}
?>
