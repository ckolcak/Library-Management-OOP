#include "PenaltyRecord.h"

PenaltyRecord::PenaltyRecord(const BorrowRecord& borrowRecord):
penaltyBorrowerID(borrowRecord.getBorrowerID()),
penaltyBookID(borrowRecord.getBorrowedBookID()),
penaltyDate(borrowRecord.getDueDate())
{}

std::string PenaltyRecord::getPenaltyBorrowerID() const {return penaltyBorrowerID;}

std::string PenaltyRecord::getPenaltyBookID() const {return penaltyBookID;}

