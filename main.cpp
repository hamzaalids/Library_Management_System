#include "Library.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

namespace {
    int readInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cout << "Please enter a whole number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    string readText(const string& prompt) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
            if (value.empty()) cout << "This field cannot be empty.\n";
        } while (value.empty());
        return value;
    }

    void showMenu() {
        cout << "\n============================================\n"
             << "        LIBRARY MANAGEMENT SYSTEM\n"
             << "============================================\n"
             << " 1. Add book\n"
             << " 2. Display all books\n"
             << " 3. Search books\n"
             << " 4. Update book details\n"
             << " 5. Delete book\n"
             << " 6. Register member\n"
             << " 7. Display members\n"
             << " 8. Issue a book\n"
             << " 9. Return a book\n"
             << "10. Display all loans\n"
             << "11. Display active loans\n"
             << " 0. Save and exit\n"
             << "============================================\n";
    }
}

int main() {
    Library library;
    if (!library.loadData()) {
        cerr << "Warning: could not load saved data. Starting with available records.\n";
    }

    cout << "Welcome! Data files are stored in the program's working folder.\n";

    while (true) {
        showMenu();
        int choice = readInt("Enter choice: ");

        switch (choice) {
        case 1: {
            int id = readInt("Book ID: ");
            string title = readText("Title: ");
            string author = readText("Author: ");
            string category = readText("Category: ");
            int copies = readInt("Total copies: ");
            if (id <= 0 || copies <= 0) {
                cout << "ID and copy count must be positive.\n";
                break;
            }
            if (library.addBook(Book(id, title, author, category, copies)))
                cout << "Book added and saved.\n";
            else
                cout << "Could not add book. Check for a duplicate ID or file write problem.\n";
            break;
        }
        case 2:
            library.displayAllBooks();
            break;
        case 3: {
            string keyword = readText("Search by ID, title, author, or category: ");
            library.searchBooks(keyword);
            break;
        }
        case 4: {
            int id = readInt("Book ID to update: ");
            string title = readText("New title: ");
            string author = readText("New author: ");
            string category = readText("New category: ");
            int copies = readInt("New total copies: ");
            if (library.updateBook(id, title, author, category, copies))
                cout << "Book updated and saved.\n";
            else
                cout << "Update failed. Check the ID, fields, and number of copies (cannot be less than copies currently borrowed).\n";
            break;
        }
        case 5: {
            int id = readInt("Book ID to delete: ");
            if (library.deleteBook(id))
                cout << "Book deleted.\n";
            else
                cout << "Delete failed: book not found or currently has an active loan.\n";
            break;
        }
        case 6: {
            int id = readInt("Member ID: ");
            string name = readText("Full name: ");
            cout << "Email (enter - if unavailable): ";
            string email;
            getline(cin, email);
            cout << "Phone (enter - if unavailable): ";
            string phone;
            getline(cin, phone);
            if (id > 0 && library.addMember(Member(id, name, email, phone)))
                cout << "Member registered and saved.\n";
            else
                cout << "Registration failed. Check the member ID or saved-data folder.\n";
            break;
        }
        case 7:
            library.displayMembers();
            break;
        case 8: {
            int bookId = readInt("Book ID to issue: ");
            int memberId = readInt("Member ID borrowing the book: ");
            if (library.issueBook(bookId, memberId))
                cout << "Book issued. It is due in 14 days. Use option 10 to see the loan ID.\n";
            else
                cout << "Issue failed: check the book/member IDs and available copies.\n";
            break;
        }
        case 9: {
            int loanId = readInt("Loan ID to return: ");
            if (library.returnBook(loanId))
                cout << "Book returned and record saved.\n";
            else
                cout << "Return failed: loan not found or already returned.\n";
            break;
        }
        case 10:
            library.displayLoans(false);
            break;
        case 11:
            library.displayLoans(true);
            break;
        case 0:
            if (library.saveData())
                cout << "All data saved. Goodbye!\n";
            else
                cout << "Warning: at least one data file could not be saved.\n";
            return 0;
        default:
            cout << "Invalid choice. Select a number from 0 to 11.\n";
        }
    }
}
