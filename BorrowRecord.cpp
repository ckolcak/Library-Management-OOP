#include "BorrowRecord.h"

BorrowRecord::BorrowRecord(const std::string& userID, const std::string& bookID):  
borrowerID(userID), 
borrowedBookID(bookID), 
isReturned(false), 
borrowDate(std::chrono::system_clock::now()), 
dueDate(borrowDate + std::chrono::hours(24*borrowDurationDays)),
returnDate{}
{}

BorrowRecord::BorrowRecord(
const std::string& userID, 
const std::string& bookID,
const std::chrono::system_clock::time_point& borrowDate,
const std::chrono::system_clock::time_point& dueDate,
const std::chrono::system_clock::time_point& returnDate,
bool isReturned
):
borrowerID(userID),
borrowedBookID(bookID),
borrowDate(borrowDate),
dueDate(dueDate),
returnDate(returnDate),
isReturned(isReturned)
{}

std::string BorrowRecord::getBorrowerID() const {return borrowerID;}
std::string BorrowRecord::getBorrowedBookID() const {return borrowedBookID;}

std::chrono::system_clock::time_point BorrowRecord::getBorrowDate() const {return borrowDate;}
std::chrono::system_clock::time_point BorrowRecord::getDueDate() const {return dueDate;}
std::chrono::system_clock::time_point BorrowRecord::getReturnDate() const {return returnDate;}

bool BorrowRecord::getIsReturned() const {return isReturned;}

void BorrowRecord::markReturned() {
    if(isReturned) return;
    
    isReturned = true;
    returnDate = std::chrono::system_clock::now();
}

bool BorrowRecord::isOverdue() const {return !isReturned && std::chrono::system_clock::now() > dueDate ? true : false;}
