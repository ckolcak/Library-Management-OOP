#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <string>
#include <chrono>

class BorrowRecord{
private:
   std::string borrowerID;
   std::string borrowedBookID;
   bool isReturned;
   const int borrowDurationDays = 14;

   std::chrono::system_clock::time_point borrowDate;
   std::chrono::system_clock::time_point dueDate;
   std::chrono::system_clock::time_point returnDate;

public:
   BorrowRecord(const std::string& userID, const std::string& bookID);

   std::string getBorrowerID() const;
   std::string getBorrowedBookID() const;

   std::chrono::system_clock::time_point getBorrowDate() const;
   std::chrono::system_clock::time_point getDueDate() const;
   std::chrono::system_clock::time_point getReturnDate () const;

   bool getIsReturned() const;

   void markReturned();

   bool isOverdue() const;

    std::string timeToString(std::chrono::system_clock::time_point timePoint) const;
};

#endif
