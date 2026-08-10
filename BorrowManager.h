#ifndef BORROWMANAGER_H
#define BORROWMANAGER_H

#include <string>
#include <vector>
#include "BorrowRecord.h"

class BorrowManager{
private:    
    std::vector<BorrowRecord> borrowRecords;
    const int maxBorrowLimit = 3;

public:
    bool bm_BorrowBook(const std::string& memberID, const std::string& bookID);
    bool bm_ReturnBook(const std::string& memberID, const std::string& bookID);

    int getActiveBorrowCount(const std::string& userID) const;
    bool isBookBorrowed(const std::string& bookID) const;
    bool hasActiveBorrow(const std::string& userID, const std::string& bookID) const;
    void bm_displayBorrowInfo() const;
};

#endif