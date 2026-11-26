#ifndef PENALTYRECORD_H
#define PENALTYRECORD_H

#include "TimeUtils.h"
#include <string>
#include <optional>


class PenaltyRecord {
private:
    int recordID;
    int penaltyAmount;
    SysClock::time_point createdAt;
    std::optional<SysClock::time_point> paidAt;

public:
    PenaltyRecord(int recordID, int amount, const SysClock::time_point& createdAt, const std::optional<SysClock::time_point>& paidAt);

    int getRecordID() const;
    int getPenaltyAmount() const;
    SysClock::time_point getCreatedAt() const;
    std::optional<SysClock::time_point> getPaidAt() const;
};

#endif