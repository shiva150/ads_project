#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for a book
struct Book {
    int book_id;
    char title[100];
    char author[50];
    int quantity;
};

// Define a structure for a transaction
struct Transaction {
    char username[50];
    char date[20];
    int quantity;
    struct Transaction* next;
};

// Define a structure for a book node in the binary tree
struct BookNode {
    int book_id;
    char title[100];
    char author[50];
    int quantity;
    struct Transaction* transactions;
    struct BookNode* left;
    struct BookNode* right;
};

// Function prototypes
struct BookNode* createBookNode(struct Book newBook);
struct BookNode* insertBook(struct BookNode* root, struct Book newBook);
void displayBooksInOrder(struct BookNode* root);
struct BookNode* findBook(struct BookNode* root, char title[]);
void borrowBook(struct BookNode* root, char title[], char username[], char date[], int quantity);
void returnBook(struct BookNode* root, char title[], char username[], char date[], int quantity);
void addTransaction(struct Transaction** transactions, char username[], char date[], int quantity);
void displayTransactions(struct Transaction* transactions);
void addNewBook(struct BookNode** root);
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    struct BookNode* root = NULL;

    // Example books
    struct Book books[] = {
        {1, "The Catcher in the Rye", "J.D. Salinger", 5},
        {2, "To Kill a Mockingbird", "Harper Lee", 3},
        {3, "1984", "George Orwell", 7}
        // Add more books as needed
    };

    // Insert books into the binary tree
    for (int i = 0; i < sizeof(books) / sizeof(books[0]); i++) {
        root = insertBook(root, books[i]);
    }

    int choice;
    char searchTitle[100];
    char username[50];
    char date[20];

    do {
        // Display menu
        printf("\nLibrary Management System Menu\n");
        printf("1. Display Books\n");
        printf("2. Search Book\n");
        printf("3. Borrow Book\n");
        printf("4. Return Book\n");
        printf("5. Display Transactions\n");
        printf("6. Add New Book\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
         clearInputBuffer();
        switch (choice) {
            case 1:
                printf("Books in the library:\n");
                displayBooksInOrder(root);
                break;
            case 2:
                printf("Enter the title of the book to search: ");
                scanf("%s", searchTitle);
                struct BookNode* foundBook = findBook(root, searchTitle);
                if (foundBook != NULL) {
                    printf("Book found!\n");
                    printf("Title: %s\nAuthor: %s\nQuantity: %d\n", foundBook->title, foundBook->author, foundBook->quantity);
                } else {
                    printf("Book not found.\n");
                }
                break;
            case 3:
                printf("Enter the title of the book to borrow: ");
                scanf("%s", searchTitle);
                printf("Enter your username: ");
                scanf("%s", username);
                printf("Enter the date (YYYY-MM-DD): ");
                scanf("%s", date);
                int quantityToBorrow;
                printf("Enter the quantity to borrow: ");
                scanf("%d", &quantityToBorrow);
                borrowBook(root, searchTitle, username, date, quantityToBorrow);
                break;
            case 4:
                printf("Enter the title of the book to return: ");
                scanf("%s", searchTitle);
                printf("Enter your username: ");
                scanf("%s", username);
                printf("Enter the date (YYYY-MM-DD): ");
                scanf("%s", date);
                int quantityToReturn;
                printf("Enter the quantity to return: ");
                scanf("%d", &quantityToReturn);
                returnBook(root, searchTitle, username, date, quantityToReturn);
                break;
            case 5:
                printf("Enter the title of the book to display transactions: ");
                scanf("%s", searchTitle);
                struct BookNode* bookWithTransactions = findBook(root, searchTitle);
                if (bookWithTransactions != NULL) {
                    displayTransactions(bookWithTransactions->transactions);
                } else {
                    printf("Book not found.\n");
                }
                break;
            case 6:
                addNewBook(&root);
                break;
            case 0:
                printf("Exiting the program. Thank you!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);

    return 0;
}

struct BookNode* createBookNode(struct Book newBook) {
    // Helper function to create a new book node
    struct BookNode* newNode = (struct BookNode*)malloc(sizeof(struct BookNode));
    newNode->book_id = newBook.book_id;
    strcpy(newNode->title, newBook.title);
    strcpy(newNode->author, newBook.author);
    newNode->quantity = newBook.quantity;
    newNode->transactions = NULL;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct BookNode* insertBook(struct BookNode* root, struct Book newBook) {
    // Function to insert a book into the binary tree
    if (root == NULL) {
        return createBookNode(newBook);
    }

    // Compare book titles to determine the insertion point
    int compareResult = strcmp(newBook.title, root->title);

    if (compareResult < 0) {
        root->left = insertBook(root->left, newBook);
    } else if (compareResult > 0) {
        root->right = insertBook(root->right, newBook);
    } else {
        // Book with the same title already exists, update quantity or handle accordingly
        root->quantity += newBook.quantity;
    }

    return root;
}

void displayBooksInOrder(struct BookNode* root) {
    // Function to display books in the binary tree in ascending order of title
    if (root != NULL) {
        displayBooksInOrder(root->left);
        printf("Title: %s\tAuthor: %s\tQuantity: %d\n", root->title, root->author, root->quantity);
        displayBooksInOrder(root->right);
    }
}

struct BookNode* findBook(struct BookNode* root, char title[]) {
    // Function to search for a book in the binary tree by title
    if (root == NULL || strcmp(title, root->title) == 0) {
        return root;
    }

    if (strcmp(title, root->title) < 0) {
        return findBook(root->left, title);
    } else {
        return findBook(root->right, title);
    }
}

void borrowBook(struct BookNode* root, char title[], char username[], char date[], int quantity) {
    // Function to simulate borrowing a book
    // Update the book quantity in the binary tree
    struct BookNode* book = findBook(root, title);

    if (book != NULL && book->quantity >= quantity) {
        book->quantity -= quantity;
        addTransaction(&(book->transactions), username, date, quantity);
        printf("Book borrowed successfully!\n");
    } else {
        printf("Book not available for borrowing or insufficient quantity.\n");
    }
}

void returnBook(struct BookNode* root, char title[], char username[], char date[], int quantity) {
    // Function to simulate returning a book
    // Update the book quantity in the binary tree
    struct BookNode* book = findBook(root, title);

    if (book != NULL) {
        book->quantity += quantity;
        addTransaction(&(book->transactions), username, date, -quantity);
        printf("Book returned successfully!\n");
    } else {
        printf("Book not found.\n");
    }
}

void addTransaction(struct Transaction** transactions, char username[], char date[], int quantity) {
    // Function to add a transaction to the transaction list of a book
    struct Transaction* newTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
    strcpy(newTransaction->username, username);
    strcpy(newTransaction->date, date);
    newTransaction->quantity = quantity;
    newTransaction->next = *transactions;
    *transactions = newTransaction;
}

void displayTransactions(struct Transaction* transactions) {
    // Function to display transactions for a book
    printf("Transaction History:\n");
    while (transactions != NULL) {
        printf("Username: %s\tDate: %s\tQuantity: %d\n", transactions->username, transactions->date, transactions->quantity);
        transactions = transactions->next;
    }
}

void addNewBook(struct BookNode** root) {
    // Function to add a new book to the library
    struct Book newBook;
    printf("Enter the book details:\n");
    printf("Book ID: ");
    scanf("%d", &newBook.book_id);
    printf("Title: ");
    scanf("%s", newBook.title);
    printf("Author: ");
    scanf("%s", newBook.author);
    printf("Quantity: ");
    scanf("%d", &newBook.quantity);

    *root = insertBook(*root, newBook);
    printf("New book added successfully!\n");
}
