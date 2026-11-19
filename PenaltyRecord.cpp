#include "PenaltyRecord.h"

PenaltyRecord::PenaltyRecord(
    int recordID, 
    const std::string& memberID, 
    int amount, 
    const SysClock::time_point& createdAt, 
    bool paid, 
    const std::optional<SysClock::time_point>& paidAt):
recordID(recordID),
memberID(memberID),
penaltyAmount(amount),
createdAt(createdAt),
isPaid(paid),
paidAt(paidAt)
{}



int PenaltyRecord::getRecordID() const {return recordID;}
   
std::string PenaltyRecord::getMemberID() const {return memberID;}
    
int PenaltyRecord::getPenaltyAmount() const {return penaltyAmount;}
    
SysClock::time_point PenaltyRecord::getCreatedAt() const {return createdAt;}
    
bool PenaltyRecord::getIsPaid() const {return isPaid;}
    
std::optional<SysClock::time_point> PenaltyRecord::getPaidAt() const {return paidAt;}