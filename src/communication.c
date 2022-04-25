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


void manager(int readfd, int writefd, char* filename)
{
   char buff[MAXBUFF];
   int n;

   // write it to the IPC descriptor.
   strcpy(buff, filename);
   n = strlen(buff);

   if (write(writefd, buff, n) != n)
      perror("worker: filename write error");

   /* Read data from the IPC descriptor and write to
    * standard output. 
    */

   while ( (n = read(readfd, buff, MAXBUFF)) > 0)
      if (write(1, buff, n) != n)   /* fd 1 = stdout */ {
         perror("worker: data write error \n");
      }
      else
          printf("\n above received by worker(im in manager)\n");
    
   if (n <0) { 
      perror("worker: data read error \n");
   }
}

void worker(int readfd, int writefd)
{
   char buff[MAXBUFF];
   char errmesg[256];
   int n, fd;

   /* Read the filename from the IPC descriptor. */
   if ((n= read(readfd, buff, MAXBUFF)) <= 0) {
       perror("worker: filename read error ");
   }

   buff[n] = '\0';  /* null terminate filename */
    printf("message received from worker:%s\n",buff);
    // write filename back to manager pipe
    if (write(writefd, buff, n) != n) {
        perror("worker: data write error");
    }
}

