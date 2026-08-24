#ifndef PENALTYRECORD_H
#define PENALTYRECORD_H

#include "BorrowRecord.h"

#include <string>
#include <chrono>
 
class PenaltyRecord{
private:
    std::string penaltyBorrowerID;
    std::string penaltyBookID;
    std::chrono::system_clock::time_point penaltyDate;

public:
    PenaltyRecord(const BorrowRecord& borrowRecord);

    std::string getPenaltyBorrowerID() const;
    std::string getPenaltyBookID() const;

    std::chrono::system_clock::time_point getPenaltyDate() const;
};

#endif