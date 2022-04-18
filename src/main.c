#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define  READ 0
#define  WRITE 1

int main(int argc, char *argv[])
{
   pid_t  pid;
   int fd[2];

   // Creation of unamed pipe
   if (pipe(fd) == -1){ perror("failed to create pipe.\n"); exit(1); }
   // Creation of listener process
   if ( (pid = fork()) == -1 ){ perror("failed to fork.\n"); exit(1); }

   // Manager Process
   if (pid) {
      close(fd[WRITE]);
      dup2(fd[READ],0); // read pipe will become stdin(0)
      close(fd[READ]);
   }
   // Listener Process
   else
   {
      close(fd[READ]);
      dup2(fd[WRITE],1); // stdout(1) of exec'd inotifywait will write data to pipe
      close(fd[WRITE]);
      execlp("inotifywait", "inotifywait", "-m", "example", (char*)NULL);
      perror("execlp failed.\n");
   }

   return 0;
}

