#include "BorrowRepository.h"
#include <stdexcept>

void BorrowRepository::startBorrowRecord(const BorrowRecord& br) {
    const char* insertBorrowRecord = "INSERT INTO BorrowRecords (memberID, isbn, borrowDate, dueDate, returnDate)"
                        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(db.handle(), insertBorrowRecord, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, br.getUserID().c_str() , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, br.getBorrowedBookID().c_str() , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, timeToString(br.getBorrowDate()).c_str() , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, timeToString(br.getDueDate()).c_str() , -1, SQLITE_TRANSIENT);
    if(br.getReturnDate().has_value()) {
        std::string returnDate = timeToString(br.getReturnDate().value());
        sqlite3_bind_text(stmt, 5, returnDate.c_str(), -1, SQLITE_TRANSIENT);
    }else
        sqlite3_bind_null(stmt,5);
    
    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);
}


bool BorrowRepository::markAsReturned(const std::string& memberID, const std::string& isbn, const SysClock::time_point& returnDate) {
    const char* returnUpdate = "UPDATE BorrowRecords SET returnDate = ? WHERE memberID = ? AND isbn = ? AND returnDate IS NULL;";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(db.handle(), returnUpdate, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, timeToString(returnDate).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, memberID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
    int affect = sqlite3_changes(db.handle());
    sqlite3_finalize(stmt);

    if(!affect)
        return false;

    return true;
}


bool BorrowRepository::removeRecordsByISBN(const std::string& isbn) {
    const char* removeRecord = "DELETE FROM BorrowRecords WHERE isbn = ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), removeRecord, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
    int affect = sqlite3_changes(db.handle());
    sqlite3_finalize(stmt);

    if(!affect)//bu isbn ile kayıt bulunmuyor
        return false;

    return true;
}

bool BorrowRepository::removeRecordsByMemberID(const std::string& memberID) {
    const char* removeRecord = "DELETE FROM BorrowRecords WHERE memberID = ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), removeRecord, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, memberID.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
    int affect = sqlite3_changes(db.handle());
    sqlite3_finalize(stmt);

    if(!affect)//bu üyeye ait kayıt bulunmuyor
        return false;

    return true;
}


std::optional<BorrowRecord> BorrowRepository::findActiveBorrow(const std::string& memberID, const std::string& isbn) const {
    const char* findBorrow = "SELECT recordID, borrowDate, dueDate FROM BorrowRecords WHERE isbn = ? AND memberID = ? AND returnDate IS NULL;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), findBorrow, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, memberID.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) {
        int recordID = sqlite3_column_int(stmt,0);
        std::string borrowDate = reinterpret_cast<const char*> (sqlite3_column_text(stmt,1));
        std::string dueDate = reinterpret_cast<const char*> (sqlite3_column_text(stmt,2));
        sqlite3_finalize(stmt);
        BorrowRecord borrowRecord(recordID, memberID, isbn, stringToTime(borrowDate), stringToTime(dueDate), std::nullopt);
        return borrowRecord;
    }

    if(result == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    std::string errorMsg = sqlite3_errmsg(db.handle());
    sqlite3_finalize(stmt);
    throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
}


bool BorrowRepository::memberHasActiveBorrow(const std::string& memberID) const {
   const char* borrowCount = "SELECT EXISTS(SELECT 1 FROM BorrowRecords WHERE memberID = ? AND returnDate IS NULL);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), borrowCount, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }
    sqlite3_bind_text(stmt, 1, memberID.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) {
        int nmbr = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return nmbr != 0;
    } else {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
}

bool BorrowRepository::bookHasActiveBorrow(const std::string& isbn) const {
    const char* borrowCount = "SELECT EXISTS(SELECT 1 FROM BorrowRecords WHERE isbn = ? AND returnDate IS NULL);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), borrowCount, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }
    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) {
        int nmbr = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return nmbr != 0;
    } else {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
}


void BorrowRepository::fetchBorrowRecords(std::vector<BorrowRecord>& borrowRecords, int limit, int offset) const {
    borrowRecords.clear();

    const char* showBorrowRecord = 
    "SELECT recordID, memberID, isbn, borrowDate, dueDate, returnDate "
    "FROM BorrowRecords "
    "ORDER BY memberID COLLATE NOCASE, isbn, recordID "
    "LIMIT ? OFFSET ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), showBorrowRecord, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_int(stmt, 1, limit);
    sqlite3_bind_int(stmt, 2, offset);
    
    while((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        int recordID = sqlite3_column_int(stmt,0);
        std::string memberID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string borrowDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::optional<SysClock::time_point> returnDate = std::nullopt;
        int res = sqlite3_column_type(stmt, 5);
        if(res != SQLITE_NULL) {
            std::string returnDateStr = reinterpret_cast<const char*> (sqlite3_column_text(stmt, 5));
            returnDate = stringToTime(returnDateStr);
        } 

        BorrowRecord borrowrecord(recordID, memberID, isbn, stringToTime(borrowDate), stringToTime(dueDate), returnDate);
        borrowRecords.push_back(borrowrecord);
    }

    if(result != SQLITE_DONE) {
        borrowRecords.clear();
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to read borrow record: " + errorMsg);
    }
    sqlite3_finalize(stmt);
}

