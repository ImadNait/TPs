#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "library.h"
///declaration of TypeElement
typedef User TypeElm;
//typedef char TypeElm;


///declaration of a Queue
typedef struct QueueNode {
    User data;                // Store the object directly
    struct QueueNode* next;   // Pointer to the next node
} QueueNode;

// Queue structure
typedef struct {
    QueueNode* head;  // Front of the queue
    QueueNode* tail;  // Rear of the queue
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

    newNode->data = x;      // Copy the object into the node
    newNode->next = NULL;

    if (Q->tail) {
        Q->tail->next = newNode;
    } else {
        Q->head = newNode;  // If the queue was empty, head is also updated
    }
    Q->tail = newNode;      // Update the tail
}


///procedure Dequeue
void Dequeue(Queue* Q, User* x) {
    if (Q->head == NULL) {
        fprintf(stderr, "Queue is empty\n");
        return;
    }

    QueueNode* temp = Q->head;
    *x = temp->data;       // Copy the object to the provided variable
    Q->head = Q->head->next;

    if (Q->head == NULL) {
        Q->tail = NULL;    // If the queue becomes empty, reset the tail
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
        exit(EXIT_FAILURE); // Handle empty queue error
    }
    return Q.head->data;
}