#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    string category;
    int totalCopies;
    int availableCopies;

public:
    Book();
    Book(int id, const string& title, const string& author,
         const string& category, int totalCopies);

    int getId() const;
    const string& getTitle() const;
    const string& getAuthor() const;
    const string& getCategory() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    void setTitle(const string& value);
    void setAuthor(const string& value);
    void setCategory(const string& value);
    bool setTotalCopies(int value);
    void setAvailableCopies(int value);

    bool issueCopy();
    bool returnCopy();
};

#endif
