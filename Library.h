#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Member.h"
#include "Loan.h"

#include <string>
#include <vector>
using namespace std;

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Loan> loans;
    int nextLoanId;

    Book* findBook(int id);
    const Book* findBook(int id) const;
    Member* findMember(int id);
    const Member* findMember(int id) const;
    Loan* findLoan(int id);
    void recalculateAvailability();
    bool saveBooks() const;
    bool saveMembers() const;
    bool saveLoans() const;

public:
    Library();

    bool loadData();
    bool saveData() const;

    bool addBook(const Book& book);
    bool updateBook(int id, const string& title, const string& author,
                    const string& category, int totalCopies);
    bool deleteBook(int id);
    void displayAllBooks() const;
    void searchBooks(const string& keyword) const;

    bool addMember(const Member& member);
    void displayMembers() const;

    bool issueBook(int bookId, int memberId);
    bool returnBook(int loanId);
    void displayLoans(bool activeOnly = false) const;
};

#endif
