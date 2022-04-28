#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <fcntl.h> 
#include <errno.h> 
#include<unistd.h> 

#define MAXBUFF 1024

extern int errno;

int main(void)
{
    int fd;
    char *buffer = calloc(MAXBUFF, sizeof(char)); 

    if((fd = open("test.txt", O_RDONLY)) == -1)
        printf("Error Number % d\n", errno); 

    //int counter = 0;
    while (read(fd, buffer, MAXBUFF) != 0)
    {   // extracting each word of file using <space> as delimeter
        char* token, *line, *temp_buff  = calloc(MAXBUFF, sizeof(char));
        strcpy(temp_buff, buffer);

        // ABOUT REGEX PATTERN
        // ^  : asserts position at start of a line
        // |  : logical OR performance
        // {1}  : word occurs exactly once

        regex_t    regex;                                                            
        char       *pattern = "^http://www.{1}|^http://{1}";                                       
        int        ret_val;

        if ((ret_val = regcomp(&regex, pattern, REG_EXTENDED)) != 0) {                    
            printf("regcomp() failed, returning nonzero (%d)\n", ret_val);                
            exit(1);                                                                 
        }  

        while ((line = strtok_r(temp_buff, "\n", &temp_buff))) {
            printf("LINE: %s\n", line); ///////////
            if(line == NULL) printf("line: I become NULL at while loop.\n"); //////////

            while ((token = strtok_r(line, " ", &line)))
            {
                printf("TOKEN: %s\n", token); ///////////////
                if(token == NULL) printf("token: I become NULL at while loop.\n"); /////////////

                if ((ret_val = regexec(&regex, token, 0, NULL, 0)) != 0)            
                    printf("failed to ERE match '%s',returning %d.\n", token, ret_val);                                                    
            }
        }

        regfree(&regex);
    }

    free(buffer);
     // close opened file
    if (close(fd) < 0) 
        perror("Error while closing file."); 
    return 0;
}