#include "DatabaseManager.h"
#include "TimeUtils.h"
#include <stdexcept>

DatabaseManager::DatabaseManager() {
    int result = sqlite3_open(fileName, &db);
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
    "memberID TEXT NOT NULL,"
    "penaltyAmount INTEGER NOT NULL,"
    "createdAt TEXT NOT NULL,"
    "paid INTEGER NOT NULL DEFAULT 0,"
    "paidAt TEXT,"
    "FOREIGN KEY (recordID) REFERENCES BorrowRecords(recordID),"
    "FOREIGN KEY (memberID) REFERENCES Members(id)"
    ");";
    result = sqlite3_exec(db, createFinesTable, nullptr, nullptr, nullptr);
    if(result != SQLITE_OK)
        closeAndThrow("Failed to create the Fines table.");
}



void DatabaseManager::addBook(const std::string& isbn, const std::string& title, const std::string& author, const std::string& category, int stock) {
    const char* insert2Books = 
    "INSERT INTO Books (isbn , title, author, category, stock)"
    "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, insert2Books, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, stock);

    result = sqlite3_step(stmt);

    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);
}

void DatabaseManager::addMember(const std::string& id, const std::string& name, const std::string& number, const std::string& mail) {
    const char* insert2Members = 
    "INSERT INTO Members (id, name, number, mail)"
    "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, insert2Members, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, number.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, mail.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);

    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);
}

void DatabaseManager::startBorrowRecord(const BorrowRecord& br) {
    const char* insert2BorrowRecord = 
    "INSERT INTO BorrowRecords (memberID, isbn, borrowDate, dueDate, returnDate)"
    "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, insert2BorrowRecord, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }
     
    std::string memberID = br.getUserID();
    std::string isbn = br.getBorrowedBookID();  
    std::string borrowDate = timeToString(br.getBorrowDate());
    std::string dueDate = timeToString(br.getDueDate());

    sqlite3_bind_text(stmt, 1, memberID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, isbn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, borrowDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, dueDate.c_str(), -1, SQLITE_TRANSIENT);
    if(br.getReturnDate().has_value()) {
        std::string returnDate = timeToString(br.getReturnDate().value());
        sqlite3_bind_text(stmt, 5, returnDate.c_str(), -1, SQLITE_TRANSIENT);
    }else
        sqlite3_bind_null(stmt,5);
    
    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);
}



bool DatabaseManager::removeBook(const std::string& isbn) {
    int result = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to begin transaction: " + errorMsg);
    }

    const char* removeBookBorrowRecord = "DELETE FROM BorrowRecords WHERE isbn = ?;";

    sqlite3_stmt* stmt = nullptr;

    result = sqlite3_prepare_v2(db, removeBookBorrowRecord, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt); 
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt); 
    
    const char* removeBook = "DELETE FROM Books WHERE isbn = ?;";

    stmt = nullptr;

    result = sqlite3_prepare_v2(db, removeBook, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt); 
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt); 

    if(!sqlite3_changes(db)) {//isbn yok demek false donunce
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    result = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to commit transaction: " + errorMsg);
    }

    return true;
}

bool DatabaseManager::removeMember(const std::string& memberID) {
    int result = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to begin transaction: " + errorMsg);
    }

    const char* removeMemberBorrowRecord = "DELETE FROM BorrowRecords WHERE memberID = ?;";

    sqlite3_stmt* stmt = nullptr;

    result = sqlite3_prepare_v2(db, removeMemberBorrowRecord, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, memberID.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt); 
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);

    const char* removeMember = "DELETE FROM Members WHERE id = ?;";

    stmt = nullptr;

    result = sqlite3_prepare_v2(db, removeMember, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, memberID.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt); 
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt); 

    if(!sqlite3_changes(db)) { //memberID yok demek false donunce
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    result = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to commit transaction: " + errorMsg);
    }

    return true;
}



