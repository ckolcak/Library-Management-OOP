#include "BorrowManager.h"
#include <iostream>
#include <fstream>
#include <sstream>  
#include <iomanip>   
#include <ctime>

bool BorrowManager::bm_BorrowBook(const std::string& memberID, const std::string& bookID){
    if(getActiveBorrowCount(memberID) >= maxBorrowLimit) {return false;}

    for(int i = 0; i < borrowRecords.size(); i++){
        if(borrowRecords[i].getBorrowedBookID() == bookID && !borrowRecords[i].getIsReturned()) {return false;}
    }
    BorrowRecord record(memberID, bookID);
    borrowRecords.push_back(record);
    saveBorrowRecordToFile(record);
    return true;
}

bool BorrowManager::bm_ReturnBook(const std::string& memberID, const std::string& bookID){
    for(int i = 0; i < borrowRecords.size(); i++){
        if(borrowRecords[i].getBorrowedBookID() == bookID && borrowRecords[i].getBorrowerID() == memberID) {
            if(borrowRecords[i].getIsReturned()) {return false;}

            borrowRecords[i].markReturned(); 
            saveBorrowRecordsToFile();
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
    std::string returnStr = borrowRecords[i].getIsReturned() ? timeToString(borrowRecords[i].getReturnDate()) : "Book is not returned.";   
    std::cout << borrowRecords[i].getBorrowerID() << "|" 
    << borrowRecords[i].getBorrowedBookID() << "|"
    << timeToString(borrowRecords[i].getBorrowDate()) << "|" 
    << timeToString(borrowRecords[i].getDueDate()) << "|"
    << returnStr << "|" << std::endl;
    }
}

void BorrowManager::loadBorrowRecordsFromFile() {
    std::ifstream textRecordOpener("BorrowRecords.txt");
    std::string sentence;
    while(std::getline(textRecordOpener,sentence)){
        std::stringstream word(sentence);
        std::string userid, bookid, borrowdate, duedate, returndate;
        std::getline(word,userid,'|');
        std::getline(word,bookid,'|');
        std::getline(word,borrowdate,'|');
        std::getline(word,duedate,'|');
        std::getline(word,returndate,'|');
        bool isReturned = !(returndate == "Book is not returned.");
            if(!isReturned){
                std::chrono::system_clock::time_point temp{};
                returndate = timeToString(temp);
            }
        BorrowRecord br(userid,bookid,stringToTime(borrowdate),stringToTime(duedate),stringToTime(returndate),isReturned);    
        borrowRecords.push_back(br);
    }

}

void BorrowManager::saveBorrowRecordToFile(const BorrowRecord& br){
    std::ofstream textRecordOpener("BorrowRecords.txt",std::ios::app);
    std::string returnStr = br.getIsReturned() ? timeToString(br.getReturnDate()) : "Book is not returned.";   
    textRecordOpener << br.getBorrowerID() << "|" 
    << br.getBorrowedBookID() << "|"
    << timeToString(br.getBorrowDate()) << "|" 
    << timeToString(br.getDueDate()) << "|"
    << returnStr << std::endl;
    textRecordOpener.close();
}

void BorrowManager::saveBorrowRecordsToFile() {
    std:: ofstream textOpen;
    textOpen.open("BorrowRecords.txt");
    for(int i = 0; i < borrowRecords.size(); i++){
    std::string returnStr = borrowRecords[i].getIsReturned() ? timeToString(borrowRecords[i].getReturnDate()) : "Book is not returned.";
    textOpen << borrowRecords[i].getBorrowerID() << "|" 
    << borrowRecords[i].getBorrowedBookID() << "|"
    << timeToString(borrowRecords[i].getBorrowDate()) << "|" 
    << timeToString(borrowRecords[i].getDueDate()) << "|"
    << returnStr << std::endl;
    }
    textOpen.close();
}

std::string BorrowManager::timeToString(std::chrono::system_clock::time_point timePoint) const {
    std::time_t t = std::chrono::system_clock::to_time_t(timePoint); 
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::chrono::system_clock::time_point BorrowManager::stringToTime(const std::string& s) const {
    std::tm tm = {};
    std::stringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return tp;
}