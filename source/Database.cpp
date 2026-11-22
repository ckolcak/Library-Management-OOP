#include "Database.h"
#include <stdexcept>

Database::Database() {
    int result = sqlite3_open(dbName, &db);
    if(result != SQLITE_OK)
        closeAndThrow("Failed to open the database.");

    const char* enableForeignKey= "PRAGMA foreign_keys = ON;";
    result = sqlite3_exec(db, enableForeignKey, nullptr, nullptr, nullptr);
    if(result != SQLITE_OK)
        closeAndThrow("Failed to enable foreign keys.");

    const char* createBookTable = 
    "CREATE TABLE IF NOT EXISTS Books("
    "isbn TEXT NOT NULL PRIMARY KEY,"
    "title TEXT NOT NULL,"
    "author TEXT NOT NULL,"
    "category TEXT NOT NULL,"
    "stock INTEGER NOT NULL"
    ");";
    result = sqlite3_exec(db, createBookTable, nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) 
        closeAndThrow("Failed to create the Books table.");

    const char* createMemberTable = 
    "CREATE TABLE IF NOT EXISTS Members("
    "id TEXT NOT NULL PRIMARY KEY,"
    "name TEXT NOT NULL,"
    "number TEXT NOT NULL,"
    "mail TEXT NOT NULL"
    ");";
    result = sqlite3_exec(db, createMemberTable, nullptr, nullptr, nullptr);
    if(result != SQLITE_OK)
        closeAndThrow("Failed to create the Members table.");

    const char* createBorrowRecordTable = 
    "CREATE TABLE IF NOT EXISTS BorrowRecords("
    "recordID INTEGER PRIMARY KEY AUTOINCREMENT,"
    "memberID TEXT NOT NULL,"
    "isbn TEXT NOT NULL,"
    "borrowDate TEXT NOT NULL,"
    "dueDate TEXT NOT NULL,"
    "returnDate TEXT,"
    "FOREIGN KEY (memberID) REFERENCES Members(id),"
    "FOREIGN KEY (isbn) REFERENCES Books(isbn)"
    ");";
    result = sqlite3_exec(db, createBorrowRecordTable, nullptr, nullptr, nullptr);
    if(result != SQLITE_OK)
        closeAndThrow("Failed to create the BorrowRecords table.");

    const char* createFinesTable =
    "CREATE TABLE IF NOT EXISTS Fines("
    "recordID INTEGER PRIMARY KEY,"
    "penaltyAmount INTEGER NOT NULL,"
    "createdAt TEXT NOT NULL,"
    "paidAt TEXT,"
    "FOREIGN KEY (recordID) REFERENCES BorrowRecords(recordID)"
    ");";
    result = sqlite3_exec(db, createFinesTable, nullptr, nullptr, nullptr);
    if(result != SQLITE_OK)
        closeAndThrow("Failed to create the Fines table.");
}


void Database::beginTransaction() {
    std::string transaction =  "BEGIN TRANSACTION;";
    execute(transaction);
} 

void Database::commit() {
    std::string commit = "COMMIT;";
    execute(commit);
}

void Database::rollback() {
    std::string rollback = "ROLLBACK;";
    sqlite3_exec(db, rollback.c_str(), nullptr, nullptr, nullptr);
}

sqlite3* Database::handle() const {
    return db;
}


void Database::closeAndThrow(const std::string& err) { //constructor exception fırlatırsa destructor çalışmaz, bu yüzden bağlantı burada kapatılıyor.
    std::string message = err + ": " + sqlite3_errmsg(db);
    sqlite3_close(db);
    db = nullptr;
    throw std::runtime_error(message);
}

void Database::execute(const std::string& sql) {
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db));
}


Database::~Database() {
    sqlite3_close(db);
}