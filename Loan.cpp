#include "Loan.h"

#include <chrono>

Loan::Loan()
    : id(0), bookId(0), memberId(0), issueTimestamp(0),
      dueTimestamp(0), returnTimestamp(0) {}

Loan::Loan(int idValue, int bookIdValue, int memberIdValue,
           long long issueValue, long long dueValue, long long returnValue)
    : id(idValue), bookId(bookIdValue), memberId(memberIdValue),
      issueTimestamp(issueValue), dueTimestamp(dueValue),
      returnTimestamp(returnValue) {}

int Loan::getId() const { return id; }
int Loan::getBookId() const { return bookId; }
int Loan::getMemberId() const { return memberId; }
long long Loan::getIssueTimestamp() const { return issueTimestamp; }
long long Loan::getDueTimestamp() const { return dueTimestamp; }
long long Loan::getReturnTimestamp() const { return returnTimestamp; }
bool Loan::isActive() const { return returnTimestamp == 0; }

bool Loan::isOverdue() const {
    const auto now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
    return isActive() && static_cast<long long>(now) > dueTimestamp;
}

void Loan::markReturned(long long timestamp) {
    returnTimestamp = timestamp;
}
