#include <iostream>
#include <ev.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <map>
#include <queue>
#include <list>
#include <ctime>
#include <cassert>
#include <iomanip>
#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <zip.h>
#include <dlfcn.h>
#include "support_functions.h"
#include "visqua_compress.h"
using namespace std;

#define MAX_COMPRESS_PROCESS 4
#define MAXLINELENGTH 1024

using namespace std;

struct file_to_compress {
	string inname, outname, username;
};

const int NUM_OF_EXCLUDE_DIR = 1;
string exclude_dir[NUM_OF_EXCLUDE_DIR] = {"original_files"};
map<int, string>wd_to_dir_name;
queue<file_to_compress> files_queue;
int fd;
int num_of_compress_process = 0, max_of_compress_process = MAX_COMPRESS_PROCESS;

ev_child cw[MAX_COMPRESS_PROCESS];
int pid_of_ev[MAX_COMPRESS_PROCESS];
string root_dir;
string token_input;
string username_input;
string url_input;
string log_file;
string keep_original;
string output_full_filename;
map<string, int> files_map;

bool is_input_dir(string input_name) {
	for (int i=0; i<NUM_OF_EXCLUDE_DIR; i++) {
		int found = input_name.find(exclude_dir[i]);
		unsigned int next_slash_position = found + exclude_dir[i].length();
		if (found > -1) {
			if ((next_slash_position >= input_name.length()) || (input_name[next_slash_position] == '/')) {
				return false;
			}
		}
	}
	return true;
}
string get_output_dir(string input_dir, string root_dir) {
	int index = input_dir.find(root_dir);
	if (index != 0) {
		return "";
	}
	else {
		string temp = input_dir.substr(root_dir.length());
		unsigned int index2 = temp.find("/");
		if (index2 < 0){
			return root_dir + "original_files/";
		}
		else if (index2 == temp.length()-1) {
			//username = temp.substr(0, index2);
			//cerr << "username1 = " << username << endl;
			return  root_dir + "original_files/" + temp.substr(0, index2+1);
			//cerr << "Input is: " << input_dir << endl;
		}
		else {
			//username = temp.substr(0, index2);
			//cerr << "username2 = " << username << endl;
			return root_dir  + "original_files/" +  temp.substr(0, index2+1) + temp.substr(index2+1);
		}
	}
}

void remove_slash(string &str) {
    while (str[str.length()-1] == '/') {
		str = str.erase(str.length()-1, 1);
    }
}

void child_cb (EV_P_ struct ev_child *w, int revents);
void get_file_and_compress() {
	if (files_queue.empty()) {
		return;
	}
	
	if (num_of_compress_process >= max_of_compress_process)
		return;
	
	struct file_to_compress ftc = files_queue.front(), ftc1;
	files_queue.pop();
	string temp = ftc.inname.substr(root_dir.length());
	files_map[temp] = 0;
	cerr << endl << "QUEUE: " << files_queue.size() << " files left" << endl;
	cerr << "size: " << files_queue.size() << endl;
	pid_t pid = fork ();
	if (pid < 0) {
			cerr << "fork() error" << endl;
		} 
		else if (pid==0) {

			//tien trinh con
			//clock_t start_time,elapsed;
			//double elapsed_time; 
			//start_time = clock(); 

			//int insize, outsize, saving, inname;
			string filename = ftc.inname;
			cout << "Checking file:'" << ftc.inname << "'\n";
			bool isSent = visqua_compress(token_input, username_input, root_dir, url_input, filename, log_file, keep_original);
			//}
			if (isSent)
			{
				cout << "Ok" << endl;
			}
			else{
				cerr << "push file: " << filename << " to queue \n" << endl;
				//cout << "Temp: " << temp << endl;
				if (files_map[temp] == 0) {
						files_queue.push(ftc);
						files_map[temp] = 1;
					}
				cerr << endl << "QUEUE: " << files_queue.size() << " files left" << endl;
				cerr << "size: " << files_queue.size() << endl;
				//cout << "Pid: " << pid << endl;
				//num_of_compress_process++;
				}
			_exit(0);
		}
		else {
			for (int i=0; i<max_of_compress_process; i++) {
				if (pid_of_ev[i] == -1) {
					ev_child_init (&cw[i], child_cb, pid, 0);
					ev_child_start(EV_DEFAULT_ &cw[i]);
					pid_of_ev[i] = pid;
					break;
				}
				else if (pid_of_ev[i] == 0) {
					ev_child_set(&cw[i], pid, 0);
					ev_child_start(EV_DEFAULT_ &cw[i]);
					pid_of_ev[i] = pid;
					break;
				}
			}
			num_of_compress_process++;
		}
	
}

