#ifndef PENALTYMANAGER_H
#define PENALTYMANAGER_H

#include "PenaltyRecord.h"

#include <vector>
#include <string>

class PenaltyManager {
private:
    std::vector<PenaltyRecord> penaltyRecords;
    const int penaltyLimit = 3;

public:
    void pm_displayPenaltyHistory(const std::string& borrowerID) const;
    bool canBorrow(const std::string& borrrowerID) const; 
    void applyBorrowingRestriction(const std::string& borrrowerID) const;
};

#endif 
