#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <sys/errno.h>
#include <sys/stat.h>
#include <signal.h>

#define  READ 0
#define  WRITE 1
#define MAXBUFF 1024

extern int errno;

#include "communication.h"
#include "queue.h"
#include "url.h"

int check_args(int argc, char** argv, char* directory_to_watch);

int main(int argc, char *argv[])
{
   char directory_to_watch[32];

   if(!check_args(argc,argv,directory_to_watch)) exit(1);
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
      char* filename = malloc(sizeof(char) * 1024);
      close(fd[WRITE]);

      // queue to with worker info
      Queue * queue = Create_Queue();
      
      while (read(fd[READ], buffer, sizeof(buffer)) != 0)
      {
         //extraction of filename from create message sent by inotifywait
         extract_filename(buffer, &filename);
         char *token = strtok(filename, "\n");  // getting rid of garbage '\n' value from pipe
         fprintf(stderr, "\n\nfilename is: %s\n", filename);
         
         ++counter; 
         
         // malloc for fifo names
         char* fifo1 = malloc(sizeof(char) * 1024) , *fifo2 = malloc(sizeof(char) * 1024);
         create_fifos(counter, &fifo1, &fifo2); /*create named pipe for worker-manager communication*/
         if ( (pid2 = fork()) == -1 ){ perror("failed to fork.\n"); exit(1); } /*create worker*/ 
         
         // Manager Process
         if(pid2) {
            send_filename_to_worker(filename, fifo1, fifo2);
         }
         // Worker Process
         else {
            int readfd, writefd;
            char* file = malloc(sizeof(char) * MAXBUFF);
            receive_filename_from_manager(fifo1, fifo2, &file, &readfd, &writefd);
            Queue_Push(&queue, getpid(), file, readfd, writefd, fifo2, fifo1);

            fprintf(stderr, "filename received from worker:%s\n",file);
            sleep(1);
            open_file_and_search_for_urls(counter, directory_to_watch,file);

            free(fifo1); free(fifo2);
            free(file);
            raise(SIGSTOP);  // child sends SIGSTOP to itself
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

      execlp("inotifywait", "inotifywait", "-m", directory_to_watch, "-e", "create", "-e" , "moved_to", (char*)NULL);
      perror("execlp failed.\n");
      exit(1);
   }

   return 0;
}

int check_args(int argc, char** argv, char* directory_to_watch)
{
   // if no extra arguments are passed, watch current directory
   if(argc==1) {
      strcpy(directory_to_watch, ".");
      return 1;
   }
   // else watch given directory from command line
   else if (argc==3){
      if(strcmp(argv[1], "-p")) {
         perror("Second argument is not -p. Please run the program again.");
         return 0;
      }
      else {
         strcpy(directory_to_watch, argv[2]);
         return 1;
      }
   }
   else 
      perror("Wrong amount of command line parameters.\n");
   return 0;
}
