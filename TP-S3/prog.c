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
        printf("Book has been borrowed successfully!");
        UpdateBookAvailabilityInFile("inventory.txt", rebook.id, false);
    } 
    else{
    Push(&R, rebook);
    }
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
    
    printf("\nBook returned!");
    SaveRecentReturnedToFile("recent_returned.txt", &RecentReturned);
    SaveBooksToFile("inventory.txt", &Inventory);

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

            // Restore book to inventory
            Push(&tempInventory, book);
        }

        // Restore inventory
        while (!isSEmpty(tempInventory)) {
            Pop(&tempInventory, &book);
            Push(&Inventory, book);
        }

        // If book was not processed, put back in queue
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

    // Update the request queue with remaining requests
    while (!isQEmpty(tempQueue)) {
        Dequeue(&tempQueue, &currentUser);
        Enqueue(&RequestQ, currentUser);
    }

    // Save updated queue and inventory
    if (processedAnyRequest) {
        SaveRequestQueueToFile(REQUEST_QUEUE_FILE, &RequestQ);
        SaveBooksToFile(INVENTORY_FILE, &Inventory);
    }
}

//Display stack function
void DisplayStack(Stack* S){
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
        printf("ID: %d\name: %s\nRequestedBookId: %d\n\n", popUser.id, popUser.name, popUser.requested_book_id);
        i++;
        Enqueue(&F, popUser);
    }
    while(!isQEmpty(F)){
        Dequeue(&F, &popUser);
        Enqueue(Q, popUser);
    }
    }
}

void SearchBook(Book book){
    Book temp;
    Stack T;
    while(!isSEmpty(Inventory)){
    Pop(&Inventory, &temp);
    if(temp.id == book.id || temp.author == book.author || temp.available == book.available || temp.title == book.title){
    printf("This book is available!");
    }
    else{
    printf("This book is not available!"); 
    }
    }
    Push(&T, temp);
    while (!isSEmpty(T)){
    Pop(&T, &temp);
    Push(&Inventory, temp);
    }
}

void sortAvailable(Stack*S){
    Book bk;
    Stack Temp;
    Stack avail;
    while(!isSEmpty(S)){
    Pop(&S, &bk);
    if(bk.available==true){
    Push(&avail, bk);
    }
    Push(&Temp, bk);
    }
    while(!isSEmpty(Temp)){
    Pop(&Temp, &bk);
    Push(&Inventory, bk);
    }
    DisplayStack(&avail);
}


void welcome(){
    int choice, choice1;
    Book book;
    User user;
    printf("\nWelcome to the library! Please select one of these options:\n1\.Process operations.\n2\.View status.\n3\.Quit\n\nselect:");
    do{
    scanf("%d",&choice);
    }while (choice!=1&&choice!=2&&choice!=3);
    if(choice==1){
    printf("Select one of these operations:\n1\.Add a new book to the library.\n2\.Return a book.\n3\.Request to borrow a book.\nselect:");
    do{scanf("%d",&choice1);}while(choice1!=1 && choice1!=2 && choice1!=3);
    if(choice1==1){
    printf("Enter your book's details:\n");
    printf("Book's ID:");
    scanf("%d",&book.id);
    printf("Book's title:");
    scanf("%s",&book.title);
    printf("Book's author:");
    scanf("%s",&book.author);
    book.available = true;
    AddBook(&Inventory, book);
    }else if(choice1==2){
    printf("Enter your book's details:\n");
    printf("Book's ID:");
    scanf("%d",&book.id);
    printf("Book's title:");
    scanf("%s",&book.title);
    printf("Book's author:");
    scanf("%s",&book.author);
    book.available = false;
    ReturnBook(book);
    }else if(choice1==3){
    printf("Enter your informations details:\n");
    printf("Your name:");
    scanf("%s",&user.name);
    printf("Your ID:");
    scanf("%d",&user.id);
    printf("Your requested book's ID:");
    scanf("%d",&user.requested_book_id);
    BorrowBook(user, user.requested_book_id);
    ProcessRequests();
    }
    }else if(choice==2){
    printf("Select one of these operations:\n1\.View Library's books.\n2\.Search for a book.\n3\.Show current request queue.\n4\.View recently returned books.\n5\.View available books.\nselect:");
    do{scanf("%d",&choice1);}while(choice1!=1 && choice1!=2 && choice1!=3 && choice1!=4 && choice1!=5);
    if(choice1==1){
    DisplayStack(&Inventory);
    }else if(choice1==2){
    printf("Enter your book's title:\n");
    scanf("%s",&book.title);
    printf("Enter your book's author:\n");
    scanf("%s",&book.author);
    SearchBook(book);
    }else if(choice1==3){
    DisplayQueue(&RequestQ);
    }else if(choice1==4){
    printf("Recently returned books:\n");
    LoadRecentReturnedFromFile("recent_returned.txt", &RecentReturned);
    DisplayStack(&RecentReturned);
    }else if(choice1==5){
    sortAvailable(&Inventory);
    }
    }else if(choice==3){
        return;
    }
    
}

int main(){
    // User user1 = { 5, "Imad", 1};
    // Book book1 = {1, "1984", "George Orwell", true},book2 = {2, "HH", "LEO MESSI", true}, book3 = {4, "GG", "Nigga", false};
    // Book book5 = { 8, "Red", "Author", true };
    InitLibrary();
    welcome();
    return 0;
}
