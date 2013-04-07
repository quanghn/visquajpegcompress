#include "support_functions.h"

string int_to_string(int x) {
	string t = "", value = "";
	if (x<0) {
		x *= -1;
		t = "-";
	}
	if (x == 0) return "0";
	while (x>0) {
		value = char(x%10+'0') + value;
		x /= 10;
	}
	return (t+value);
}

int get_file_size(char *name) {
	struct stat filestatus;
	stat(name, &filestatus );
	return filestatus.st_size;
}
string splitFilename(const string str) {
  size_t found= str.find_last_of("/\\");
  return str.substr(found+1);
}
string size_to_str(int size) {
	string t = "B";
	float fsize = (float)size;
	stringstream ss (stringstream::in | stringstream::out);
	if (fsize > 1024*1024) {
		t = "MB";
		fsize /= (1024*1024);
	}
	else if (fsize >1024) {
		t = "KB";
		fsize /= 1024;
	}
	ss << fsize;
	string result = ss.str() + t;
	return result;
}

bool checkpath(const char* path) {
    struct stat st;
	if(stat(path,&st)){
		return true;
	} else {
		return false;
	}
}

bool mkpath(const char* path, mode_t mode) {
    char p[MAXPATHLEN];
    int c = 0, ret;
    char *lastslash = 0;
    struct stat s;

    if (!path || !*path) return true;
    while (*path) {
        if (c >= MAXPATHLEN) {
            return false;
        }
        p[c++] = *path++;
    }
    p[c] = 0;

    lastslash = strchr(p+1, '/');
    while (1) {
        if (lastslash) *lastslash = 0;
        ret = stat(p, &s);
        if (ret < 0) {
            ret = mkdir(p, mode);
            if (ret < 0) return false;
        } else {
            if (!S_ISDIR(s.st_mode)) {
                return false;
            }
        }
        if (!lastslash) return true;
        *lastslash = '/';
        lastslash = strchr(lastslash+1, '/');
    }
    return true;
}

string to_lower(string str) {
    for (int i=0; i<str.length(); i++) 
        if (str[i] >= 0x41 && str[i] <= 0x5A) 
            str[i] = str[i] + 0x20;
    return str;
}

bool is_exist(string filename) {
	struct stat buf;
	int result = stat(filename.c_str(), &buf);
	return (result==0);
}

bool end_with(string fileName, string ext) {
    fileName = to_lower(fileName);
    ext = to_lower(ext);
	if (ext.length() >= fileName.length()) return false;
	for (int i=1; i<=ext.length(); i++)
		if (fileName[fileName.length()-i] != ext[ext.length()-i]) {
			return false;
		}
	return true;
}

string get_current_datetime() {
    time_t now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return string(buf);
}

string get_current_moy(){
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m", &tstruct);
	return string(buf);
}
 
int get_current_month() {
    time_t now = time(0);
    struct tm  tstruct;
    char       buf[4];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%m", &tstruct);
    return atoi(buf);
}

string filename_time(){
	time_t now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);
    return string(buf);
}

bool is_sub_str_of(string sub_str, string str){
  if(sub_str.length() < 2){
    return false;
  }
	istringstream str_stream(str);
	while(!str_stream.eof()){
	  string temp;
	  getline(str_stream, temp, '-');
	  if(sub_str.compare(temp) == 0){
	    return true;
	  }
	}
	return false;
}

void normalize_url(string &url){
  replace(url.begin(), url.end(), '\n', ' ');
  
}
/*
bool is_url_jpeg(string url){
	string header_info_filename = filename_time() + ".txt";
	string ftype = "Content-Type: ";
	string jpeg = "image/jpeg";
	string command = "curl -r 0-0,1 " + url + " -I > " + header_info_filename;
	system("curl -r 0-0,1 http://vnexpress.net/Files/Subject/3b/bd/c1/35/chay4.jpg -I > info.txt");
	ifstream fi(header_info_filename.c_str());
	while(!fi.eof()){
		string temp;
		getline(fi, temp);
		if(temp.compare(0, ftype.length(), ftype) == 0){
			if(temp.compare(ftype.length(), jpeg.length(), jpeg) == 0){
				remove(header_info_filename.c_str());
				return true;
			} else {
				remove(header_info_filename.c_str());
				return false;
			} 
		}
	}
}
*/
bool add_to_zip(const char *archive, const char *file, const char *name)
{
  struct zip *za;
  struct zip_source *zs;
  int err;

  if ((za=zip_open(archive, ZIP_CREATE, &err)) == NULL) {
    return false;
  }

  if ((zs=zip_source_file(za, file, 0, -1)) == NULL) {
    return false;
  }

  if (zip_add(za, name, zs) == -1) {
    zip_source_free(zs);
    return false;
  }

  if (zip_close(za) == -1) {
    return false;
  }

  return true;
}

bool zip_list_file(std::string zip_file_name, std::string list_file[], std::string list_name[], int num_file) {
  bool add_zip_status;
  for(int i = 0; i < num_file; i++){
    add_zip_status = add_to_zip(zip_file_name.c_str(), list_file[i].c_str(), list_name[i].c_str());
    if(!add_zip_status) return false;
  }
  return true;
}

bool is_jpeg(char file_in_name[])
{
  unsigned char buffer[2];
  ifstream file_in;
  file_in.open ( file_in_name );
  file_in.read ( ( char * ) buffer, 2 );
  if((int)buffer[0] == 255 && (int)buffer[1] == 216){
    return true;
  }
  return false;
}
bool is_png(char file_in_name[])
{
  unsigned char buffer[4];
  ifstream file_in;
  file_in.open ( file_in_name );
  file_in.read ( ( char * ) buffer, 4 );
  if((int)buffer[0] == 137 && (int)buffer[1] == 80 && (int)buffer[2] == 78 && (int)buffer[3] == 71) {
    return true;
  }
  return false;
}
void split_path(string path, string &dirname, string &input_short_filename) {
  int index = path.find_last_of("/");
  if (index >1) {
    dirname = path.substr(0, index+1);
    if (index < path.length()-1)
      input_short_filename = path.substr(index+1);
    else input_short_filename = "";
  }
  else {
    dirname = "";
    input_short_filename = path;
  }
}
bool insert_compressed_image_infomation_to_file(string log_file, string filename) {
  ofstream fo(log_file.c_str());
  //int month = get_current_month() - 1;
  string datetime = get_current_datetime();
  if (fo) {
    fo << datetime << ": " << filename << endl;
    fo << endl;
    fo.close();
    return true;
  }
  else return false;
}

