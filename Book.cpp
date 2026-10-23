#include "Book.h"

#include <algorithm>

Book::Book()
    : id(0), totalCopies(0), availableCopies(0) {}

Book::Book(int idValue, const string& titleValue, const string& authorValue,
           const string& categoryValue, int totalCopiesValue)
    : id(idValue), title(titleValue), author(authorValue), category(categoryValue),
      totalCopies(totalCopiesValue), availableCopies(totalCopiesValue) {}

int Book::getId() const { return id; }
const string& Book::getTitle() const { return title; }
const string& Book::getAuthor() const { return author; }
const string& Book::getCategory() const { return category; }
int Book::getTotalCopies() const { return totalCopies; }
int Book::getAvailableCopies() const { return availableCopies; }

void Book::setTitle(const string& value) { title = value; }
void Book::setAuthor(const string& value) { author = value; }
void Book::setCategory(const string& value) { category = value; }

bool Book::setTotalCopies(int value) {
    if (value < 1 || value < (totalCopies - availableCopies)) {
        return false;
    }
    const int borrowedCopies = totalCopies - availableCopies;
    totalCopies = value;
    availableCopies = totalCopies - borrowedCopies;
    return true;
}

void Book::setAvailableCopies(int value) {
    availableCopies = max(0, min(value, totalCopies));
}

bool Book::issueCopy() {
    if (availableCopies <= 0) return false;
    --availableCopies;
    return true;
}

bool Book::returnCopy() {
    if (availableCopies >= totalCopies) return false;
    ++availableCopies;
    return true;
}
