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
#define PERMS   0666

void create_fifos();
int open_fifo(char* fifo, int action);
void manager(int readfd, int writefd, char* filename);
void worker(int readfd, int writefd);

int main(int argc, char *argv[])
{
   pid_t  pid, pid2;
   int fd[2];
   int readfd, writefd;

   // Creation of unamed pipe
   if (pipe(fd) == -1){ perror("failed to create pipe.\n"); exit(1); }
   // Creation of listener process
   if ( (pid = fork()) == -1 ){ perror("failed to fork.\n"); exit(1); }

   // Manager Process
   if (pid) {
      char buffer[1024];
      close(fd[WRITE]);
      
      while (read(fd[READ], buffer, sizeof(buffer)) != 0)
      {
         // extraction of filename from create message sent by inotifywait
         char* temp, *token = strtok(buffer, " ");
         while( token != NULL ) {
            strcpy(temp,token);     // last loop will store filename
            token = strtok(NULL, " ");
         }
         
         printf( "TEMP: %s\n", temp );

         if ( (pid2 = fork()) == -1 ){ perror("failed to fork.\n"); exit(1); }



         // Manager Process
         if(pid2)
         {
            /* Create the FIFOs, then open them -- one for
            * reading and one for writing.
            */
            create_fifos();
            readfd = open_fifo(FIFO1, READ);
            writefd = open_fifo(FIFO2, WRITE);

            manager(readfd, writefd, temp);

            close(readfd);
            close(writefd);
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

