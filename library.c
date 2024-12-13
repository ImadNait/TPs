#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.c"
#include "queue.c"
#include "library.h"
#define INVENTORY_FILE "inventory.txt"
#define REQUEST_QUEUE_FILE "request_queue.txt"
#define RECENT_RETURNED_FILE "recent_returned.txt"

Stack Inventory; 
Stack RecentReturned;  
Queue RequestQ; 

//function to save new books to the inventory file
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

//function to load books from the inventory file
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


    Stack temp;
    InitStack(&temp);

    Book book;
    int tempAvailability;


    while (fscanf(file, "%d\n", &book.id) == 1) {
        fgets(book.title, sizeof(book.title), file);
        book.title[strcspn(book.title, "\n")] = '\0';  
        fgets(book.author, sizeof(book.author), file);
        book.author[strcspn(book.author, "\n")] = '\0';  
        fscanf(file, "%d\n", &tempAvailability);
        book.available = (bool)tempAvailability;


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

// Function to save the request queue to a file
void SaveRequestQueueToFile(const char* filename, Queue* requestQueue) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening request queue file for writing.\n");
        return;
    }

    Queue tempQueue;
    InitQueue(&tempQueue);
    User user;

    while (!isQEmpty(*requestQueue)) {
        Dequeue(requestQueue, &user);
        fprintf(file, "%d\n%s\n%d\n\n", user.id, user.name, user.requested_book_id);
        Enqueue(&tempQueue, user);
    }

    // Restore the original queue
    while (!isQEmpty(tempQueue)) {
        Dequeue(&tempQueue, &user);
        Enqueue(requestQueue, user);
    }

    fclose(file);
}

// Function to load the request queue from a file
void LoadRequestQueueFromFile(const char* filename, Queue* requestQueue) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening request queue file for reading.\n");
        return;
    }

    User user;
    while (fscanf(file, "%d\n", &user.id) == 1) {
        fgets(user.name, sizeof(user.name), file);
        user.name[strcspn(user.name, "\n")] = '\0';
        fscanf(file, "%d\n", &user.requested_book_id);
        Enqueue(requestQueue, user);
    }

    fclose(file);
}

// Function to save recently returned books to a file
void SaveRecentReturnedToFile(const char* filename, Stack* recentReturned) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening recent returned books file for writing.\n");
        return;
    }

    Stack tempStack;
    InitStack(&tempStack);
    Book book;

    while (!isSEmpty(*recentReturned)) {
        Pop(recentReturned, &book);
        fprintf(file, "%d\n%s\n%s\n%d\n\n", book.id, book.title, book.author, (bool)book.available);
        Push(&tempStack, book);
    }

    // Restore the original stack
    while (!isSEmpty(tempStack)) {
        Pop(&tempStack, &book);
        Push(recentReturned, book);
    }

    fclose(file);
}

// Function to load recently returned books from a file
void LoadRecentReturnedFromFile(const char* filename, Stack* recentReturned) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening recent returned books file for reading.\n");
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
        Push(recentReturned, book);
    }

    fclose(file);
}


void InitLibrary() {
    InitStack(&Inventory); 
    InitStack(&RecentReturned);  
    InitQueue(&RequestQ); 
    
    LoadBooksFromFile("inventory.txt", &Inventory);
    LoadRecentReturnedFromFile("recent_returned.txt", &RecentReturned);
    LoadRequestQueueFromFile("request_queue.txt", &RequestQ);
}

