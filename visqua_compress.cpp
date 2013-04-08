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
#include "support_functions.h"


string get_output_dir_visqua(string input_dir, string root_dir) {
  int index = input_dir.find(root_dir);
  //username = "";
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

bool visqua_compress(string &token_input, string &username_input, string &root_dir, string &url_input, string &filename, string &log_file, string &keep_original) {
  char token[FILENAME_MAX];
  char username[FILENAME_MAX];
  strcpy(token,token_input.c_str());
  strcpy(username, username_input.c_str());
  Exiv2::Image::AutoPtr image;
  image = Exiv2::ImageFactory::open(filename);
  assert(image.get() != 0);
  image->readMetadata();
  Exiv2::IptcData &iptcData = image->iptcData();
//Exiv2::ExifData &exifData = image->exifData();
  Exiv2::IptcData::const_iterator end_iptcData = iptcData.end();
//Exiv2::ExifData::const_iterator end_exifData = exifData.end();
  char inputfile[FILENAME_MAX];
  char outputfile[FILENAME_MAX];
  bool status_of_compress=false;      
        
  strcpy(inputfile,filename.c_str());
  string input_dir, input_short_filename;
  split_path(filename, input_dir, input_short_filename);
  string output_dir = get_output_dir_visqua(input_dir, root_dir);
  

  for (Exiv2::IptcData::const_iterator md = iptcData.begin(); md != end_iptcData; ++md) {
    string compressed_by_visqua = md->value().toString().c_str();
  
      if (compressed_by_visqua == "Compressed by Visqua")
    {            
        status_of_compress = true;
        break;
    }
  }
      if (status_of_compress)
      {
        cout << filename << " was compressed by Visqua" << "'\n";
      }
      else{
          if (keep_original == "yes"){
          
          if (output_dir != ""){
            //cout << "does not exists" << endl;
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
          cout << "copy " << inputfile << " to " << outputfile << endl;
          }
          cout << "has not compressed by visqua before" << "'\n";
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

          //cout << "ID: " << temp_str << endl; 
          char* id_filename = (char*) temp_str.c_str();
          //char id_filename[FILENAME_MAX];
      //    cout << "ID_Filename: " << id_filename << endl;
          /*strcpy(id_filename,filename.c_str());
          strcpy(id_filename,"abc");*/
          fp = fopen(id_filename, "w");
          //cout << "ID filename: " << id_filename << endl;
          /* Fill in the upload filename field */ 
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
                puts ( "File successfully compressed" );
                ifstream f1 (id_filename, fstream::binary);
                ofstream f2 (inputfile, fstream::trunc|fstream::binary);

                char ch;
                fstream fo;
                fo.open((char*)log_file.c_str(), fstream::in | fstream::out | fstream::app);
                if(!f1) 
                  fo << "Can't open INPUT file: " << f1 << "id_filename: " << id_filename << endl;
                   //fo.close();
                if(!f2) 
                  fo << "Can't open OUTPUT file" << endl;
                  //fo.close();
                while(f1 && f1.get(ch) ) 
                f2.put(ch);
                unlink (id_filename);
                
                string datetime = get_current_datetime();
                fo << datetime << " " << filename << " successfully "<< endl; 
                fo.close();
                return true;
              }
              else
              {
                perror( "Error compressing file" );
                fstream fo;
                fo.open((char*)log_file.c_str(), fstream::in | fstream::out | fstream::app);
                string datetime = get_current_datetime();
                fo << datetime << " " << filename << " failed "<< endl; 
                fo.close();
                return false;
              }
              //return true;
          }
      }
return true;
}

 
