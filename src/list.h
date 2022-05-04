#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h> 


typedef struct node {
    char* url;
    int appearance;   // counting the amount of times a url was found on a text file
    struct node* next;
} Url;

typedef struct List{
    Url* head;
}List;

// function to create linked list
List* Create_List(void);

// function to add new node in list
void Insert_Node(List** list, const char* url);

// function to print list
void Print(List* list);

// function to search if url already exists in list returns url node if url exists, NULL otherwise
Url* Search_Url(List* list, const char* url);

// function to delete entire list
void Delete_List(List** list);

void write_url_data(List* list, int fd);

#endif