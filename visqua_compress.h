#ifndef _VISQUA_COMPRESS_H
#define _VISQUA_COMPRESS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

bool visqua_compress(string &token_input, string &username_input, string &root_dir, string &url_input, string &filename, string &log_file, string &keep_original);

#endif