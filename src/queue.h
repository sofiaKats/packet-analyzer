#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct Worker {
    pid_t m_pid;
    char* filename;
    int busy;           // busy=1 if worker is busy, busy=0 if worker is NOT busy
    int readfd;
    int writefd;
    char* rd_fifo_name;
    char* wr_fifo_name;
    struct Worker* next;
} Worker;

typedef struct Queue{
    int size; // may delete this
    Worker* front;
    Worker* rear;
}Queue;

// Queue initialization and creation
Queue* Create_Queue(void);

// Insert item in queue
void Queue_Push(Queue** queue, pid_t m_pid, char* filename, int readfd, int writefd, char* rd_fifo_name, char* wr_fifo_name);

// Delete front item from queue
void Queue_Pop(Queue** queue);

// Print elements in queue
void Print_Queue(Queue* queue);

// returns 1 if queue is empty, 0 if queue is NOT empty
int queue_is_empty(Queue* queue);

#endif