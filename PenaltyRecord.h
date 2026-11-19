#ifndef PENALTYRECORD_H
#define PENALTYRECORD_H

#include "TimeUtils.h"
#include <string>
#include <optional>


class PenaltyRecord {
private:
    int recordID;
    std::string memberID;
    int penaltyAmount;
    SysClock::time_point createdAt;
    bool isPaid;
    std::optional<SysClock::time_point> paidAt;

public:
    PenaltyRecord(int recordID, const std::string& memberID, int amount, const SysClock::time_point& createdAt, bool paid, const std::optional<SysClock::time_point>& paidAt);

    int getRecordID() const;
    std::string getMemberID() const;
    int getPenaltyAmount() const;
    SysClock::time_point getCreatedAt() const;
    bool getIsPaid() const;
    std::optional<SysClock::time_point> getPaidAt() const;
};

#endif