void child_cb (EV_P_ struct ev_child *w, int revents) {
	for (int i=0; i<max_of_compress_process; i++) {
		if (pid_of_ev[i] == w->rpid) {
			pid_of_ev[i] = 0;
			break;
		}
	}
	ev_child_stop (EV_A_ w);
	num_of_compress_process--;
	get_file_and_compress();
}

void wait_to_compress(string input_full_filename, bool overwrite) {
	
	if (is_jpeg((char*)input_full_filename.c_str())) {
		string input_dir, input_short_filename;
		split_path(input_full_filename, input_dir, input_short_filename);
		
		if (is_input_dir(input_dir)) {
			//string username = "";
			//cout << "is input dir"<< endl;
			
			string output_dir = get_output_dir(input_dir, root_dir);
			
			//cout << "out dir " << output_dir << endl;
			string output_full_filename = output_dir + input_short_filename;
			//cout << "input_short_filename" << input_short_filename << endl;
			
			bool need_to_insert = false;
			if (!overwrite) {
				//bool exist = is_exist(output_full_filename);
				//if (!exist) {
				need_to_insert = true;
				//}
			}
			else {
				need_to_insert = true;
			}
			
			if (need_to_insert) {
				struct file_to_compress ftc;
				
				ftc.inname = input_full_filename;
				cerr << "push file: " << input_full_filename << " to queue \n" << endl;
				string temp = input_full_filename.substr(root_dir.length());
				//cout << "Temp: " << temp << endl;
				if (files_map[temp] == 0) {
					files_queue.push(ftc);
					files_map[temp] = 1;
				}
			}
		}
	}
}

void readable_cb (struct ev_loop *loop, struct ev_io *w, int revents) {
	char buf[1024];
	int i=0;
    int len = read(w->fd, buf, 1024);
    
	while(i<len){
		struct inotify_event *event = ( struct inotify_event * ) &buf[i];
		int wd = event->wd;
		string path = wd_to_dir_name[wd];
        remove_slash(path);
		if (event->mask & IN_CREATE ) {
			if ( event->mask & IN_ISDIR ) {
				string current_path = path + "/" + event->name + "/";
					if (is_input_dir(current_path)) {
						if (keep_original == "yes"){

						string output_dir = get_output_dir(current_path, root_dir);
						if (output_dir != "" ){
							
							mkpath(output_dir.c_str(), 0777);

							}
						}
					int wd = inotify_add_watch( fd, current_path.c_str(), IN_ALL_EVENTS);
					wd_to_dir_name[wd] = current_path;
					cerr << "The directory " << event->name << " has been created in" << path << endl;
					}
            }
            else {
				cerr << "The file " << event->name << " was created in path " << path << endl;
            }
        }
		else if (event->mask & IN_MOVED_TO) {
			if ( event->mask & IN_ISDIR ) {}
            else {}
		}
		else if (event->mask & IN_MOVED_FROM) {
			if ( event->mask & IN_ISDIR ) {
				//cerr << "The directory " << event->name << " was move out of path " << path << endl;       
            }
            else {
				//cerr << "The file " << event->name << " was move out of path " << path << endl;
            }
		}
		else if (event->mask & IN_DELETE) {
			if ( event->mask & IN_ISDIR ) {
				//cerr << "The directory " << event->name << " was deleted in path " << path << endl;       
            }
            else {
				//cerr << "The file " << event->name << " was deleted in path " << path << endl;
            }
		}
		else if (event->mask & IN_CLOSE_WRITE) {
			if ( event->mask & IN_ISDIR ) {
				//cerr << "The directory " << event->name << " was modified to path " << path << endl;       
            }
            else {
				cerr << "The file " << event->name << " was created in " << path << endl;
				//cerr << "abc" << endl;
	
				wait_to_compress(path + "/" + string(event->name), true);
				get_file_and_compress();
			}
		}
		
        i+=sizeof(struct inotify_event)+event->len;
	}
}

