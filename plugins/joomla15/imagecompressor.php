<?php
defined('_JEXEC') or die('Restricted Access');
jimport('joomla.plugin.plugin');

$mainframe->registerEvent('onPrepareContent', 'imagecompressor');

function imagecompressor(&$row, &$params, $page)
{
	$pattern = '/<img.src="([^"]*)".([^>]*)>/';
	preg_match_all($pattern, $row->text, $matches);

	// iterate through them and replace with cached version
	for($i = 0; $i< count($matches[0]); $i++)	
	{
		$replacements[] = getCache($matches[1][$i], $matches[2][$i]) . "<br>";
	}

	for($i = 0; $i < count($replacements); $i++)
	{
		$row->text = str_replace($matches[0][$i],$replacements[$i] ,$row->text);		
	}
}

function getCache($imagePath, $restofTag)
{
	//first, check to see if the rest of the tag has the width parameter
	$wPattern = '/width="([^"]*)"/';
	preg_match($wPattern, $restofTag, $match);
	$fileName = getFileName($imagePath);
	$imageDir = getDir($imagePath);
	$imageCacheDir = "cache/" . getDir($imagePath);
	$fileNameCache = $fileName;
	$cache = $imageCacheDir . "/" . $fileNameCache;

	// create the cache if it doesn't exist
	if(!is_dir($imageCacheDir))
		mkdir($imageCacheDir,0777,true);
	
	// create the file if it doesn't exist
	if(!file_exists($cache)) {
		$uploadfile = $imageDir . "/" . $fileName;
		$file_visqua = fopen($uploadfile.'.tmp.visqua', 'w');
		$plugin = & JPluginHelper::getPlugin('content', 'imagecompressor');
		$pluginParams = new JParameter($plugin->params);
		$token = $pluginParams->get('token', 1);
		$url_api = $pluginParams->get('url_api', 1);
		$keep_orig_file = $pluginParams->get('keep_orig_file', 1);
		if ($keep_orig_file == 'yes'){		
			copy($uploadfile,$uploadfile.'.visqua');
		}
		else {
			if(file_exists($uploadfile.'.visqua')) {
			unlink($uploadfile.'.visqua');
			}	
		}
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
		if (($status == 200))
			{
				unlink($uploadfile);
				rename($uploadfile.'.tmp.visqua', $uploadfile);
				copy($uploadfile, $cache);
			}
		else
			{
			unlink($uploadfile.'.tmp.visqua');
			copy($uploadfile, $cache);
			}
	}
	
	if(!file_exists($cache))
		return null;
	return "<img src=\"$cache\" $restofTag >";
}


function getDir($fileName){
	$pos = strrpos($fileName, "/");
	if(!$pos)
		return ".";
	return substr($fileName,0, $pos);
}

function getFileName($data){
	$pos = strrpos($data, "/");
	if(!$pos)
		return ".";
		
	return substr($data,$pos+1, strlen($data) - $pos - 1);	
}

function getExtension($fileName){
	$pos = strrpos($fileName, ".");
	if(!$pos)
		return null;
	return substr($fileName,$pos +1, strlen($fileName) - $pos -1);
}

?>
