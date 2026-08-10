#include "BorrowManager.h"
#include <iostream>

bool BorrowManager::bm_BorrowBook(const std::string& memberID, const std::string& bookID){
    if(getActiveBorrowCount(memberID) >= maxBorrowLimit) {return false;}

    for(int i = 0; i < borrowRecords.size(); i++){
        if(borrowRecords[i].getBorrowedBookID() == bookID && !borrowRecords[i].getIsReturned()) {return false;}
    }
    BorrowRecord record(memberID, bookID);
    borrowRecords.push_back(record);
    return true;
}

bool BorrowManager::bm_ReturnBook(const std::string& memberID, const std::string& bookID){
    for(int i = 0; i < borrowRecords.size(); i++){
        if(borrowRecords[i].getBorrowedBookID() == bookID && borrowRecords[i].getBorrowerID() == memberID) {
            if(borrowRecords[i].getIsReturned()) {return false;}

            borrowRecords[i].markReturned(); 
            return true;
        }
    }
    std::cout << "Borrow record not found." << std::endl;
    return false;
}

int BorrowManager::getActiveBorrowCount(const std::string& userID) const {
    int cntr = 0;
    for(int i = 0; i < borrowRecords.size(); i++){
        if(borrowRecords[i].getBorrowerID() == userID && !(borrowRecords[i].getIsReturned())){
            cntr++;
        }
    }
    return cntr;
}

bool BorrowManager::isBookBorrowed(const std::string& bookID) const {
    for(int i = 0; i < borrowRecords.size(); i++){
        if(borrowRecords[i].getBorrowedBookID() == bookID && !borrowRecords[i].getIsReturned()) {return true;}
    }
    return false;
}

bool BorrowManager::hasActiveBorrow(const std::string& userID, const std::string& bookID) const {
    for(int i = 0; i < borrowRecords.size(); i++){
        if(borrowRecords[i].getBorrowerID() == userID &&
        borrowRecords[i].getBorrowedBookID() == bookID &&
        !borrowRecords[i].getIsReturned()) {return true;}
    }
    return false;
}

void BorrowManager::bm_displayBorrowInfo() const {
    if(borrowRecords.empty()){
        std::cout << "There are currently no borrow records" << std::endl;
        return;
    }
    for(int i = 0; i < borrowRecords.size(); i++){
    std::string returnStr = borrowRecords[i].getIsReturned() ? borrowRecords[i].timeToString(borrowRecords[i].getReturnDate()) : "Book is not returned. ";   
    std::cout << borrowRecords[i].getBorrowerID() << " | " 
    << borrowRecords[i].getBorrowedBookID() << " | "
    << borrowRecords[i].timeToString(borrowRecords[i].getBorrowDate()) << " | " 
    << borrowRecords[i].timeToString(borrowRecords[i].getDueDate()) << " | "
    << returnStr << " | " << std::endl;
    }
}
