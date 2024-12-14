#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "library.h"
///declaration of TypeElement
typedef User TypeElm;



///declaration of a Queue
typedef struct QueueNode {
    User data;               
    struct QueueNode* next;  
} QueueNode;

// Queue structure
typedef struct {
    QueueNode* head;  
    QueueNode* tail; 
} Queue;



///Queues operations
///procedure InitQueue
void InitQueue(Queue* Q) {
    Q->head = NULL;
    Q->tail = NULL;
}
///procedure Enqueue
void Enqueue(Queue* Q, User x) {
    QueueNode* newNode = malloc(sizeof(QueueNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    newNode->data = x;     
    newNode->next = NULL;

    if (Q->tail) {
        Q->tail->next = newNode;
    } else {
        Q->head = newNode;  
    }
    Q->tail = newNode;     
}


///procedure Dequeue
void Dequeue(Queue* Q, User* x) {
    if (Q->head == NULL) {
        fprintf(stderr, "Queue is empty\n");
        return;
    }

    QueueNode* temp = Q->head;
    *x = temp->data;      
    Q->head = Q->head->next;

    if (Q->head == NULL) {
        Q->tail = NULL;    
    }
    free(temp);
}


///Function isQEmpty
int isQEmpty(Queue Q) {
    return Q.head == NULL;
}

///Function Head
User Head(Queue Q) {
    if (Q.head == NULL) {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE); 
    }
    return Q.head->data;
}