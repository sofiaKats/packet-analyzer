#include "communication.h"

void create_fifos(int no_fifo, char** fifo1, char** fifo2) {
    // if it's the first time a fifo is created, make a directory to store fifos to
    if(no_fifo==1) {
        char dir_path[] = "named_fifos";
        DIR* dir = opendir(dir_path);
        if (dir)                            // If dir already exists 
            delete_dir(dir_path);           // completely remove it
        
        mkdir(dir_path, 0777);
    }
    // Create a unique name (e.g manager_f_read_1 , manager_f_write_6 )
	sprintf(*fifo1, "%s%d", "named_fifos/manager_read_", no_fifo);
    if ( (mkfifo(*fifo1, PERMS) < 0) && (errno != EEXIST) ) {
       perror("can't create fifo");
   }

   sprintf(*fifo2, "%s%d", "named_fifos/manager_write_", no_fifo);
   if ((mkfifo(*fifo2, PERMS) < 0) && (errno != EEXIST)) {
       unlink_fifo(*fifo1);
       perror("can't create fifo");
   }
}

int open_fifo(char* fifo, int action) {
    int value;
    if ( (value = open(fifo, action))  < 0)  {
      perror("can't open fifo");
   }
   return value;
}

void send_filename_to_worker(char* filename, char* fifo1, char* fifo2) {
    int readfd, writefd;
    /* Created the FIFOs, now open them -- one for
    * reading and one for writing.
    */
    readfd = open_fifo(fifo1, READ);
    writefd = open_fifo(fifo2, WRITE);

    char buff[MAXBUFF];
    int n;

    // write it to the IPC descriptor.
    strcpy(buff, filename);
    n = strlen(buff);

    if (write(writefd, buff, n) != n)
        perror("manager: filename write error");

    close(readfd);
    close(writefd);
}

void receive_filename_from_manager(char* fifo1, char* fifo2, char** filename, int* readfd, int* writefd) {
    /* Open the FIFOs.  We assume server has already created them.  */
    *writefd = open_fifo(fifo1, WRITE);
    *readfd = open_fifo(fifo2, READ);

    char buff[MAXBUFF];
    int n;

    /* Read the filename from the IPC descriptor. */
    if ((n= read(*readfd, buff, MAXBUFF)) <= 0) {
        perror("worker: filename read error ");
    }

    buff[n] = '\0';  /* null terminate filename */
    strcpy(*filename, buff);
         
    close(*readfd);
    close(*writefd);
}

void unlink_fifo(char* fifo) {
    /* Delete the FIFO, now that we're done.  */
    if ( unlink(fifo) < 0) {
        perror("can't unlink fifo\n");
    }
}

void extract_filename(char* buffer, char** temp) {
    char *token = strtok(buffer, " "); 
    while( token != NULL ) {
        strcpy(*temp,token);     // last loop will store filename
        token = strtok(NULL, " ");
    }
}

void delete_dir(char* directory) {
    struct dirent *dr;
    char file_path[350];
    DIR* dir;
    if ((dir = opendir (directory)) != NULL) {
        // read directory and delete all existing files in direcotry
        while ((dr = readdir (dir)) != NULL) {
            //printf ("%s\n", dr->d_name);
            if (!strcmp(dr->d_name, ".") || !strcmp(dr->d_name, "..")) continue;
            sprintf(file_path, "named_fifos/%s", dr->d_name);
            unlink_fifo(file_path); //delete named pipe pipe
        }
    closedir (dir);
    }

    // then delete directory itself
    if(rmdir(directory) == -1)
        perror("Failed to remove directory.\n");
}
