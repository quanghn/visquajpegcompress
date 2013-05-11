#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <queue>
#include <list>
#include <ctime>
#include <cassert>
#include <iomanip>
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include "support_functions.h"

bool visqua_compress(string &token_input, string &username_input, string &root_dir, string &url_input, string &filename, string &log_file, string &keep_original, string &debug) {
  char token[FILENAME_MAX];
  char username[FILENAME_MAX];
  strcpy(token,token_input.c_str());
  strcpy(username, username_input.c_str());
  char inputfile[FILENAME_MAX];
  char outputfile[FILENAME_MAX];
  strcpy(inputfile,filename.c_str());
  string input_dir, input_short_filename;
  split_path(filename, input_dir, input_short_filename);
  string output_dir = get_output_dir_visqua(input_dir, root_dir);
          if (keep_original == "yes"){
          if (output_dir != ""){
            mkpath(output_dir.c_str(), 0777);
            string output_full_filename = output_dir + input_short_filename;
            strcpy(outputfile,output_full_filename.c_str());
          }
          ifstream orig_file (inputfile, fstream::binary);
          ofstream backup_file (outputfile, fstream::trunc|fstream::binary);
          char ch_backup;
          if(!orig_file) 
            cout << "Can't not onpen INPUT file" << endl;             
          if(!backup_file) 
            cout << "Can't not onpen OUTPUT file" << endl;
          while(orig_file && orig_file.get(ch_backup)) 

          backup_file.put(ch_backup);
          if (debug =="1"){
          cout << "copy " << inputfile << " to " << outputfile << endl;
          }
          }
          //cout << "has not compressed by visqua before" << "'\n";
          CURL *curl;
          FILE *fp;
          //CURLcode res;
          struct curl_slist *headerlist=NULL;
          struct curl_httppost *formpost=NULL;
          struct curl_httppost *lastptr=NULL;
          static const char buf[] = "Expect:";
          curl_global_init(CURL_GLOBAL_ALL);
          curl = curl_easy_init();
          headerlist = curl_slist_append(headerlist, buf);
          int id = (rand()%10000)*10000+rand()%10000;
          //cout << "id: " << id << endl;
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
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
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
                if (debug == "1"){
                puts ( "File successfully compressed" );
                }
                ifstream f1 (id_filename, fstream::binary);
                ofstream f2 (inputfile, fstream::trunc|fstream::binary);

                char ch;
                //fstream fo;
                //fo.open((char*)log_file.c_str(), fstream::in | fstream::out | fstream::app);
                if(!f1) 
                  cerr << "Can't open INPUT file: " << f1 << "id_filename: " << id_filename << endl;
                   //fo.close();
                if(!f2) 
                  cerr << "Can't open OUTPUT file" << endl;
                  //fo.close();
                while(f1 && f1.get(ch) ) 
                f2.put(ch);
                unlink (id_filename);
                
                string datetime = get_current_datetime();
                //fo << datetime << " " << filename << " successfully "<< endl; 
                //fo.close();
                return true;
              }
              else
              {
                perror( "Error compressing file" );
                //fstream fo;
                //fo.open((char*)log_file.c_str(), fstream::in | fstream::out | fstream::app);
                //string datetime = get_current_datetime();
                //fo << datetime << " " << filename << " failed "<< endl; 
                //fo.close();
                return false;
              }
              //return true;
         // }
      }
return true;
}