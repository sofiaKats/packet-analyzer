#ifndef URL_H
#define URL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <fcntl.h> 
#include <errno.h> 
#include <unistd.h> 
#include  <sys/stat.h>
#include <dirent.h>

#include "list.h"

#define MAXBUFF 1024

// code implemented with functions from: cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf

// opens given file from inotifywait, reads it and finds all existing urls in file
void open_file_and_search_for_urls(int start, char* directory, char* filename);

// given a buffer, funct checks for the existance of urls using regex library
void find_urls(char** temp_buff, List* url_list);

// creates a .out file, parses through url list and writes the url string as well as no of appearances
void create_file_and_write_valid_urls(int start, List* url_list, char* filename);

// empties out_files directory of previous content if there is any
void empty_directory(char* directory);
#endif
