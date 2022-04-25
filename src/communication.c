#include "communication.h"

void create_fifos()
{
    if ( (mkfifo(FIFO1, PERMS) < 0) && (errno != EEXIST) ) {
       perror("can't create fifo");
   }

   if ((mkfifo(FIFO2, PERMS) < 0) && (errno != EEXIST)) {
       unlink(FIFO1);
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

void send_filename_to_worker(char* filename)
{
    int readfd, writefd;
    /* Created the FIFOs, now open them -- one for
    * reading and one for writing.
    */
    readfd = open_fifo(FIFO1, READ);
    writefd = open_fifo(FIFO2, WRITE);

    char buff[MAXBUFF];
    int n;

    // write it to the IPC descriptor.
    strcpy(buff, filename);
    n = strlen(buff);

    if (write(writefd, buff, n) != n)
        perror("worker: filename write error");

    close(readfd);
    close(writefd);
}

char* receive_filename_from_manager(void)
{
    int readfd, writefd;
    /* Open the FIFOs.  We assume server has already created them.  */
    writefd = open_fifo(FIFO1, WRITE);
    readfd = open_fifo(FIFO2, READ);

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

void unlink_fifos(void)
{
    /* Delete the FIFOs, now that we're done.  */
    if ( unlink(FIFO1) < 0) {
        perror("client: can't unlink \n");
    }
    if ( unlink(FIFO2) < 0) {
        perror("client: can't unlink \n");
    }
}