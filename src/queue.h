#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>


typedef struct Q_node {
    int element;
    struct Q_node* next;
} Q_node;

typedef struct Queue{
    int size; // may delete this
    Q_node* front;
    Q_node* rear;
}Queue;

// Queue initialization and creation
Queue* Create_Queue(void);

// Insert item in queue
void Queue_Push(Queue** queue, int item);

// Delete front item from queue
void Queue_Pop(Queue** queue);

// Print elements in queue
void Print_Queue(Queue* queue);

// returns 1 if queue is empty, 0 if queue is NOT empty
int queue_is_empty(Queue* queue);

#endif