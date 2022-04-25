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


// function to create named pipes between manager and worker
void create_fifos();

// function to open named pipe between manager and worker
int open_fifo(char *fifo, int action);

// function to send filename through named pipe
void send_filename_to_worker(char* filename);

// function to receive filename through named pipe
char* receive_filename_from_manager(void);

// when done with communication, we delete the fifos
void unlink_fifos(void);
#endif