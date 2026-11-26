#include "PenaltyRecord.h"

PenaltyRecord::PenaltyRecord(
    int recordID, 
    int amount, 
    const SysClock::time_point& createdAt, 
    const std::optional<SysClock::time_point>& paidAt):
recordID(recordID),
penaltyAmount(amount),
createdAt(createdAt),
paidAt(paidAt)
{}



int PenaltyRecord::getRecordID() const {return recordID;}
    
int PenaltyRecord::getPenaltyAmount() const {return penaltyAmount;}
    
SysClock::time_point PenaltyRecord::getCreatedAt() const {return createdAt;}
    
std::optional<SysClock::time_point> PenaltyRecord::getPaidAt() const {return paidAt;}