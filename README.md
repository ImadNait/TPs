# Algo 3 TP Project

### This project is divided into 2 parts:

# Part 1: Task Management System 
a simple task management system using a singly linked list. Each 
task has a unique identifier, a description, a priority level (1-5), and a status 
("Pending", "In Progress" or "Completed").

## system features:
* **Task Insertion:** Implement a function to add a new task to the list. Tasks 
should be added in order of priority (higher priority tasks come first).
* **Task Deletion:** Implement a function to delete a task by its identifier. 
* **Update Status:** Implement a function to update the status of a task by its 
identifier. 
* **Display Tasks:** Implement a function to display all tasks, grouped by their 
current status (i.e., "Pending" tasks first, followed by "In Progress" and then 
"Completed"). 
* **Search by Priority:** Implement a function to search and display all tasks 
with a specific priority level. 

# Part 2: Library Book Management System
a system that manages a library's books and borrowing operations using stacks 
and queues. The stacks are used for organizing the "recently returned books" section, and the library's "inventory" section, 
while the queue is used to manage a "borrow request queue" for users waiting to 
borrow popular books.

## system features:
* **Add New Book:** Add a book to the library's inventory.
*  **Borrow Book:** Allow a user to borrow a book if available, if the book is currently unavailable, the user is added to the borrow 
request queue. 
* **Return Book:** Returned books are placed in the "recently returned" stack. 
* **Process Borrow Requests:** When a book becomes available, process the next user in the borrow 
request queue. 
* **View Recently Returned Books:** Display books in the "recently returned" stack. 
* **Display Borrow Request Queue:** Show the current queue of users waiting for books. 
* **Search for a Book:** Check if a book is available in the library's inventory. 
