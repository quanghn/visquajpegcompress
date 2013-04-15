#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

using namespace std;
string token_input = "65897756", root_dir="/data/test_dir/", username_input = "test2", url_input="http://api.vn.visqua.com", filename , keep_original="yes";
#define MAXPATHLEN 5000

string splitFilename(const string str) {
  size_t found= str.find_last_of("/\\");
  return str.substr(found+1);
}
void split_path(string path, string &dirname, string &input_short_filename) {
  unsigned int index = path.find_last_of("/");
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

bool curl(string &token_input, string &root_dir, string &username_input, string &url_input, string &filename, string &keep_original){
  char token[FILENAME_MAX];
  char username[FILENAME_MAX];
  strcpy(token,token_input.c_str());
  strcpy(username, username_input.c_str());
  char inputfile[FILENAME_MAX];
        
  strcpy(inputfile,filename.c_str());
  string input_dir, input_short_filename;
  split_path(filename, input_dir, input_short_filename);
          CURL *curl;
          FILE *fp;
          struct curl_slist *headerlist=NULL;
          struct curl_httppost *formpost=NULL;
          struct curl_httppost *lastptr=NULL;
          static const char buf[] = "Expect:";
          curl_global_init(CURL_GLOBAL_ALL);
          curl = curl_easy_init();
          headerlist = curl_slist_append(headerlist, buf);
          int id = (rand()%10000)*10000+rand()%10000;
          cout << "id: " << id << endl;
          stringstream string_id;
          string_id << id;
          long http_status;
          string tmp_input = "/tmp/";
          string filename_tmp;
          filename_tmp = splitFilename(filename);
          string temp_str = tmp_input + filename_tmp;
          char* id_filename = (char*) temp_str.c_str();
          fp = fopen(id_filename, "w");
          curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "userfile",
                     CURLFORM_FILE, inputfile,
                     CURLFORM_END);
          /* Fill in the filename field */ 
          curl_formadd(&formpost,
                   &lastptr,
                   CURLFORM_COPYNAME, "token",
                   CURLFORM_COPYCONTENTS, token,
                   CURLFORM_END);
          /* Fill in the filename field */ 
          curl_formadd(&formpost,
                   &lastptr,
                   CURLFORM_COPYNAME, "username",
                   CURLFORM_COPYCONTENTS, username,
                   CURLFORM_END);
          headerlist = curl_slist_append(headerlist, buf);
          curl = curl_easy_init();
          if(curl) {
          /* what URL that receives this POST */ 
            curl_easy_setopt(curl, CURLOPT_URL, (char*)url_input.c_str());
            /* only disable 100-continue header if explicitly requested */ 
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
            curl_easy_setopt(curl, CURLOPT_FILE, fp);
            
            /* pointer to pass to our read function */ 
            curl_easy_perform(curl);
            /* always cleanup */
            curl_easy_getinfo(curl, CURLINFO_HTTP_CODE,&http_status);
            //cout << "Status" << http_status << endl;
            curl_easy_cleanup(curl);
            curl_slist_free_all (headerlist);
            fclose(fp);
            //cout << "F2: " << inputfile << endl;
              if ( http_status == 200 )
              {
                puts ( "File successfully compressed" );
                ifstream f1 (id_filename, fstream::binary);
                ofstream f2 (inputfile, fstream::trunc|fstream::binary);

                char ch;
                //fstream fo;
                if(!f1) 
                  //fo << "Can't open INPUT file: " << f1 << "id_filename: " << id_filename << endl;
                  // fo.close();
                if(!f2) 
                  //fo << "Can't open OUTPUT file" << endl;
                 // fo.close();
                while(f1 && f1.get(ch) ) 
                f2.put(ch);
                unlink (id_filename);
                
                
                return true;
              }
              else
              {
                perror( "Error compressing file" );
                //fstream fo;
                //fo.open((char*)"debug.log", fstream::in | fstream::out | fstream::app);
                //string datetime = get_current_datetime();
                //fo << datetime << " " << filename << " failed "<< endl; 
                //fo.close();
                return false;
              }
            }
}
int main(int argc, char *argv[]){
  filename = string(argv[1]);
  cout << "Token: " << token_input << endl;
  cout << "username_input: " << username_input << endl;
  cout << "url_input: " << url_input << endl;
  cout << "keep_original: " << keep_original << endl;
  cout << "filename: " << filename << endl;
curl(token_input, root_dir, username_input, url_input,filename, keep_original);
}