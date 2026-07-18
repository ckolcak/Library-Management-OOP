#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <string>
#include <vector>
#include <chrono>
#include <ctime>

class BorrowRecord{
private:
   std::string borrowerID;
   int borrowedBookID;

   std::chrono::system_clock::time_point borrowDate;
   std::chrono::system_clock::time_point returnDate;

public:
   BorrowRecord(const std::string& userID, int bookID);

   std::string getBorrowerID() const;
   int getBorrowedBookID() const;

   std::chrono::system_clock::time_point getBorrowDate() const;
   std::chrono::system_clock::time_point getReturnDate() const;

   void setBorrowDate();
   void setReturnDate();
};

#endif