//Process Request function
void ProcessRequests() {
    if (isQEmpty(RequestQ)) {
        printf("No pending requests in the queue.\n");
        return;
    }

    Queue tempQueue;
    InitQueue(&tempQueue);
    User currentUser;
    Book book;
    bool processedAnyRequest = false;

    while (!isQEmpty(RequestQ)) {
        Dequeue(&RequestQ, &currentUser);
        bool bookFound = false;
        bool bookProcessed = false;
        Stack tempInventory;
        InitStack(&tempInventory);

        while (!isSEmpty(Inventory)) {
            Pop(&Inventory, &book);

 
            if (book.id == currentUser.requested_book_id) {
                bookFound = true;

                if (book.available) {
                    book.available = false;
                    UpdateBookAvailabilityInFile(INVENTORY_FILE, book.id, false);
                    
                    printf("Book %d borrowed by user %s (ID: %d)\n", 
                           book.id, currentUser.name, currentUser.id);
                    processedAnyRequest = true;
                    bookProcessed = true;
                } 
            }

            Push(&tempInventory, book);
        }

        while (!isSEmpty(tempInventory)) {
            Pop(&tempInventory, &book);
            Push(&Inventory, book);
        }

        if (!bookProcessed) {
            if (!bookFound) {
                printf("Book %d not found in inventory for user %s\n", 
                       currentUser.requested_book_id, currentUser.name);
            } else {
                Enqueue(&tempQueue, currentUser);
                printf("Book %d not available. Request for %s remains in queue.\n", 
                       currentUser.requested_book_id, currentUser.name);
            }
        }
    }

    while (!isQEmpty(tempQueue)) {
        Dequeue(&tempQueue, &currentUser);
        Enqueue(&RequestQ, currentUser);
    }

    if (processedAnyRequest) {
        SaveRequestQueueToFile(REQUEST_QUEUE_FILE, &RequestQ);
        SaveBooksToFile(INVENTORY_FILE, &Inventory);
    }
}

void AddBook(Stack*Library, Book book){
    Push(Library, book);
    SaveBooksToFile("inventory.txt", Library);
    ProcessRequests();
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
        printf("Book has been borrowed successfully!\n");
        UpdateBookAvailabilityInFile("inventory.txt", rebook.id, false);
    } 
    Push(&R, rebook);
    }
    while(!isSEmpty(R)){
    Pop(&R, &rebook); 
    Push(&Inventory, rebook);
    }
    if(!check){
        printf("Book not found!\n u have been added to the request queue.");
        Enqueue(&RequestQ, user);
        SaveRequestQueueToFile("request_queue.txt", &RequestQ);
    }
}

//Rerturn book function
void ReturnBook(Book book){
    Book temp;
    Stack T;
    InitStack(&T);
    bool found = false;

    while(!isSEmpty(Inventory)){
        Pop(&Inventory, &temp);
        if(temp.id == book.id){
            temp.available = true;
            found = true;
            UpdateBookAvailabilityInFile("inventory.txt", temp.id, true);
            Push(&RecentReturned, temp);  // Push the updated temp, not book
        }
        Push(&T, temp);
    }

    // Restore the inventory stack
    while (!isSEmpty(T)){
        Pop(&T, &temp);
        Push(&Inventory, temp);
    }

    // Save the inventory after it has been fully restored
    SaveBooksToFile("inventory.txt", &Inventory);
    SaveBooksToFile("recent_returned.txt", &RecentReturned);

    if (found) {
        printf("\nBook returned!");
    } else {
        printf("\nBook not found in inventory!");
    }

    ProcessRequests();
}




//Display stack function
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
//function that displays "the recent returned" stack
void DisplayStack(Stack* S){
    Stack R;
    InitStack(&R);
    Book popBook;
    int i=1;
    printf("Current Books:\n");
    while(!isSEmpty(*S)){
        Pop(S, &popBook);
        printf("Book %d\n", i);
        printf("ID: %d\nTitle: %s\nAuthor: %s\n\n", popBook.id, popBook.title, popBook.author);
        i++;     
        Push(&R, popBook);
    }
    while(!isSEmpty(R)){
        Pop(&R, &popBook);
        Push(S, popBook);
    }
}

void DisplayQueue(Queue* Q){
    Queue F;
    InitQueue(&F);
    User popUser;
    int i=1;
    printf("Current Queue:\n");
    if(isQEmpty(*Q)){
        printf("None");
    }
    else{
    while(!isQEmpty(*Q)){
        Dequeue(Q, &popUser);
        printf("User %d\n", i);
        printf("ID: %d\nname: %s\nRequestedBookId: %d\n\n", popUser.id, popUser.name, popUser.requested_book_id);
        i++;
        Enqueue(&F, popUser);
    }
    while(!isQEmpty(F)){
        Dequeue(&F, &popUser);
        Enqueue(Q, popUser);
    }
    }
}

