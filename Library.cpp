#include "Library.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

namespace {
    const char* BOOKS_FILE = "books.txt";
    const char* MEMBERS_FILE = "members.txt";
    const char* LOANS_FILE = "loans.txt";

    string lowerText(string value) {
        transform(value.begin(), value.end(), value.begin(),
            [](unsigned char ch) { return static_cast<char>(tolower(ch)); });
        return value;
    }

    string timestampText(long long timestamp) {
        if (timestamp == 0) return "-";
        time_t raw = static_cast<time_t>(timestamp);
        tm* local = localtime(&raw);
        if (!local) return "Unknown";
        ostringstream out;
        out << put_time(local, "%Y-%m-%d");
        return out.str();
    }

    long long nowTimestamp() {
        return static_cast<long long>(chrono::system_clock::to_time_t(
            chrono::system_clock::now()));
    }
}

Library::Library() : nextLoanId(1) {}

Book* Library::findBook(int id) {
    for (Book& book : books) if (book.getId() == id) return &book;
    return nullptr;
}
const Book* Library::findBook(int id) const {
    for (const Book& book : books) if (book.getId() == id) return &book;
    return nullptr;
}
Member* Library::findMember(int id) {
    for (Member& member : members) if (member.getId() == id) return &member;
    return nullptr;
}
const Member* Library::findMember(int id) const {
    for (const Member& member : members) if (member.getId() == id) return &member;
    return nullptr;
}
Loan* Library::findLoan(int id) {
    for (Loan& loan : loans) if (loan.getId() == id) return &loan;
    return nullptr;
}

void Library::recalculateAvailability() {
    for (Book& book : books) {
        int activeLoans = 0;
        for (const Loan& loan : loans) {
            if (loan.getBookId() == book.getId() && loan.isActive()) ++activeLoans;
        }
        book.setAvailableCopies(book.getTotalCopies() - activeLoans);
    }
}

bool Library::loadData() {
    books.clear();
    members.clear();
    loans.clear();
    nextLoanId = 1;

    {
        ifstream in(BOOKS_FILE);
        int id, total;
        string title, author, category;
        while (in >> id >> quoted(title) >> quoted(author) >> quoted(category) >> total) {
            if (id > 0 && total > 0 && !findBook(id)) {
                books.emplace_back(id, title, author, category, total);
            }
        }
    }
    {
        ifstream in(MEMBERS_FILE);
        int id;
        string name, email, phone;
        while (in >> id >> quoted(name) >> quoted(email) >> quoted(phone)) {
            if (id > 0 && !findMember(id)) {
                members.emplace_back(id, name, email, phone);
            }
        }
    }
    {
        ifstream in(LOANS_FILE);
        int id, bookId, memberId;
        long long issued, due, returned;
        while (in >> id >> bookId >> memberId >> issued >> due >> returned) {
            if (id > 0) {
                loans.emplace_back(id, bookId, memberId, issued, due, returned);
                nextLoanId = max(nextLoanId, id + 1);
            }
        }
    }
    recalculateAvailability();
    return true;
}

bool Library::saveBooks() const {
    ofstream out(BOOKS_FILE, ios::trunc);
    if (!out) return false;
    for (const Book& b : books) {
        out << b.getId() << ' ' << quoted(b.getTitle()) << ' '
            << quoted(b.getAuthor()) << ' ' << quoted(b.getCategory()) << ' '
            << b.getTotalCopies() << '\n';
    }
    return static_cast<bool>(out);
}
bool Library::saveMembers() const {
    ofstream out(MEMBERS_FILE, ios::trunc);
    if (!out) return false;
    for (const Member& m : members) {
        out << m.getId() << ' ' << quoted(m.getName()) << ' '
            << quoted(m.getEmail()) << ' ' << quoted(m.getPhone()) << '\n';
    }
    return static_cast<bool>(out);
}
bool Library::saveLoans() const {
    ofstream out(LOANS_FILE, ios::trunc);
    if (!out) return false;
    for (const Loan& l : loans) {
        out << l.getId() << ' ' << l.getBookId() << ' ' << l.getMemberId() << ' '
            << l.getIssueTimestamp() << ' ' << l.getDueTimestamp() << ' '
            << l.getReturnTimestamp() << '\n';
    }
    return static_cast<bool>(out);
}
bool Library::saveData() const {
    return saveBooks() && saveMembers() && saveLoans();
}

bool Library::addBook(const Book& book) {
    if (book.getId() <= 0 || book.getTitle().empty() || book.getAuthor().empty()
        || book.getCategory().empty() || book.getTotalCopies() < 1
        || findBook(book.getId())) return false;
    books.push_back(book);
    return saveData();
}

bool Library::updateBook(int id, const string& title, const string& author,
                         const string& category, int totalCopies) {
    Book* book = findBook(id);
    if (!book || title.empty() || author.empty() || category.empty() || totalCopies < 1)
        return false;
    int borrowed = book->getTotalCopies() - book->getAvailableCopies();
    if (totalCopies < borrowed) return false;
    book->setTitle(title);
    book->setAuthor(author);
    book->setCategory(category);
    if (!book->setTotalCopies(totalCopies)) return false;
    return saveData();
}

