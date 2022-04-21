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
    queue->front = malloc(sizeof(Q_node));
    queue->rear = malloc(sizeof(Q_node));

    // Initialization of empty data members
    queue->size = 0;
    queue->front = NULL;
    queue->rear = NULL;
	
	return queue;
}


void Queue_Push(Queue** queue, int item)
{
    // Creation of newnode
	Q_node * NewNode;
	NewNode = malloc(sizeof(Q_node));
	NewNode->element = item;
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

    Q_node* temp;
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

    Q_node *current;
	current = queue->rear;

	while(current != NULL){
		printf("%d",current->element);
		if(current->next != NULL) {
			printf(",");
		}
		current = current->next;
		
	}
	printf("\n");
}