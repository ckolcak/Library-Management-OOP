#include "BorrowRecord.h"

#include <sstream>   
#include <iomanip>   
#include <ctime>

BorrowRecord::BorrowRecord(const std::string& userID, const std::string& bookID):  
borrowerID(userID), 
borrowedBookID(bookID), 
isReturned(false), 
borrowDate(std::chrono::system_clock::now()), 
dueDate(borrowDate + std::chrono::hours(24*borrowDurationDays)),
returnDate{}
{}

std::string BorrowRecord::getBorrowerID() const {return borrowerID;}
std::string BorrowRecord::getBorrowedBookID() const {return borrowedBookID;}

std::chrono::system_clock::time_point BorrowRecord::getBorrowDate() const {return borrowDate;}
std::chrono::system_clock::time_point BorrowRecord::getDueDate() const {return dueDate;}
std::chrono::system_clock::time_point BorrowRecord::getReturnDate() const {return returnDate;}

bool BorrowRecord::getIsReturned() const {return isReturned;}

void BorrowRecord::markReturned(){
    if(isReturned) return;
    
    isReturned = true;
    returnDate = std::chrono::system_clock::now();
}

bool BorrowRecord::isOverdue() const {return !isReturned && std::chrono::system_clock::now() > dueDate ? true : false;}

std::string BorrowRecord::timeToString(std::chrono::system_clock::time_point timePoint) const{
    std::time_t t = std::chrono::system_clock::to_time_t(timePoint); 
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}