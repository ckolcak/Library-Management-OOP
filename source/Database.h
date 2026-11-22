#ifndef DATABASE_H 
#define DATABASE_H

#include <sqlite3.h>
#include <string>

class Database{
private:
    sqlite3* db = nullptr;
    const char* dbName = "main.db";

    void execute(const std::string& sql);
    void closeAndThrow(const std::string& err);

public:

    Database();

    Database(const Database&) = delete; // Kopyalanamaz: SQLite bağlantısının sahibi. Kopyalanırsa iki nesne aynı bağlantıyı kapatmaya çalışır.
    Database& operator=(const Database&) = delete;

    void beginTransaction();
    void commit();
    void rollback();

    sqlite3* handle() const;

    ~Database();
    
};

#endif