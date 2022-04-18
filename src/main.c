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

  if (pipe(fd) == -1){ perror("pipe"); exit(1); }
  if ( (pid = fork()) == -1 ){ perror("fork"); exit(1); }

   // Manager Process
   if (pid) {
      close(fd[WRITE]);
      dup2(fd[READ],0); // 0 : standard input (stdin)
      close(fd[READ]);
   }
   // Listener Process
   else
   {
      close(fd[READ]);
      dup2(fd[WRITE],1); // 1 : standard output (stdout)
      close(fd[WRITE]);
      execlp("inotifywait", "inotifywait", "-m", "example", (char*)NULL);
      perror("execlp");
   }

   return 0;
}

