#ifndef LOAN_H
#define LOAN_H

#include <ctime>

class Loan {
private:
    int id;
    int bookId;
    int memberId;
    long long issueTimestamp;
    long long dueTimestamp;
    long long returnTimestamp; // 0 means the book has not been returned

public:
    Loan();
    Loan(int id, int bookId, int memberId, long long issueTimestamp,
         long long dueTimestamp, long long returnTimestamp = 0);

    int getId() const;
    int getBookId() const;
    int getMemberId() const;
    long long getIssueTimestamp() const;
    long long getDueTimestamp() const;
    long long getReturnTimestamp() const;
    bool isActive() const;
    bool isOverdue() const;
    void markReturned(long long timestamp);
};

#endif
