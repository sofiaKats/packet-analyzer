#include "url.h"

void open_file_and_search_for_urls(char* filename)
{
    int fd;
    char *file = calloc(1024, sizeof(char)), *buffer = calloc(MAXBUFF, sizeof(char)); 
    List* url_list = Create_List();

    char *token = strtok(filename, "\n");  // getting rid of garbage '\n' value from pipe
    sprintf(file, "example/%s", token);
    //printf("FILE:_%s_\n",file );

    // if( access(file, F_OK ) == 0 ) {
    //     printf("(2)file exists\n");
    // } else {
    //     printf("(2)file doesn't exists\n");
    // }

    if((fd = open(file, O_RDONLY)) == -1)
        printf("Couldn't open file. Error Number % d\n", errno); 
    while (read(fd, buffer, MAXBUFF) != 0)
    {   // extracting each word of file using <space> as delimeter
        char* token, *line, *temp_buff;
        temp_buff = buffer;    // coping buffer to avoid strtok_r from changing it

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

        while ((line = strtok_r(temp_buff, "\n", &temp_buff))) {
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
    free(buffer);
    free(file);
    if (close(fd) < 0) perror("Error while closing file."); 
}   