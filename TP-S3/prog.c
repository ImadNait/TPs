#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.c"
#include "queue.c"
#include "library.h"

Stack Inventory; 
Stack RecentReturned;  
Queue RequestQ; 

void SaveBooksToFile(const char* filename, Stack* library) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    Stack temp;
    InitStack(&temp);
    Book book;

    while (!isSEmpty(*library)) {
        Pop(library, &book);
        fprintf(file, "%d\n%s\n%s\n%d\n\n", book.id, book.title, book.author, (bool)book.available);  // No change
        Push(&temp, book);
    }

    while (!isSEmpty(temp)) {
        Pop(&temp, &book);
        Push(library, book);
    }

    fclose(file);
}

void LoadBooksFromFile(const char* filename, Stack* library) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file for reading or file not found.\n");
        return;
    }

    Book book;
    int temp; 
    while (fscanf(file, "%d\n", &book.id) == 1) {
        fgets(book.title, sizeof(book.title), file);
        book.title[strcspn(book.title, "\n")] = '\0';
        fgets(book.author, sizeof(book.author), file);
        book.author[strcspn(book.author, "\n")] = '\0';  
        fscanf(file, "%d\n", &temp);  
        book.available = (bool)temp; 
        Push(library, book);
    }

    fclose(file);
}

void UpdateBookAvailabilityInFile(const char* filename, int bookID, bool newAvailability) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    // Temporary storage for books
    Stack temp;
    InitStack(&temp);

    Book book;
    int tempAvailability;

    // Read books from the file and update the target book
    while (fscanf(file, "%d\n", &book.id) == 1) {
        fgets(book.title, sizeof(book.title), file);
        book.title[strcspn(book.title, "\n")] = '\0';  
        fgets(book.author, sizeof(book.author), file);
        book.author[strcspn(book.author, "\n")] = '\0';  // Remove trailing newline
        fscanf(file, "%d\n", &tempAvailability);
        book.available = (bool)tempAvailability;

        // Update the book's availability if the ID matches
        if (book.id == bookID) {
            book.available = newAvailability;
        }

        Push(&temp, book);
    }
    fclose(file);

    // Write back the updated stack to the file
    file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    while (!isSEmpty(temp)) {
        Pop(&temp, &book);
        fprintf(file, "%d\n%s\n%s\n%d\n", book.id, book.title, book.author, book.available);
    }

    fclose(file);
}



void InitLibrary() {
    InitStack(&Inventory); 
    InitStack(&RecentReturned);  
    InitQueue(&RequestQ); 
}

void AddBook(Stack*Library, Book book){
    Push(Library, book);
    SaveBooksToFile("inventory.txt", Library);
}

void BorrowBook(User user ,int ID){
    Book rebook;
    Stack R;
    bool check = false;
    InitStack(&R);
    while(!isSEmpty(Inventory)&&!check){
    Pop(&Inventory, &rebook);
    if(rebook.id == ID && rebook.available==true){
        check=true;
        rebook.available = false;
        UpdateBookAvailabilityInFile("inverntory.txt", rebook.id, false);
    } 
    else{
    Push(&R, rebook);
    }
    }
    while(!isSEmpty(R)){
    Pop(&R, &rebook); 
    Push(&Inventory, rebook);
    }
    if(check){
        Enqueue(&RequestQ, user);
    }


}

//Rerturn book function
void ReturnBook(Book book){
    Book temp;
    Stack T;
    while(!isSEmpty(Inventory)){
    Pop(&Inventory, &temp);
    if(temp.id == book.id){
    temp.available = true;
    UpdateBookAvailabilityInFile("inventory.txt", temp.id, true);
    }
    Push(&T, temp);
    }
    while (!isSEmpty(T)){
    Pop(&T, &temp);
    Push(&Inventory, temp);
    }
    
    Push(&RecentReturned, book);

}

void ProcessRequest(User user){
    Book bk;
    Stack G;
    InitStack(&G);
    while(!isSEmpty(Inventory)){
    Pop(&Inventory, &bk);
    if(user.requested_book_id == bk.id){
        if(bk.available == true){
            bk.available = false;
        }else{
            Enqueue(&RequestQ, user);
        }
    }
    Push(&G, bk);
    }
    while(!isSEmpty(G)){
        Pop(&G, &bk);
        Push(&Inventory, bk);
    }
    
}

void DisplayS(Stack* S){
    Stack R;
    InitStack(&R);
    Book popBook;
    int i=1;
    printf("Current Books:\n");
    while(!isSEmpty(*S)){
        Pop(S, &popBook);
        printf("Book %d\n", i);
        printf("ID: %d\nTitle: %s\nAuthor: %s\nAvailablity: %s\n\n", popBook.id, popBook.title, popBook.author, popBook.available ? "true" : "false");
        i++;     
        Push(&R, popBook);
    }
    while(!isSEmpty(R)){
        Pop(&R, &popBook);
        Push(S, popBook);
    }
}

void DisplayStack(){
    DisplayS(&RecentReturned);
}

void DisplayQueue(Queue* Q){
    Queue F;
    InitQueue(&F);
    User popUser;
    int i=1;
    printf("Current Queue:\n");
    while(!isQEmpty(*Q)){
        Dequeue(Q, &popUser);
        printf("User %d\n", i);
        printf("ID: %d\name: %s\nRequestedBookId: %d\n\n", popUser.id, popUser.name, popUser.requested_book_id);
        i++;
        Enqueue(&F, popUser);
    }
    while(!isQEmpty(F)){
        Dequeue(&F, &popUser);
        Enqueue(Q, popUser);
    }
}


int main(){
    User user1 = { 5, "Imad", 1};
    Book book1 = {1, "1984", "George Orwell", true},book2 = {2, "HH", "LEO MESSI", true}, book3 = {4, "GG", "Nigga", false};
    Book book5 = { 8, "Red", "Author", true };
    InitLibrary();
    Push(&Inventory, book1);  
    Push(&Inventory, book2);
    Push(&Inventory, book3);
    AddBook(&Inventory, book5);
    ProcessRequest(user1);
    DisplayS(&Inventory);
    DisplayQueue(&RequestQ);
    return 0;
}