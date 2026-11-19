#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <string>
#include <chrono>
#include <optional>
#include "TimeUtils.h"

class BorrowRecord{
private:
   int recordID; 
   std::string userID;
   std::string bookID;
   int borrowDurationDays = 14;

   SysClock::time_point borrowDate;
   SysClock::time_point dueDate;
   std::optional<SysClock::time_point> returnDate;

public:
   BorrowRecord(const std::string& userID, const std::string& bookID);

   BorrowRecord(
      int recordID,
      const std::string& userID, 
      const std::string& bookID,
      const SysClock::time_point& borrowDate,
      const SysClock::time_point& dueDate,
      const std::optional<SysClock::time_point>& returnDate
   );

   int getRecordID() const;
   std::string getUserID() const;
   std::string getBorrowedBookID() const;

   SysClock::time_point getBorrowDate() const;
   SysClock::time_point getDueDate() const;
   std::optional<SysClock::time_point> getReturnDate () const;

   void setReturnDate(const SysClock::time_point& rd);
};

#endif