bool DatabaseManager::borrowBook(const BorrowRecord& br) {
    int result = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to begin transaction: " + errorMsg);
    }

    const char* borrowBook = "UPDATE Books SET stock = stock - 1 WHERE isbn = ? AND stock > 0;";

    sqlite3_stmt* stmt = nullptr;

    result = sqlite3_prepare_v2(db, borrowBook, -1 ,&stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    std::string isbn = br.getBorrowedBookID();
    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);
    
    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt); 
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt); 

    int affect = sqlite3_changes(db); //etkilediği satır sayısını döndürür. 0 ise kitap bulunamadı ya da stokta kalmadı.
    if(!affect) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    try {
    startBorrowRecord(br);
    } catch(...) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw;
    }

    result = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to commit transaction: " + errorMsg);
    }

    return true;
}

bool DatabaseManager::returnBook(const std::string& memberID, const std::string& isbn) {
    int result = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to begin transaction: " + errorMsg);
    }

    std::optional<BorrowRecord> activeBorrow;
    try {
        activeBorrow = findActiveBorrow(memberID, isbn);
        if(activeBorrow == std::nullopt) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
    } catch(...) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw;
    }

    const char* bookReturn = "UPDATE BorrowRecords SET returnDate = ? WHERE isbn = ? AND memberID = ? AND returnDate IS NULL;";

    sqlite3_stmt* stmt = nullptr;

    result = sqlite3_prepare_v2(db, bookReturn, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    SysClock::time_point returnTime = SysClock::now();
    std::string returnDate = timeToString(returnTime);

    sqlite3_bind_text(stmt, 1, returnDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, isbn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, memberID.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);

    int affect = sqlite3_changes(db);
    if(!affect) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    try {
        activeBorrow.value().setReturnDate(returnTime);
        if(!increaseBookStock(isbn, 1)) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
        insertFineIfLate(activeBorrow.value());
    } catch(...) { 
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw;// Önce geri al, sonra hatayı olduğu gibi yukarı ilet.
    }

    result = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw std::runtime_error("Failed to commit transaction: " + errorMsg);
    }
    return true;
}



void DatabaseManager::insertFineIfLate(const BorrowRecord& br) { //bu fonksiyona daha iyi bak
    if(br.getReturnDate() == std::nullopt)
        throw std::runtime_error("Book not returned yet");

    if(br.getReturnDate() <= br.getDueDate())
        return;

    SysClock::duration diff = br.getReturnDate().value() - br.getDueDate();
    int hours = std::chrono::ceil<std::chrono::hours> (diff).count();
    int days = (hours + 23)/24;
    int amount = days * dailyPenalty;

    const char* insertFile = 
    "INSERT INTO Fines (recordID, memberID, penaltyAmount, createdAt)" 
    "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, insertFile, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    int recordID = br.getRecordID();
    std::string memberID = br.getUserID();
    SysClock::time_point createdAt = br.getReturnDate().value();

    sqlite3_bind_int(stmt, 1, recordID);
    sqlite3_bind_text(stmt, 2, memberID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, amount);
    sqlite3_bind_text(stmt, 4, timeToString(createdAt).c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);
}