void listdir(char* dirname, list<string> &dirs_list) {
	DIR* d_fh;
	struct dirent* entry;
	char longest_name[MAXPATHLEN];

	while( (d_fh = opendir(dirname)) == NULL) {
		cerr << "Couldn't open directory: " << dirname << endl;
		return;
	}
	
	while((entry=readdir(d_fh)) != NULL) {
		/* Don't descend up the tree or include the current directory */
		if(strncmp(entry->d_name, "..", 2) != 0 && strncmp(entry->d_name, ".", 1) != 0) {
			strncpy(longest_name, dirname, MAXPATHLEN);
           if (dirname[strlen(dirname)-1] != '/') 
				strncat(longest_name, "/", 256);
				strncat(longest_name, entry->d_name, 256);
			
			//printf("longges_name = %s\n", longest_name);
			if (entry->d_type == DT_DIR) {
				strncat(longest_name, "/", MAXPATHLEN);
				bool status = is_input_dir(string(longest_name));
				if (status) {
					dirs_list.push_back(string(longest_name));
					listdir(longest_name, dirs_list);
				}
			}
			else {
			//	cerr << "longges_name = " << longest_name << endl;
				wait_to_compress(longest_name, false);
			}
		}
	}

	closedir(d_fh);
}

void signal_callback_handler(int signum) {
	cerr << "Caught signal " << signum << endl;
	for (int i=1; i<1000; i++)
	
		inotify_rm_watch(fd, i);
	
	close(fd);
	exit(1);
}

void init(list<string>dirs_list) {
	//Tao cac thu muc o thu muc dich giong voi thu muc nguon
	for (list<string>::iterator it=dirs_list.begin(); it!=dirs_list.end(); ++it) {
		string input_dir = *it;
		int wd = inotify_add_watch(fd, input_dir.c_str(), IN_ALL_EVENTS);
		wd_to_dir_name[wd] = input_dir;
	}
}

void timeout_cb (struct ev_loop *loop, struct ev_timer *w, int revents) {
	
	while (!files_queue.empty() && (num_of_compress_process < max_of_compress_process)) {
		get_file_and_compress();

	}
	
}
void read_config_file(string configfile, string &token_input, string &url_input, string &username_input, string &root_dir, string &log_file, string &keep_original)
{
	std::map < std::string, std::string > myMap;
    char input[MAXLINELENGTH];
    FILE *fin;
    int len, ptr;
    bool found;
    char config_file[MAXLINELENGTH];
    strcpy(config_file,configfile.c_str());

    if ((fin = fopen(config_file, "r")) == NULL){
        fprintf(stderr, "Unable to open config file!\n");
    }

    while (fgets(input, MAXLINELENGTH, fin)){
        if (input[0] == '#')
            continue;
        len = strlen(input);
        if (len < 2)
            continue;
        if (input[len-1] == '\n')
            input[len-1] = '\0';
        ptr = 0;
        found = false;
        while (ptr < len){
            if (input[ptr] == '='){
                found = true;
                input[ptr++] = '\0';
                break;
            }
            ptr++;
        }
        if (found){
            myMap[input] = &input[ptr];
        }
        token_input =  myMap["token"];
        url_input = myMap["url"];
        username_input = myMap["username"];
        root_dir = myMap["root_dir"];
        log_file = myMap["log_file"];
        keep_original = myMap["keep_original"];
    }
    fclose(fin);
}
int main(int argc, char *argv[]) {
	
	
	if (argc < 1) {
		cerr << "Usage: \n" << endl;
		return 0;
	}
	read_config_file("/etc/visquacompress/visqua.conf", token_input, url_input, username_input, root_dir, log_file, keep_original);
	cout << "token: " << token_input << endl;
	cout << "root_dir: " << root_dir << endl;
	cout << "log_file: " << log_file << endl;
	cout << "url_input: " << url_input << endl;
	cout << "keep_original: " << keep_original << endl;
	
	if (root_dir[root_dir.length()-1] != '/') {
		root_dir = root_dir + '/';
	}
	if (max_of_compress_process > MAX_COMPRESS_PROCESS) {
		cerr << "Number of compress process have to less than " << MAX_COMPRESS_PROCESS << endl;
		return 0;
	}
	for (int i=0; i<max_of_compress_process; i++) {
		pid_of_ev[i] = -1;
	}
        
	signal(SIGINT, signal_callback_handler);
	
	fd = inotify_init();//(O_NONBLOCK);
	list<string> dirs_list;
	dirs_list.push_front((char*)root_dir.c_str());
	listdir((char*)root_dir.c_str(), dirs_list);
	init(dirs_list);
	struct ev_loop *loop = ev_default_loop(0);
	struct ev_io readable_ev;
	ev_io_init(&readable_ev, readable_cb, fd, EV_READ);
	ev_io_start(loop, &readable_ev);
	struct ev_timer stdin_watcher; 
	ev_timer_init(&stdin_watcher, timeout_cb, 0.5, 0.); /* note, only repeat used */
	ev_timer_start(loop, &stdin_watcher); /* start timer */
	ev_loop (loop, 0);
  
 }
