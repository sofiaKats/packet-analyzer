#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <sys/errno.h>
#include  <sys/stat.h>

#define  READ 0
#define  WRITE 1
#define MAXBUFF 1024
#define FIFO1   "/tmp/fifo.1"
#define FIFO2   "/tmp/fifo.2"

#include "communication.h"


int main(int argc, char *argv[])
{
   pid_t  pid, pid2;
   int fd[2];

   // Creation of unamed pipe
   if (pipe(fd) == -1){ perror("failed to create pipe.\n"); exit(1); }
   // Creation of listener process
   if ( (pid = fork()) == -1 ){ perror("failed to fork.\n"); exit(1); }

   // Manager Process
   if (pid) {
      char buffer[MAXBUFF];
      close(fd[WRITE]);
      
      while (read(fd[READ], buffer, sizeof(buffer)) != 0)
      {
         // extraction of filename from create message sent by inotifywait
         char* temp = malloc(sizeof(char) * MAXBUFF); 
         char *token = strtok(buffer, " ");
         while( token != NULL ) {
            strcpy(temp,token);     // last loop will store filename
            token = strtok(NULL, " ");
         }
         
         printf( "TEMP: %s\n", temp );

         int readfd, writefd;
         create_fifos();
         if ( (pid2 = fork()) == -1 ){ perror("failed to fork.\n"); exit(1); }
         
         // Manager Process
         if(pid2)
         {
            /* Created the FIFOs, now open them -- one for
            * reading and one for writing.
            */
            readfd = open_fifo(FIFO1, READ);
            writefd = open_fifo(FIFO2, WRITE);

            manager(readfd, writefd, temp);

            close(readfd);
            close(writefd);
            free(temp); //!!!!!!!!!!!!!!!!!!!
         }
         // Worker Process
         else
         {
            /* Open the FIFOs.  We assume server has already created them.  */
            writefd = open_fifo(FIFO1, WRITE);
            readfd = open_fifo(FIFO2, READ);

            worker(readfd, writefd);
         
            close(readfd);
            close(writefd);

            /* Delete the FIFOs, now that we're done.  */

            if ( unlink(FIFO1) < 0) {
               perror("client: can't unlink \n");
            }
            if ( unlink(FIFO2) < 0) {
               perror("client: can't unlink \n");
            }
         }
      }
      close(fd[READ]);
   }


   // Listener Process
   else
   {
      close(fd[READ]);
      dup2(fd[WRITE],1); // stdout(1) of listener process will write data to pipe
      close(fd[WRITE]);

      execlp("inotifywait", "inotifywait", "-m", "example", "-e", "create", (char*)NULL);
      perror("execlp failed.\n");
      exit(1);
   }

   return 0;
}