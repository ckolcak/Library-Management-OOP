#ifndef BORROWREPOSITORY_H
#define BORROWREPOSITORY_H

#include "Database.h"
#include "BorrowRecord.h"
#include "TimeUtils.h"
#include <optional>
#include <vector>
#include <string>

class BorrowRepository {
private:
    Database& db;

public:
    explicit BorrowRepository(Database& database): db(database) {}

    void startBorrowRecord(const BorrowRecord& br);
    [[nodiscard]]bool markAsReturned(const std::string& memberID, const std::string& isbn, const SysClock::time_point& returnDate);

    [[nodiscard]]bool removeRecordsByISBN(const std::string& isbn);
    [[nodiscard]]bool removeRecordsByMemberID(const std::string& memberID);

    std::optional<BorrowRecord> findActiveBorrow(const std::string& memberID, const std::string& isbn) const;

    [[nodiscard]]bool memberHasActiveBorrow(const std::string& memberID) const;
    [[nodiscard]]bool bookHasActiveBorrow(const std::string& isbn) const;

    void fetchBorrowRecords(std::vector<BorrowRecord>& borrowRecords, int limit, int offset) const;

};

#endif
