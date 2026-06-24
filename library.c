#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define FILENAME "books.dat"

typedef struct {
    int id;
    char title[100];
    char author[100];
    char genre[50];
    int year;
    int is_issued;
    char issued_to[100];
} Book;

Book books[MAX_BOOKS];
int book_count = 0;

void clear_screen() { system("clear || cls"); }

void load_books() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) return;
    fread(&book_count, sizeof(int), 1, fp);
    fread(books, sizeof(Book), book_count, fp);
    fclose(fp);
}

void save_books() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) { printf("Error saving data!\n"); return; }
    fwrite(&book_count, sizeof(int), 1, fp);
    fwrite(books, sizeof(Book), book_count, fp);
    fclose(fp);
}

void print_line() {
    printf("============================================================\n");
}

void print_header() {
    print_line();
    printf("           📚  LIBRARY MANAGEMENT SYSTEM  📚\n");
    print_line();
}

void add_book() {
    if (book_count >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }
    Book b;
    b.id = book_count + 1;
    b.is_issued = 0;
    strcpy(b.issued_to, "N/A");

    printf("\nEnter Book Title  : "); scanf(" %[^\n]", b.title);
    printf("Enter Author Name : "); scanf(" %[^\n]", b.author);
    printf("Enter Genre       : "); scanf(" %[^\n]", b.genre);
    printf("Enter Year        : "); scanf("%d", &b.year);

    books[book_count++] = b;
    save_books();
    printf("\n✅ Book added successfully! (ID: %d)\n", b.id);
}

void display_all_books() {
    if (book_count == 0) { printf("\nNo books found!\n"); return; }
    printf("\n%-5s %-30s %-20s %-15s %-6s %-10s\n",
           "ID", "Title", "Author", "Genre", "Year", "Status");
    print_line();
    for (int i = 0; i < book_count; i++) {
        printf("%-5d %-30s %-20s %-15s %-6d %-10s\n",
               books[i].id, books[i].title, books[i].author,
               books[i].genre, books[i].year,
               books[i].is_issued ? "Issued" : "Available");
    }
}

void search_book() {
    char query[100];
    printf("\nEnter title or author to search: ");
    scanf(" %[^\n]", query);
    int found = 0;
    for (int i = 0; i < book_count; i++) {
        if (strstr(books[i].title, query) || strstr(books[i].author, query)) {
            printf("\nID     : %d\n", books[i].id);
            printf("Title  : %s\n", books[i].title);
            printf("Author : %s\n", books[i].author);
            printf("Genre  : %s\n", books[i].genre);
            printf("Year   : %d\n", books[i].year);
            printf("Status : %s\n", books[i].is_issued ? "Issued" : "Available");
            print_line();
            found++;
        }
    }
    if (!found) printf("No books found matching '%s'\n", query);
}

void issue_book() {
    int id;
    printf("\nEnter Book ID to issue: ");
    scanf("%d", &id);
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) {
            if (books[i].is_issued) {
                printf("❌ Book is already issued to %s\n", books[i].issued_to);
                return;
            }
            printf("Enter student name: ");
            scanf(" %[^\n]", books[i].issued_to);
            books[i].is_issued = 1;
            save_books();
            printf("✅ Book issued to %s successfully!\n", books[i].issued_to);
            return;
        }
    }
    printf("❌ Book ID %d not found!\n", id);
}

void return_book() {
    int id;
    printf("\nEnter Book ID to return: ");
    scanf("%d", &id);
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) {
            if (!books[i].is_issued) {
                printf("❌ This book was not issued!\n");
                return;
            }
            printf("✅ Book returned from %s\n", books[i].issued_to);
            books[i].is_issued = 0;
            strcpy(books[i].issued_to, "N/A");
            save_books();
            return;
        }
    }
    printf("❌ Book ID %d not found!\n", id);
}

void delete_book() {
    int id;
    printf("\nEnter Book ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) {
            for (int j = i; j < book_count - 1; j++)
                books[j] = books[j + 1];
            book_count--;
            save_books();
            printf("✅ Book deleted successfully!\n");
            return;
        }
    }
    printf("❌ Book ID %d not found!\n", id);
}

int main() {
    load_books();
    int choice;
    do {
        clear_screen();
        print_header();
        printf("  1. Add Book\n");
        printf("  2. Display All Books\n");
        printf("  3. Search Book\n");
        printf("  4. Issue Book\n");
        printf("  5. Return Book\n");
        printf("  6. Delete Book\n");
        printf("  0. Exit\n");
        print_line();
        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_book(); break;
            case 2: display_all_books(); break;
            case 3: search_book(); break;
            case 4: issue_book(); break;
            case 5: return_book(); break;
            case 6: delete_book(); break;
            case 0: printf("\n👋 Goodbye!\n"); break;
            default: printf("Invalid choice!\n");
        }
        if (choice != 0) { printf("\nPress Enter to continue..."); getchar(); getchar(); }
    } while (choice != 0);

    return 0;
}