bool Library::deleteBook(int id) {
    Book* book = findBook(id);
    if (!book) return false;
    for (const Loan& loan : loans) {
        if (loan.getBookId() == id && loan.isActive()) return false;
    }
    books.erase(remove_if(books.begin(), books.end(),
        [id](const Book& b) { return b.getId() == id; }), books.end());
    return saveData();
}

void Library::displayAllBooks() const {
    if (books.empty()) {
        cout << "\nNo books in the catalogue yet.\n";
        return;
    }
    cout << "\nBOOK CATALOGUE\n";
    cout << left << setw(8) << "ID" << setw(30) << "Title"
         << setw(22) << "Author" << setw(18) << "Category"
         << "Available/Total\n";
    cout << string(96, '-') << '\n';
    for (const Book& b : books) {
        cout << left << setw(8) << b.getId()
             << setw(30) << b.getTitle().substr(0, 28)
             << setw(22) << b.getAuthor().substr(0, 20)
             << setw(18) << b.getCategory().substr(0, 16)
             << b.getAvailableCopies() << '/' << b.getTotalCopies() << '\n';
    }
}

void Library::searchBooks(const string& keyword) const {
    string key = lowerText(keyword);
    bool found = false;
    for (const Book& b : books) {
        string id = to_string(b.getId());
        if (lowerText(b.getTitle()).find(key) != string::npos ||
            lowerText(b.getAuthor()).find(key) != string::npos ||
            lowerText(b.getCategory()).find(key) != string::npos ||
            id.find(key) != string::npos) {
            if (!found) {
                cout << "\nMatching books:\n";
                cout << left << setw(8) << "ID" << setw(30) << "Title"
                     << setw(22) << "Author" << setw(18) << "Category"
                     << "Available/Total\n";
            }
            cout << left << setw(8) << b.getId()
                 << setw(30) << b.getTitle().substr(0, 28)
                 << setw(22) << b.getAuthor().substr(0, 20)
                 << setw(18) << b.getCategory().substr(0, 16)
                 << b.getAvailableCopies() << '/' << b.getTotalCopies() << '\n';
            found = true;
        }
    }
    if (!found) cout << "No matching books found.\n";
}

bool Library::addMember(const Member& member) {
    if (member.getId() <= 0 || member.getName().empty() || findMember(member.getId()))
        return false;
    members.push_back(member);
    return saveData();
}

void Library::displayMembers() const {
    if (members.empty()) {
        cout << "\nNo registered members.\n";
        return;
    }
    cout << "\nMEMBERS\n";
    cout << left << setw(8) << "ID" << setw(28) << "Name"
         << setw(34) << "Email" << "Phone\n";
    cout << string(85, '-') << '\n';
    for (const Member& m : members) {
        cout << left << setw(8) << m.getId()
             << setw(28) << m.getName().substr(0, 26)
             << setw(34) << m.getEmail().substr(0, 32)
             << m.getPhone() << '\n';
    }
}

bool Library::issueBook(int bookId, int memberId) {
    Book* book = findBook(bookId);
    if (!book || !findMember(memberId) || book->getAvailableCopies() <= 0)
        return false;

    const long long issued = nowTimestamp();
    const long long due = issued + 14LL * 24 * 60 * 60;
    if (!book->issueCopy()) return false;
    loans.emplace_back(nextLoanId++, bookId, memberId, issued, due, 0);
    return saveData();
}

bool Library::returnBook(int loanId) {
    Loan* loan = findLoan(loanId);
    if (!loan || !loan->isActive()) return false;
    Book* book = findBook(loan->getBookId());
    if (!book || !book->returnCopy()) return false;
    loan->markReturned(nowTimestamp());
    return saveData();
}

void Library::displayLoans(bool activeOnly) const {
    bool found = false;
    cout << "\nLOAN RECORDS (loan period: 14 days)\n";
    cout << left << setw(8) << "Loan ID" << setw(10) << "Book ID"
         << setw(11) << "Member ID" << setw(14) << "Issued"
         << setw(14) << "Due" << setw(14) << "Returned" << "Status\n";
    cout << string(91, '-') << '\n';
    for (const Loan& loan : loans) {
        if (activeOnly && !loan.isActive()) continue;
        cout << left << setw(8) << loan.getId()
             << setw(10) << loan.getBookId()
             << setw(11) << loan.getMemberId()
             << setw(14) << timestampText(loan.getIssueTimestamp())
             << setw(14) << timestampText(loan.getDueTimestamp())
             << setw(14) << timestampText(loan.getReturnTimestamp());
        if (!loan.isActive()) cout << "Returned";
        else if (loan.isOverdue()) cout << "OVERDUE";
        else cout << "Borrowed";
        cout << '\n';
        found = true;
    }
    if (!found) cout << "No matching loan records.\n";
}
