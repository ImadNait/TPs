#ifndef LIBRARY_H
#define LIBRARY_H

typedef struct {
    int id;
    char title[100];
    char author[100];
    bool available;
} Book;

typedef struct {
    int id;
    char name[100];
    int requested_book_id;
} User;

typedef User TypeElem;

#endif