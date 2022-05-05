#ifndef URL_H
#define URL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <fcntl.h> 
#include <errno.h> 
#include <unistd.h> 

#include "list.h"

#define MAXBUFF 1024

// opens given file from inotifywait, reads it and finds all existing urls in file
void open_file_and_search_for_urls(char* filename);

#endif