#include "url.h"

void open_file_and_search_for_urls(char* filename)
{
    int fd;
    char *file = calloc(1024, sizeof(char)), *buffer = calloc(MAXBUFF, sizeof(char)); 
    List* url_list = Create_List();

    char *token = strtok(filename, "\n");  // getting rid of garbage '\n' value from pipe
    sprintf(file, "example/%s", token);    // !!!!! fix the directory name

    if((fd = open(file, O_RDONLY)) == -1)
        printf("Couldn't open file. Error Number %d @ url.c file.\n", errno); 

    while (read(fd, buffer, MAXBUFF) != 0)
    {   
        char *temp_buff = buffer; // coping buffer to avoid strtok_r from changing it

        find_urls(&temp_buff, url_list);
    }
    Print(url_list);
    free(buffer);
    free(file);
    if (close(fd) < 0) perror("Error while closing file @ url.c file."); 
}   


void find_urls(char** temp_buff, List* url_list) {
        char* token, *line;
        // ABOUT REGEX PATTERN
        //   ^  : asserts position at start of a line
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
            printf("LINE: %s\n", line); ///////////

            while ((token = strtok_r(line, " ", &line)))
            {
                printf("TOKEN: %s\n", token); ///////////////

                if (regexec(&regex, token, 0, NULL, 0))            
                    printf("Token %s is not an acceptable url.\n", token);   
                else {
                    Insert_Node(&url_list, token);
                }                                                 
            }
        }
        regfree(&regex);
}