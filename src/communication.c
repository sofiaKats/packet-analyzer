#include "communication.h"

void create_fifos(int no_fifo, char** fifo1, char** fifo2) {
    // Create a unique name (e.g manager_f_read_1 , manager_f_write_6 )
	sprintf(*fifo1, "%s%d", "manager_f_read_", no_fifo);
    if ( (mkfifo(*fifo1, PERMS) < 0) && (errno != EEXIST) ) {
       perror("can't create fifo");
   }

   sprintf(*fifo2, "%s%d", "manager_f_write_", no_fifo);
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

void receive_filename_from_manager(char* fifo1, char* fifo2) {
    int readfd, writefd;
    /* Open the FIFOs.  We assume server has already created them.  */
    writefd = open_fifo(fifo1, WRITE);
    readfd = open_fifo(fifo2, READ);

    char buff[MAXBUFF];
    int n;

    /* Read the filename from the IPC descriptor. */
    if ((n= read(readfd, buff, MAXBUFF)) <= 0) {
        perror("worker: filename read error ");
    }

    buff[n] = '\0';  /* null terminate filename */
        printf("message received from worker:%s\n",buff);
         
    close(readfd);
    close(writefd);
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