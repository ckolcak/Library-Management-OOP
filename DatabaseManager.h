#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>
#include "Book.h"
#include "Member.h"
#include "BorrowRecord.h"

class DatabaseManager {
private:
    sqlite3* db = nullptr;
    const char* fileName = "main.db";
    const int dailyPenalty = 500;

    void closeAndThrow(const std::string& err);
    void startBorrowRecord(const BorrowRecord& br);
    void insertFineIfLate(const BorrowRecord& br);
    [[nodiscard]]bool increaseBookStock(const std::string& isbn, int amount);

public:
    DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete; // Kopyalanamaz: SQLite bağlantısının sahibi. Kopyalanırsa iki nesne aynı bağlantıyı kapatmaya çalışır.
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void addBook(const std::string& isbn, const std::string& title, const std::string& author, const std::string& category, int stock);
    void addMember(const std::string& id, const std::string& name, const std::string& number, const std::string& mail);

    [[nodiscard]]bool removeBook(const std::string& isbn); 
    [[nodiscard]]bool removeMember(const std::string& memberID);

    std::optional<Book> findBookByISBN(const std::string& isbn) const;
    std::optional<Member> findMemberByID(const std::string& id) const;
    std::optional<BorrowRecord> findActiveBorrow(const std::string& memberID, const std::string& isbn) const;

    [[nodiscard]]bool borrowBook(const BorrowRecord& br);
    [[nodiscard]]bool returnBook(const std::string& memberID, const std::string& isbn);

    [[nodiscard]]bool memberHasActiveBorrow(const std::string& memberID) const;
    [[nodiscard]]bool bookHasActiveBorrow(const std::string& isbn) const;

    void fetchBooks(std::vector<Book>& books, int limit, int offset) const;
    void fetchMembers(std::vector <Member>& members, int limit, int offset) const;
    void fetchBorrowRecords(std::vector <BorrowRecord>& borrowRecords, int limit, int offset) const;

    ~DatabaseManager();

    //ipin ucu kaçtı en baştan kendine hatırlat
};
     
#endif 