bool DatabaseManager::increaseBookStock(const std::string& isbn, int amount) {
    if(amount <= 0)
        return false;

    const char* increaseStock = "UPDATE Books SET stock = stock + ? WHERE isbn = ?;";
    
    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, increaseStock, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_int(stmt, 1, amount);
    sqlite3_bind_text(stmt, 2, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
    int affect = sqlite3_changes(db);

    if(!affect) {//isbn bulunmuyor
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}



std::optional<Book> DatabaseManager::findBookByISBN(const std::string& isbn) const {
    const char* findBook = "SELECT title, author, category, stock FROM Books WHERE isbn = ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, findBook, -1, &stmt, nullptr);

    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) { 
        std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int stock = sqlite3_column_int(stmt, 3);

        sqlite3_finalize(stmt);
        Book book(isbn, title, author, category, stock);
        return book;
    }
    if(result == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    std::string errorMsg = sqlite3_errmsg(db);
    sqlite3_finalize(stmt);
    throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
}

std::optional<Member> DatabaseManager::findMemberByID(const std::string& id) const {
    const char* findMember = "SELECT id, name, number, mail FROM Members WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, findMember, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) {
        std::string memberID = reinterpret_cast<const char*> (sqlite3_column_text(stmt,0));
        std::string name = reinterpret_cast<const char*> (sqlite3_column_text(stmt,1));
        std::string number = reinterpret_cast<const char*> (sqlite3_column_text(stmt,2));
        std::string mail = reinterpret_cast<const char*> (sqlite3_column_text(stmt,3));
        sqlite3_finalize(stmt);
        Member member(memberID, name, number, mail);
        return member;
    }

    if(result == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    std::string errorMsg = sqlite3_errmsg(db);
    sqlite3_finalize(stmt);
    throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
}

std::optional<BorrowRecord> DatabaseManager::findActiveBorrow(const std::string& memberID, const std::string& isbn) const {
    const char* findBorrow = "SELECT recordID, borrowDate, dueDate FROM BorrowRecords WHERE isbn = ? AND memberID = ? AND returnDate IS NULL;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, findBorrow, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
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

    std::string errorMsg = sqlite3_errmsg(db);
    sqlite3_finalize(stmt);
    throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
}



bool DatabaseManager::memberHasActiveBorrow(const std::string& memberID) const {
    const char* borrowCount = "SELECT EXISTS(SELECT 1 FROM BorrowRecords WHERE memberID = ? AND returnDate IS NULL);";
    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, borrowCount, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }
    sqlite3_bind_text(stmt, 1, memberID.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) {
        int nmbr = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return nmbr != 0;
    } else {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
}

bool DatabaseManager::bookHasActiveBorrow(const std::string& isbn) const {
    const char* borrowCount = "SELECT EXISTS(SELECT 1 FROM BorrowRecords WHERE isbn = ? AND returnDate IS NULL);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, borrowCount, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }
    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) {
        int nmbr = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return nmbr != 0;
    } else {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
}


void DatabaseManager::fetchBooks(std::vector<Book>& books, int limit, int offset) const {
    books.clear();

    const char* showBook = 
    "SELECT isbn, title, author, category, stock "
    "FROM Books "
    "ORDER BY title COLLATE NOCASE, isbn "
    "LIMIT ? OFFSET ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, showBook, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_int(stmt, 1, limit);
    sqlite3_bind_int(stmt, 2, offset);
    
    while((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        int stock = sqlite3_column_int(stmt, 4);

        Book book(isbn, title, author, category, stock);
        books.push_back(book);
    }

    if(result != SQLITE_DONE) {
        books.clear();
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to read books: " + errorMsg);
    }
    sqlite3_finalize(stmt);
}

void DatabaseManager::fetchMembers(std::vector <Member>& members, int limit, int offset) const {
    members.clear();

    const char* showMembers = 
    "SELECT id, name, number, mail "
    "FROM Members "
    "ORDER BY name COLLATE NOCASE, id "
    "LIMIT ? OFFSET ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, showMembers, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_int(stmt, 1, limit);
    sqlite3_bind_int(stmt, 2, offset);
    
    while((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string mail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        Member member(id, name, number, mail);
        members.push_back(member);
    }

    if(result != SQLITE_DONE) {
        members.clear();
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to read members: " + errorMsg);
    }
    sqlite3_finalize(stmt);
}

void DatabaseManager::fetchBorrowRecords(std::vector <BorrowRecord>& borrowRecords, int limit, int offset) const {
    borrowRecords.clear();

    const char* showBorrowRecord = 
    "SELECT recordID, memberID, isbn, borrowDate, dueDate, returnDate "
    "FROM BorrowRecords "
    "ORDER BY memberID COLLATE NOCASE, isbn, recordID "
    "LIMIT ? OFFSET ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db, showBorrowRecord, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db);
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
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to read borrow record: " + errorMsg);
    }
    sqlite3_finalize(stmt);
}



void DatabaseManager::closeAndThrow(const std::string& err) { //constructor exception fırlatırsa destructor çalışmaz, bu yüzden bağlantı burada kapatılıyor.
    std::string message = err + ": " + sqlite3_errmsg(db);
    sqlite3_close(db);
    db = nullptr;
    throw std::runtime_error(message);
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(db);
}
