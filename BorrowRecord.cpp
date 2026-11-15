#include "BorrowRecord.h"

BorrowRecord::BorrowRecord(const std::string& userID, const std::string& bookID):  
recordID(-1),
userID(userID), 
bookID(bookID), 
borrowDate(SysClock::now()), 
dueDate(borrowDate + std::chrono::hours(24*borrowDurationDays)),
returnDate(std::nullopt)
{}

BorrowRecord::BorrowRecord(
int recordID,
const std::string& userID, 
const std::string& bookID,
const SysClock::time_point& borrowDate,
const SysClock::time_point& dueDate,
const std::optional<SysClock::time_point>& returnDate
):
recordID(recordID),
userID(userID),
bookID(bookID),
borrowDate(borrowDate),
dueDate(dueDate),
returnDate(returnDate)
{}

int BorrowRecord::getRecordID() const {return recordID;}
std::string BorrowRecord::getUserID() const {return userID;}
std::string BorrowRecord::getBorrowedBookID() const {return bookID;}

SysClock::time_point BorrowRecord::getBorrowDate() const {return borrowDate;}
SysClock::time_point BorrowRecord::getDueDate() const {return dueDate;}
std::optional<SysClock::time_point> BorrowRecord::getReturnDate() const {
    return returnDate;
}
