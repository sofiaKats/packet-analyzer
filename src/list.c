#include "list.h"

// using my own implementation of a link list homework, from the subject Data Structures

// function to create linked list
List* Create_List(void)
{
    List* list;
    // Creation of list
	if((list = malloc(sizeof(List))) == NULL){
		printf("failed to create list");
		return NULL;
	}
    list->head = NULL;
    return list;
}

// function to add new node in list
void Insert_Node(List** list, char* url)
{
    Url* newnode, *current;
    get_url_location(&url);
    // url already exists on list, just increment the appearance counter and return
    if((newnode = Search_Url(*list,url))  ) {
        newnode->appearance++;
        return;
    }

    newnode = malloc(sizeof(Url));
    newnode->url = malloc(sizeof(char) * 1024);
    newnode->next = NULL;

    newnode->appearance = 1;
    strcpy(newnode->url, url);

    // list is empty
    if((*list)->head == NULL)
        (*list)->head = newnode;
    // list already exists
    else {
        current = (*list)->head;
        while (current->next  ) current = current->next;
        current->next = newnode;
    }
}

// function to print list
void Print(List* list)
{
    if(list->head == NULL) {
        printf("Nothing to print here, list is empty.\n");
        return;
    }
        
    Url* current;
    current = list->head;

    while(current) {
        printf("url: %s appearance:%d  ",current->url, current->appearance);
        if (current->next)
            printf("->");
        current = current->next;
    }
    printf("\n");
}

// function to search if url already exists in list returns 1 if url exists, 0 otherwise
Url* Search_Url(List* list, const char* url)
{
    Url* current;
    current = list->head;

    while(current) {
        if(!strcmp(current->url, url))
            return current;
        current = current->next;
    }
    return NULL;
}

// function to delete entire list
void Delete_List(List** list)
{
    Url* current, *temp;
    current = (*list)->head;

    while(current) {
        temp = current;
        current = current->next;
        temp->next = NULL;
        free(temp->url);    // freeing allcated c string
        free(temp);
    }

    (*list)->head = NULL;
    free(*list);
    (*list) = NULL;
}

void write_url_data(List* list, int fd)
{
    char* url_data = calloc(1024, sizeof(char));
    Url* current = list->head;
    while (current) {
        strcpy(url_data, " ");
        sprintf(url_data, "%s %d \n", current->url, current->appearance);
        //printf("URL DATA: %s\n", url_data);
        write(fd ,(char *)url_data, strlen(url_data));
        current = current->next;
    }
    free(url_data);
}

void get_url_location(char** url)
{
    removeSubstr(*url, "/");
}

// given a and a substring it finds the url location with www.
void removeSubstr (char *string, char *sub) {
    int counter=0; 
	char* token = strtok(string, sub);
    char location[50];
    while(token != NULL) {
        strcpy(location, token);
        token = strtok(NULL, sub);
        // after second '/' break the loop, we found the location of url
        if((++counter)==2) break;
    }
    strcpy(string, location);
}
