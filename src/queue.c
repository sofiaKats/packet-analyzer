#include "queue.h"

Queue* Create_Queue(void)
{
    // Creation of queue
	Queue * queue = malloc(sizeof(Queue));	
	
	if(queue == NULL){
		printf("failed to create queue");
		return NULL;
	}

    // allocation of memory for first and last node of deque
    queue->front = malloc(sizeof(Worker));
    queue->rear = malloc(sizeof(Worker));

    // Initialization of empty data members
    queue->size = 0;
    queue->front = NULL;
    queue->rear = NULL;
	
	return queue;
}


void Queue_Push(Queue** queue, pid_t m_pid, char* filename, int readfd, int writefd, char* rd_fifo_name, char* wr_fifo_name)
{
    // Creation of newnode
	Worker * NewNode;
	NewNode = malloc(sizeof(Worker));
	//NewNode->element = item;
    NewNode->m_pid = m_pid;
    NewNode->filename = malloc(sizeof(char) * 1024);
    strcpy(NewNode->filename, filename);
    NewNode->busy = 1;
    NewNode->readfd = readfd;
    NewNode->writefd = writefd;
    NewNode->rd_fifo_name = malloc(sizeof(char) * 1024);
    NewNode->wr_fifo_name = malloc(sizeof(char) * 1024);
    strcpy(NewNode->rd_fifo_name, rd_fifo_name);
    strcpy(NewNode->wr_fifo_name, wr_fifo_name);
	NewNode->next = NULL;
	
	(*queue)->size++;
	
    // if queue is empty
	if((*queue)->front == NULL){
		(*queue)->front = (*queue)->rear = NewNode;
		
	}else{ // queue has elements
        (*queue)->front->next = NewNode;
		(*queue)->front = NewNode;	
	}
}


void Queue_Pop(Queue** queue)
{// Queue follows the order of First In First Out (FIFO)
    if((*queue)->rear == NULL) {
        printf("Queue already empty, nothing to delete here.\n");
        return;
    }

    Worker* temp;
    temp = (*queue)->rear;

    (*queue)->rear = (*queue)->rear->next;
    (*queue)->size--;
    free(temp);
}

int queue_is_empty(Queue* queue)
{
    return (queue->rear == NULL);
}


void Print_Queue(Queue* queue)
{
    if(queue->rear == NULL) {
        printf("Queue is empty, nothing to print here.\n");
        return;
    }

    Worker *current;
	current = queue->rear;

	while(current != NULL){
		printf("%d",current->m_pid);
		if(current->next != NULL) {
			printf(",");
		}
		current = current->next;
		
	}
	printf("\n");
}