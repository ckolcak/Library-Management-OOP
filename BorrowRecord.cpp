#include "BorrowRecord.h"

BorrowRecord::BorrowRecord(const std::string& userID, int bookID):  
borrowerID(userID), borrowedBookID(bookID) {}

std::string BorrowRecord::getBorrowerID() const {return borrowerID;}
int BorrowRecord::getBorrowedBookID() const {return borrowedBookID;}

std::chrono::system_clock::time_point BorrowRecord::getBorrowDate() const {return borrowDate;}
std::chrono::system_clock::time_point BorrowRecord::getReturnDate() const {return returnDate;}

void BorrowRecord::setBorrowDate(){
    borrowDate = std::chrono::system_clock::now();
}
void BorrowRecord::setReturnDate(){
    returnDate = std::chrono::system_clock::now();
}
