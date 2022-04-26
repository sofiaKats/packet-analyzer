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

#include "communication.h"
#include "queue.h"


int main(int argc, char *argv[])
{
   pid_t  pid, pid2;
   int fd[2], counter = 0 ; // counter used for creating unique fifo names

   // Creation of unamed pipe
   if (pipe(fd) == -1){ perror("failed to create pipe.\n"); exit(1); }
   // Creation of listener process
   if ( (pid = fork()) == -1 ){ perror("failed to fork.\n"); exit(1); }

   // Manager Process
   if (pid) {
      char buffer[MAXBUFF];
      memset(buffer, 0, MAXBUFF);  // initializing the buffer content
      close(fd[WRITE]);
      
      while (read(fd[READ], buffer, sizeof(buffer)) != 0)
      {
         //extraction of filename from create message sent by inotifywait
         char  temp[MAXBUFF] , *token = strtok(buffer, " "); 
         memset(temp, 0, MAXBUFF);
         while( token != NULL ) {
            strcpy(temp,token);     // last loop will store filename
            token = strtok(NULL, " ");
         }
         printf( "TEMP: %s\n", temp );
         
         // queue to with worker info
         Queue * queue = Create_Queue();

         ++counter;
         //check if a brand new worker is needed
         // malloc for fifo names
         char* fifo1 = malloc(sizeof(char) * 1024) , *fifo2 = malloc(sizeof(char) * 1024);
         create_fifos(counter, &fifo1, &fifo2); /*create named pipe for worker-manager communication*/
         if ( (pid2 = fork()) == -1 ){ perror("failed to fork.\n"); exit(1); } /*create worker*/ 
         
         // Manager Process
         if(pid2) {
            send_filename_to_worker(temp, fifo1, fifo2);
         }
         // Worker Process
         else {
            receive_filename_from_manager(fifo1, fifo2);
            unlink_fifo(fifo1); unlink_fifo(fifo2);
            free(fifo1); free(fifo2);
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