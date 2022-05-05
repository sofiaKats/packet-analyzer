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

// given a buffer, funct checks for the existance of urls using regex library
void find_urls(char** temp_buff, List* url_list);

// creates a .out file, parses through url list and writes the url string as well as no of appearances
void create_file_and_write_valid_urls(List* url_list, char* filename);

#endif