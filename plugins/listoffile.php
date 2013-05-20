<?
include "includes/getListFile.php";
$dirlist = getFileList("test2", true, 2);
echo "<table border='1'>\n";
  echo "<tr><th>Name</th><th>Type</th><th>Size</th><th>Last Mod.</th></tr>\n";
  foreach($dirlist as $file) {
    #if(!preg_match("/\.jpg$/\.JPG$/", $file['name'])) continue;
	if($file['type'] != 'image/jpeg') continue;
    echo "<tr>\n";
    echo "<td>{$file['name']}</td>\n";
    echo "<td>{$file['type']}</td>\n";
    echo "<td>{$file['size']}</td>\n";
    echo "<td>",date('r', $file['lastmod']),"</td>\n";
    echo "</tr>\n";
  }
  echo "</table>\n\n";
?>
