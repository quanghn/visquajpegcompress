#ifndef _SUPPORT_FUNCTIONS_
#define _SUPPORT_FUNCTIONS_

#include <iostream>
#include <fstream>
#include <sstream>

#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <zip.h>

#define MAXPATHLEN 5000

using namespace std;

string int_to_string(int x);
bool checkpath(const char* path);
bool mkpath(const char* path, mode_t mode);
string size_to_str(int size);
string to_lower(string str);

bool is_exist(string filename);
bool end_with(string fileName, string ext);

string get_current_datetime();
string get_current_moy();
int get_current_month();
int get_file_size(char *name);
bool is_sub_str_of(string sub_str, string str);
void normalize_url(string &url);
bool is_url_jpeg(string url);
bool add_to_zip(const char *archive, const char *file, const char *name);
bool zip_list_file(std::string zip_file_name, std::string list_file[], std::string list_name[], int num_file);
bool is_jpeg(char file_in_name[]);

#endif
