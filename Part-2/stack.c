#include<stdio.h>
#include <stdlib.h>
#include "library.h"

///declaration of a Stack
typedef struct Estack* Stack;
typedef struct Estack {
    Book Data;
    Stack Next;
} SNode;
///Stacks operations
///procedure InitStack
void InitStack(Stack *P)
{*P=NULL;}

///procedure Push
void Push(Stack* S, Book x) {
    Stack V = malloc(sizeof(SNode));
    if (!V) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    V->Data = x;  
    V->Next = *S;
    *S = V;
}


///procedure Pop
void Pop(Stack* S, Book* x) {
    if (*S == NULL) {
        printf("Stack is empty!\n");
        return;
    }
    Stack V = *S;
    *x = V->Data;  
    *S = V->Next;
    free(V);
}


///Function isSEmpty
int isSEmpty(Stack S) {
    return S == NULL;
}


///Function Top
Book Top(Stack S) {
    if (S == NULL) {
        printf("Stack is empty!\n");
        exit(1);
    }
    return S->Data; 
}