void SearchBook(Book book) {
    Book temp;
    Stack T;
    InitStack(&T); 
    bool found = false;

    while (!isSEmpty(Inventory)) {
        Pop(&Inventory, &temp);
        if (temp.id == book.id || strcmp(temp.title, book.title) == 0 || strcmp(temp.author, book.author) == 0) {
            printf("Book has been found!\nHere are more details:\nID: %d\nTitle: %s\nAuthor: %s\nAvailability: %s\n", temp.id, temp.title, temp.author, temp.available ? "true" : "false");
            found = true;
        }
        Push(&T, temp);
    }

    if (!found) {
        printf("This book is not available!\n");
    }

    while (!isSEmpty(T)) {
        Pop(&T, &temp);
        Push(&Inventory, temp);
    }
}

//an additional feature: a function that displays only the available books
void sortAvailable(){
    Book bk;
    Stack Temp;
    InitStack(&Temp);
    int i=1;
    while(!isSEmpty(Inventory)){
    Pop(&Inventory, &bk);        
    if(bk.available==true){
    printf("Book %d\n", i);
    printf("ID: %d\nTitle: %s\nAuthor: %s\n\n", bk.id, bk.title, bk.author);
    i++; 
    }
    Push(&Temp, bk);
    }
    while(!isSEmpty(Temp)){
    Pop(&Temp, &bk);
    Push(&Inventory, bk);
    }
}


void welcome(){
    int choice, choice1;
    Book book;
    User user;
    printf("\nWelcome to the library! Please select one of these options:\n1.Process operations.\n2.View status.\n3.Quit\n\nselect:");
    do{
    scanf("%d",&choice);
    }while (choice!=1&&choice!=2&&choice!=3);
    if(choice==1){
    printf("Select one of these operations:\n1.Add a new book to the library.\n2.Return a book.\n3.Request to borrow a book.\nselect:");
    do{scanf("%d",&choice1);}while(choice1!=1 && choice1!=2 && choice1!=3);
    if(choice1==1){
    printf("Enter your book's details:\n");
    printf("Book's ID:");
    scanf("%d",&book.id);
    printf("Book's title:");
    scanf("%s",book.title);
    printf("Book's author:");
    scanf("%s",book.author);
    book.available = true;
    AddBook(&Inventory, book);
    }else if(choice1==2){
    printf("Enter your book's details:\n");
    printf("Book's ID:");
    scanf("%d",&book.id);
    printf("Book's title:");
    scanf("%s",book.title);
    printf("Book's author:");
    scanf("%s",book.author);
    book.available = false;
    ReturnBook(book);
    }else if(choice1==3){
    printf("Enter your informations details:\n");
    printf("Your name:");
    scanf("%s",user.name);
    printf("Your ID:");
    scanf("%d",&user.id);
    printf("Your requested book's ID:");
    scanf("%d",&user.requested_book_id);
    BorrowBook(user, user.requested_book_id);
    ProcessRequests();
    }
    }else if(choice==2){
    printf("Select one of these operations:\n1.View Library's books.\n2.Search for a book.\n3.Show current request queue.\n4.View recently returned books.\n5.View available books.\n\nselect:");
    do{scanf("%d",&choice1);}while(choice1!=1 && choice1!=2 && choice1!=3 && choice1!=4 && choice1!=5);
    if(choice1==1){
    DisplayS(&Inventory);
    }else if(choice1==2){
    printf("Enter your book's title:");
    scanf("%s",book.title);
    printf("Enter your book's author:");
    scanf("%s",book.author);
    SearchBook(book);
    }else if(choice1==3){
    DisplayQueue(&RequestQ);
    }else if(choice1==4){
    printf("Recently returned books:\n");
    LoadRecentReturnedFromFile("recent_returned.txt", &RecentReturned);
    DisplayStack(&RecentReturned);
    }else if(choice1==5){
    printf("\nThe available books:\n");
    sortAvailable(&Inventory);
    }
    }else if(choice==3){
        return;
    }
    
}

int main(){
    InitLibrary();
    welcome();
    return 0;
}
