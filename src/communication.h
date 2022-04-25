#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include  <sys/types.h>
#include  <sys/stat.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>           
#include <unistd.h>

#define  READ 0
#define  WRITE 1

#define MAXBUFF 1024
#define FIFO1   "/tmp/fifo.1"
#define FIFO2   "/tmp/fifo.2"
#define PERMS   0666


void create_fifos();
int open_fifo(char *fifo, int action);
void manager(int readfd, int writefd, char *filename);
void worker(int readfd, int writefd);

#endif