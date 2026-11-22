#include "BookRepository.h"
#include <stdexcept>

std::optional<Book> BookRepository::findBookByISBN(const std::string& isbn) const {
    const char* findBook = "SELECT title, author, category, stock FROM Books WHERE isbn = ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), findBook, -1, &stmt, nullptr);

    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
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

    std::string errorMsg = sqlite3_errmsg(db.handle());
    sqlite3_finalize(stmt);
    throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
}


void BookRepository::addBook(const Book& book) {
    const char* insert2Books = 
    "INSERT INTO Books (isbn , title, author, category, stock)"
    "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), insert2Books, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, book.getISBN().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, book.getCategory().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, book.getAvailableStock());

    result = sqlite3_step(stmt);

    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);
}


bool BookRepository::increaseBookStock(const std::string& isbn, int amount) {//amount kontrolunu librarye bırak (iş kuralı).
    const char* increaseStock = "UPDATE Books SET stock = stock + ? WHERE isbn = ?;";
    
    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), increaseStock, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_int(stmt, 1, amount);
    sqlite3_bind_text(stmt, 2, isbn.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }
    int affect = sqlite3_changes(db.handle());
    sqlite3_finalize(stmt);

    if(!affect)//isbn bulunmuyor
        return false;

    return true;
}

bool BookRepository::decreaseStockIfAvailable(const std::string& isbn) {
    const char* decreaseStock = "UPDATE Books SET stock = stock - 1 WHERE isbn = ? AND stock > 0;";
    
    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), decreaseStock, -1, &stmt, nullptr);
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

    if(!affect)//isbn bulunmuyor yada stock 0 dan küçük yada eşit
        return false;

    return true;
}


bool BookRepository::removeBook(const std::string& isbn) {
    const char* removeBook = "DELETE FROM Books WHERE isbn = ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), removeBook, -1, &stmt, nullptr);
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

    sqlite3_finalize(stmt); 

    if(!sqlite3_changes(db.handle())) {//isbn yok demek false donunce
        return false;
    }
    return true;
}


void BookRepository::fetchBooks(std::vector<Book>& books, int limit, int offset) const { 
     books.clear();

    const char* showBook = 
    "SELECT isbn, title, author, category, stock "
    "FROM Books "
    "ORDER BY title COLLATE NOCASE, isbn "
    "LIMIT ? OFFSET ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), showBook, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
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
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to read books: " + errorMsg);
    }
    sqlite3_finalize(stmt);
}