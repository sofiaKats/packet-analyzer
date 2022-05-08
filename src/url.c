#include "url.h"

void open_file_and_search_for_urls(int start, char* directory, char* filename)
{
    int fd;
    char *file = calloc(1024, sizeof(char)), *buffer = calloc(MAXBUFF, sizeof(char)); 
    List* url_list = Create_List();  // list to hold url strings and no of appearance in file

    sprintf(file, "%s/%s", directory, filename);   

    if((fd = open(file, O_RDONLY)) == -1)
        printf("Couldn't open file. Error Number %d @ url.c file.\n", errno); 

    while (read(fd, buffer, MAXBUFF) != 0)
    {   
        char *temp_buff = buffer; // coping buffer to avoid strtok_r from changing it
        
        find_urls(&temp_buff, url_list);
        create_file_and_write_valid_urls(start, url_list, filename);
        fprintf(stderr, "all done with filename: %s.out",filename);
       
    }
    free(buffer);
    free(file);
    if (close(fd) < 0) perror("Error while closing file @ url.c file."); 
}   


void find_urls(char** temp_buff, List* url_list) {
        char* token, *line;
        // ABOUT REGEX PATTERN
        //   ^  : asserts position at start of a line
        //   |  : boolean OR operation
        // {1}  : word occurs exactly once
        regex_t    regex;                                                            
        char       *pattern = "^http://www.{1}|^http://{1}";                                       
        int        ret_val;

        // compiling regular expression
        if ((ret_val = regcomp(&regex, pattern, REG_EXTENDED))) {                    
            printf("regcomp() failed, returning nonzero (%d)\n", ret_val);                
            exit(1);                                                                 
        }  

        // extracting each word of file using <space> as delimeter
        while ((line = strtok_r(*temp_buff, "\n", temp_buff))) {
            //printf("LINE: %s\n", line); ///////////

            while ((token = strtok_r(line, " ", &line)))
            {
                //printf("TOKEN: %s\n", token); ///////////////

                if (regexec(&regex, token, 0, NULL, 0)) 
                {
                    //printf("Token %s is not an acceptable url.\n", token);  
                }           
                else {
                    Insert_Node(&url_list, token);
                }                                                 
            }
        }
        regfree(&regex);
}

void create_file_and_write_valid_urls(int start, List* url_list, char* filename)
{
    // if it's the first time we create a .out file, make sure
    // out_files directory is empty and ready for new use
    if(start==1) empty_directory("out_files");
    int fd;
    char fname[512];

    sprintf(fname, "out_files/%s.out", filename);

    // open .out file with suitable permissions
    mode_t fdmode = (S_IRUSR | S_IWUSR);
    if ((fd = open(fname, O_RDWR | O_CREAT, fdmode)) == -1)
        perror("Failed to open .out file");

    write_url_data(url_list, fd); 

     // close opened file
    if (close(fd) < 0) perror("Error while closing .out file.");
    Delete_List(&url_list); // we don't need the list anymore
}


void empty_directory(char* directory) {
    struct dirent *dr;
    char file_path[350];
    DIR* dir;

    if ((dir = opendir (directory)) != NULL) {
        // read directory and delete all existing files in directory

        while ((dr = readdir (dir)) != NULL) {
            if (!strcmp(dr->d_name, ".") || !strcmp(dr->d_name, "..")) continue;
            sprintf(file_path, "out_files/%s", dr->d_name);
            if(remove(file_path)) perror("Error at url.c. Couldn't remove file.\n"); //delete file
        }

    closedir (dir);
    }
}
